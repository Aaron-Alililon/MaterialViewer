struct VertexInputType {
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float sTangent : TEXCOORD1;
    float sBinormal : TEXCOORD2;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float sTangent : TEXCOORD0;
    float sBinormal : TEXCOORD1;
};

PixelInputType VSMain(VertexInputType input) {
    PixelInputType output;
    
    output.position = float4(input.uv * 2 - 1, 0, 1);
    output.tangent = input.tangent;
    output.binormal = input.binormal;
    output.sTangent = input.sTangent;
    output.sBinormal = input.sBinormal;
    
    return output;
}