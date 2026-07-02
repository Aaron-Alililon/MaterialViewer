cbuffer MatrixBuffer : register(b0) {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix worldInverseTranspose;
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
    
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.normal = mul(input.normal, (float3x3) worldInverseTranspose);
    
    output.uv = input.uv;
    
    output.tangent = mul(input.tangent, (float3x3) worldInverseTranspose);
    
    output.binormal = mul(input.binormal, (float3x3) worldInverseTranspose);
    
    return output;
}