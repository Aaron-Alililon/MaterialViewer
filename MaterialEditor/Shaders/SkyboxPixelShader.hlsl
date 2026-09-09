Texture2D albedoTex : register(t0);
SamplerState sampleType : register(s0);

struct PixelInputType {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

float4 PSMain(PixelInputType input) : SV_TARGET {
    float4 textureColor = albedoTex.Sample(sampleType, input.uv);
    return textureColor;
}