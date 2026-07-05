SamplerState sampleType : register(s0);

cbuffer MatrixBuffer : register(b0) {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix worldInverseTranspose;
};

cbuffer PropertiesBuffer : register(b1) {
    float2 uvScale;
    float globalIllumination;
    float displacementStrength;
    bool usePOM;
    float2 minMaxPOMLayers;
};

struct VertexInputType {
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float4 worldPos : TEXCOORD1;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

PixelInputType VSMain(VertexInputType input) {
    PixelInputType output;
    
    output.uv = input.uv * uvScale;
    
    input.position.w = 1.0f;

    output.worldPos = mul(input.position, worldMatrix);
    output.position = mul(output.worldPos, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.normal = mul(input.normal, (float3x3) worldInverseTranspose);
    
    output.tangent = mul(input.tangent, (float3x3) worldInverseTranspose);
    
    output.binormal = mul(input.binormal, (float3x3) worldInverseTranspose);
    
    return output;
}