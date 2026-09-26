#include "Common/Tonemap.hlsli"

#define POM_DISPLACEMENT_FACTOR 2.0f
#define NBCS_DISPLACEMENT_FACTOR 4.0f
#define NBCS_BINARY_STEPS 5
#define IBL_MAX_SPECULAR_MIP 5.0f
#define REFLECTION_SHADOW_STEPS 12

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D displacementTex : register(t2);
Texture2D roughnessTex : register(t3);
Texture2D metallicTex : register(t4);
Texture2D ambientOcclusionTex : register(t5);
Texture2D normalMapTex : register(t6);
Texture2D tangentMapTex : register(t7);
TextureCube skyboxIrradianceCube : register(t8);
TextureCube skyboxSpecularCube : register(t9);
Texture2D brdfLutTex : register(t10);

SamplerState pointSampleType : register(s0);
SamplerState linearSampleType : register(s1);

cbuffer MatrixBuffer : register(b0) {
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
  matrix worldInverseTranspose;
};

cbuffer PropertiesBuffer : register(b1) {
  float2 uvScale;
  float globalIllumination;
  int tonemapMethod;
  float exposure;
  float displacementStrength;
  int displacementMethod;
  float nbcsStepSizeFactor;
  float2 minMaxPOMLayers;
  int selfOcclusionMethod;
  float horizonFade;
};

cbuffer CameraBuffer : register(b3) {
  float4 camPosition;
};

struct PixelInputType {
  float4 position : SV_POSITION;
  float4 worldPos : TEXCOORD1;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
  float3 tangent : TANGENT;
  float3 binormal : BINORMAL;
};

float2 parallaxMapping(float2 uv, float3 tangentView) {
  float numLayers = lerp(minMaxPOMLayers.y, minMaxPOMLayers.x, max(dot(float3(0.0, 0.0, 1.0), tangentView), 0.0));
    
  float layerDepth = 1.0 / numLayers;
  float currentLayerDepth = 0.0;
    
  float2 P = tangentView.xy * (displacementStrength * POM_DISPLACEMENT_FACTOR);
  float2 deltaTexCoords = P / numLayers;
    
  float2 currentTexCoords = uv;
  float currentDepthMapValue = 1.0f - displacementTex.SampleLevel(linearSampleType, currentTexCoords, 0).r;
  
  while (currentLayerDepth < currentDepthMapValue) {
    currentTexCoords -= deltaTexCoords;
    currentDepthMapValue = 1.0f - displacementTex.SampleLevel(linearSampleType, currentTexCoords, 0).r;
    currentLayerDepth += layerDepth;
  }
    
  float2 prevTexCoords = currentTexCoords + deltaTexCoords;
    
  float afterDepth = currentDepthMapValue - currentLayerDepth;
  float beforeDepth = 1.0f - displacementTex.SampleLevel(linearSampleType, prevTexCoords, 0).r - currentLayerDepth + layerDepth;
 
  float weight = afterDepth / (afterDepth - beforeDepth);
  float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

  return finalTexCoords;
}

float2 nbcs(float2 uv, float3 objectSpaceViewVector, float3 tangentViewVector) {
  float numSteps = lerp(minMaxPOMLayers.y, minMaxPOMLayers.x, max(dot(float3(0.0, 0.0, 1.0), tangentViewVector), 0.0));
  float stepSize = 1.0f / numSteps;
    
  float3 uv_i = float3(uv / uvScale, 0);
  float3 prevUV_i = uv_i;
    
  for (int i = 0; i < numSteps; i++) {
    float4 t = tangentMapTex.SampleLevel(linearSampleType, uv_i.xy, 0);
    float4 n = normalMapTex.SampleLevel(linearSampleType, uv_i.xy, 0);
    float3 b = normalize(cross(t.xyz, n.xyz));
    float3x3 TBN = float3x3(t.xyz, b, n.xyz);
        
    float3 tangentSpaceV = mul(TBN, objectSpaceViewVector);
    tangentSpaceV.x /= t.w;
    tangentSpaceV.y /= n.w;
    tangentSpaceV.z /= -max(0.001f, displacementStrength * NBCS_DISPLACEMENT_FACTOR);
    
    prevUV_i = uv_i;
    uv_i += nbcsStepSizeFactor * stepSize * tangentSpaceV;

    if (uv_i.z < 0) {
      discard;
      return 0;
    }
    
    float curHeight = 1.0f - displacementTex.SampleLevel(linearSampleType, uv_i.xy * uvScale, 0).r;
    
    if (uv_i.z > curHeight) {
      float heightPrev = 1.0f - displacementTex.SampleLevel(linearSampleType, prevUV_i.xy * uvScale, 0).r;

      float3 lo = prevUV_i;
      float3 hi = uv_i;
      float loD = heightPrev - lo.z;
      float hiD = curHeight - hi.z;

      [unroll]
      for (int b = 0; b < NBCS_BINARY_STEPS; b++) {
        float3 mid = lerp(lo, hi, 0.5f);
        float h = 1.0f - displacementTex.SampleLevel(linearSampleType, mid.xy * uvScale, 0).r;
        float d = h - mid.z;
        if (d > 0) {
          lo = mid;
          loD = d;
        } else {
          hi = mid;
          hiD = d;
        }
      }
      
      return lerp(lo.xy, hi.xy, loD / (loD - hiD)) * uvScale;
    }
  }

  discard;
  return 0;
}

float3 F_SchlickRoughness(float NdotV, float3 F0, float roughness) {
  return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(saturate(1.0 - NdotV), 5.0);
}

float heightFieldVisibility(float2 uv, float3 dirTS, float softness) {
  if (dirTS.z <= 0.0)
    return 0.0; // pointing into the surface

  float startDepth = 1.0f - displacementTex.SampleLevel(linearSampleType, uv, 0).r;
  float2 uvPerDepth = dirTS.xy * (displacementStrength * POM_DISPLACEMENT_FACTOR);

  float vis = 1.0;
  for (int k = 1; k <= REFLECTION_SHADOW_STEPS; k++) {
    float t = (float) k / REFLECTION_SHADOW_STEPS;
    float rayDepth = startDepth * (1.0 - t); // climbs to the top surface
    float2 sampleUV = uv + uvPerDepth * (startDepth * t);
    float surfDepth = 1.0f - displacementTex.SampleLevel(linearSampleType, sampleUV, 0).r;

    // < 0 means the ray is below the surface (blocked); dividing by t makes far occluders count less
    float diff = surfDepth - rayDepth;
    vis = min(vis, saturate(1.0 + softness * diff / t));
  }
  return vis;
}

float4 PSMain(PixelInputType input) : SV_TARGET {
  float3x3 TBN = float3x3(input.tangent, input.binormal, input.normal);
    
  float3 viewVector = normalize(camPosition.xyz - input.worldPos.xyz);
    
  float2 mappedUVs = input.uv;
  
  // --- Displacement --- //
  if (displacementMethod == 1 || displacementMethod == 2) {
    float3x3 worldInverse = transpose((float3x3) worldInverseTranspose);
    float3 objectSpaceViewVector = normalize(mul(viewVector, worldInverse));
    float3 tangentViewVector = normalize(mul(TBN, objectSpaceViewVector));
  
    if (displacementMethod == 1) { // POM
      mappedUVs = parallaxMapping(input.uv, tangentViewVector);
    } else { // NBCS
      mappedUVs = nbcs(input.uv, -objectSpaceViewVector, tangentViewVector);
    }
  }
    
  float3 albedo = albedoTex.SampleLevel(linearSampleType, mappedUVs, 0).xyz;
  float roughness = roughnessTex.SampleLevel(linearSampleType, mappedUVs, 0).r;
  float metallic = metallicTex.SampleLevel(linearSampleType, mappedUVs, 0).r;
  float ao = ambientOcclusionTex.SampleLevel(linearSampleType, mappedUVs, 0).r;
    
  float3 normal = normalTex.SampleLevel(linearSampleType, mappedUVs, 0).rgb * 2.0 - 1.0;
  normal.g *= -1;
  float3 worldNormal = normalize(mul(normal, TBN));
  float NdotV = saturate(dot(worldNormal, viewVector));
  float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
  
  // --- IBL --- //
  float3 kS = F_SchlickRoughness(NdotV, F0, roughness);
  float3 kD = (1.0 - kS) * (1.0 - metallic);

  float3 irradiance = skyboxIrradianceCube.Sample(linearSampleType, worldNormal).rgb;
  float3 diffuseIBL = kD * irradiance * albedo;

  float3 reflectVector = reflect(-viewVector, worldNormal);
  float3 prefilteredColor = skyboxSpecularCube.SampleLevel(linearSampleType, reflectVector, roughness * IBL_MAX_SPECULAR_MIP).rgb;

  float2 brdf = brdfLutTex.SampleLevel(pointSampleType, float2(NdotV, roughness), 0).rg;
  float3 specularIBL = prefilteredColor * (kS * brdf.x + brdf.y);
  
  if (selfOcclusionMethod == 1 && displacementMethod != 0) {
    float3 reflectTS = normalize(mul(TBN, reflectVector)); // world -> tangent (TBN rows are the basis vectors)
    float vis = heightFieldVisibility(mappedUVs, reflectTS, 8.0 * (1.0 - roughness) + 1.0);
    specularIBL *= vis;
  }
  
  if (selfOcclusionMethod == 2) {
    float3 geoNormal = normalize(input.normal);
    float horizon = saturate(1.0 + horizonFade * dot(reflectVector, geoNormal));
    specularIBL *= horizon * horizon;
  }
  
  float3 hdrCol = (diffuseIBL + specularIBL) * ao * globalIllumination;
  
  float3 sdrCol = applyTonemap(hdrCol, tonemapMethod, exposure);
  
  return float4(sdrCol, 1);
}