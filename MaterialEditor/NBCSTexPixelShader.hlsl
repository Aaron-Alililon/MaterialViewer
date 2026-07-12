struct PixelInputType {
  float4 position : SV_POSITION;
  float3 normal : NORMAL;
  float3 tangent : TANGENT;
  float sTangent : TEXCOORD0;
  float sBinormal : TEXCOORD1;
  float2 uv : TEXCOORD2;
};

struct PSOutput {
  float4 normalOut : SV_TARGET0;
  float4 tangentOut : SV_TARGET1;
};

PSOutput PSMain(PixelInputType input) {
  PSOutput output;
    
  output.normalOut = float4(input.normal, input.sBinormal); // when drawing texture to sphere: red (+x) is left, green (+y) is down, blue (+z) is towards the cam
  output.tangentOut = float4(input.tangent, input.sTangent); // when drawing texture to sphere: red points roughly towards cam, blue roughly to the right, back left quarter is black
    
  return output;
}