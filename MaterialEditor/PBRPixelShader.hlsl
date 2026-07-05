Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D displacementTex : register(t2);
Texture2D roughnessTex : register(t3);
Texture2D metallicTex : register(t4);
Texture2D ambientOcclusionTex : register(t5);
SamplerState sampleType : register(s0);

cbuffer PropertiesBuffer : register(b1) {
    float2 uvScale;
    float globalIllumination;
    float displacementStrength;
    bool usePOM;
    float2 minMaxPOMLayers;
};

struct LightData {
    float4 position;
    float4 direction;
    float4 color;
};

StructuredBuffer<LightData> directionals : register(t6);

cbuffer NumDirectionalsBuffer : register(b2) {
    int numDirectionals;
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
    
    float2 P = tangentView.xy * displacementStrength;
    float2 deltaTexCoords = P / numLayers;
    
    float2 currentTexCoords = uv;
    float currentDepthMapValue = 1.0f - displacementTex.SampleLevel(sampleType, currentTexCoords, 0).r;
  
    while (currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = 1.0f - displacementTex.SampleLevel(sampleType, currentTexCoords, 0).r;
        currentLayerDepth += layerDepth;
    }
    
    float2 prevTexCoords = currentTexCoords + deltaTexCoords;
    
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = 1.0f - displacementTex.SampleLevel(sampleType, prevTexCoords, 0).r - currentLayerDepth + layerDepth;
 
    float weight = afterDepth / (afterDepth - beforeDepth);
    float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

float3 F_Schlick(float VdotH, float3 F0) {
    return F0 + (1.0 - F0) * pow(saturate(1.0 - VdotH), 5.0);
}

float D_GGX(float NdotH, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    return a2 / (3.141592f * denom * denom);
}

float G_SchlickGGX(float NdotV, float roughness) {
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float G_Smith(float NdotV, float NdotL, float roughness) {
    float ggx1 = G_SchlickGGX(NdotV, roughness);
    float ggx2 = G_SchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

float3 specular(float3 F, float D, float G, float NdotV, float NdotL) {
    float3 num = F * D * G;
    float denom = max(4 * NdotV * NdotL, 0.00001f);
    return num / denom;
}

float3 reflectance(float3 normal, float3 view, float3 albedo, float roughness, float metallic, float3 lightColor, float3 lightDirection) {
    float3 halfway = normalize(view + lightDirection);
        
    float NdotV = saturate(dot(normal, view));
    float NdotL = saturate(dot(normal, lightDirection));
    float NdotH = saturate(dot(normal, halfway));
    float VdotH = saturate(dot(view, halfway));
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
    
    float3 F = F_Schlick(VdotH, F0);
    float D = D_GGX(NdotH, roughness);
    float G = G_Smith(NdotV, NdotL, roughness);
    
    float3 d = (1.0f - F) * (1.0f - metallic);
        
    return lightColor * NdotL * (d * albedo + specular(F, D, G, NdotV, NdotL));
}

float4 PSMain(PixelInputType input) : SV_TARGET
{
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent - N * dot(N, input.tangent));
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    float3x3 inverseTBN = transpose(TBN);
    
    float3 viewVector = normalize(camPosition.xyz - input.worldPos.xyz);
    float3 tangentViewVector = normalize(mul(inverseTBN, viewVector));
    float2 parallaxMappedUVs = parallaxMapping(input.uv, tangentViewVector);
    
    if (parallaxMappedUVs.x > uvScale.x || parallaxMappedUVs.y > uvScale.y || parallaxMappedUVs.x < 0.0 || parallaxMappedUVs.y < 0.0)
        discard;
    
    float3 albedo = albedoTex.Sample(sampleType, parallaxMappedUVs).xyz;
    float roughness = roughnessTex.Sample(sampleType, parallaxMappedUVs).r;
    float metallic = metallicTex.Sample(sampleType, parallaxMappedUVs).r;
    float ao = ambientOcclusionTex.Sample(sampleType, parallaxMappedUVs).r;
    
    float3 normal = normalTex.Sample(sampleType, parallaxMappedUVs).rgb * 2.0 - 1.0;
    normal.g *= -1;
    float3 worldNormal = normalize(mul(normal, TBN));
    
    float3 reflectanceSum = 0;
    for (int i = 0; i < numDirectionals; i++) {
        float3 lightColor = directionals[i].color.xyz;
        float3 lightDirection = directionals[i].direction.xyz;
        reflectanceSum += reflectance(worldNormal, viewVector, albedo, roughness, metallic, lightColor, lightDirection);
    }
    
    return float4(globalIllumination * albedo * ao + reflectanceSum, 1);
}