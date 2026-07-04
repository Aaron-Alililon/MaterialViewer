#define MAX_LIGHTS 3

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
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

StructuredBuffer<LightData> directionals : register(t5);

cbuffer NumDirectionalsBuffer : register(b2) {
    int numDirectionals;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

float4 PSMain(PixelInputType input) : SV_TARGET {
    float4 textureColor = albedoTex.Sample(sampleType, input.uv);
    float3 textureNormal = normalTex.Sample(sampleType, input.uv).rgb * 2.0 - 1.0;
    textureNormal = float3(textureNormal.r, textureNormal.g * -1, textureNormal.b);
    
    float3x3 TBN = float3x3(
        normalize(input.tangent),
        normalize(input.binormal),
        normalize(input.normal)
    );
    float3 worldNormal = normalize(mul(textureNormal, TBN));
    
    float3 lambert = 0;
    for (int i = 0; i < numDirectionals; i++) {
        lambert = saturate(lambert + max(0, dot(worldNormal, normalize(directionals[i].direction.xyz))) * directionals[i].color.rgb);
    }
    lambert = max(globalIllumination, lambert);
    float3 diffuse = textureColor.rgb * lambert;
    
    return float4(diffuse, 1);
}