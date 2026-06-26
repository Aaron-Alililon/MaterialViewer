cbuffer MatrixBuffer {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType {
    float4 position : POSITION;
    float3 normal : NORMAL;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

PixelInputType VSMain(VertexInputType input) {
    PixelInputType output;
    
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.normal = input.normal;
    
    return output;
}