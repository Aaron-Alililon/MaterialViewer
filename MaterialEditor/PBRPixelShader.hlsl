#define MAX_LIGHTS 3

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D roughnessTex : register(t3);
Texture2D metallicTex : register(t4);
Texture2D ambientOcclusionTex : register(t5);
SamplerState sampleType : register(s0);

cbuffer PropertiesBuffer : register(b1) {
    float4 sunDirection;
    float2 uvScale;
    float globalIllumination;
    float displacementStrength;
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

float4 PSMain(PixelInputType input) : SV_TARGET {
    float3 albedo = albedoTex.Sample(sampleType, input.uv).xyz;
    float roughness = roughnessTex.Sample(sampleType, input.uv).r;
    float metallic = metallicTex.Sample(sampleType, input.uv).r;
    float ao = ambientOcclusionTex.Sample(sampleType, input.uv).r;
    
    float3 normal = normalTex.Sample(sampleType, input.uv).rgb * 2.0 - 1.0;
    normal = float3(normal.r, normal.g * -1, normal.b);
    
    float3x3 TBN = float3x3(
        normalize(input.tangent),
        normalize(input.binormal),
        normalize(input.normal)
    );
    float3 worldNormal = normalize(mul(normal, TBN));
    
    float3 view = normalize(camPosition.xyz - input.worldPos.xyz);
    
    float3 reflectanceSum = 0;
    for (int i = 0; i < numDirectionals; i++) {
        float3 lightColor = directionals[i].color;
        float3 lightDirection = directionals[i].direction;
        reflectanceSum += reflectance(worldNormal, view, albedo, roughness, metallic, lightColor, lightDirection);
    }
    return float4(globalIllumination * albedo * ao + reflectanceSum, 1);
}