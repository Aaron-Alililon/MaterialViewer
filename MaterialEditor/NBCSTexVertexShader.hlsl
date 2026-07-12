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
  float3 normal : NORMAL;
  float3 tangent : TANGENT;
  float sTangent : TEXCOORD0;
  float sBinormal : TEXCOORD1;
  float2 uv : TEXCOORD2;
};

PixelInputType VSMain(VertexInputType input) {
  PixelInputType output;
  
  output.position = float4(input.uv.x * 2 - 1, (1 - input.uv.y) * 2 - 1, 0, 1);
  output.tangent = input.tangent;
  output.normal = input.normal;
  output.sTangent = input.sTangent;
  output.sBinormal = input.sBinormal;
  output.uv = input.uv;
    
  return output;
}