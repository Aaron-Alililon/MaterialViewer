Texture2D displacementTex : register(t2);
SamplerState sampleType : register(s0);

cbuffer MatrixBuffer : register(b0) {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix worldInverseTranspose;
};

cbuffer PropertiesBuffer : register(b1) {
    float4 sunDirection;
    float globalIllumination;
    float3 _;
    float displacementStrength;
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
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

PixelInputType VSMain(VertexInputType input) {
    PixelInputType output;
    
    output.uv = input.uv * float2(8.0f, 4.0f);
    
    float displacement = displacementTex.SampleLevel(sampleType, output.uv, 0).r;
    float4 displaced = input.position + float4(input.normal * displacement * displacementStrength, 0.0);
    
    input.position.w = 1.0f;

    output.position = mul(displaced, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.normal = mul(input.normal, (float3x3) worldInverseTranspose);
    
    output.tangent = mul(input.tangent, (float3x3) worldInverseTranspose);
    
    output.binormal = mul(input.binormal, (float3x3) worldInverseTranspose);
    
    return output;
}