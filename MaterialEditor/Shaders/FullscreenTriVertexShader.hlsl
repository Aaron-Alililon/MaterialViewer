struct VSOutput {
  float4 position : SV_Position;
  float2 uv : TEXCOORD0;
};

VSOutput VSMain(uint vertexID : SV_VertexID) {
  VSOutput output;
  
  // id 0 -> (0,0), id 1 -> (2,0), id 2 -> (0,2)
  output.uv = float2((vertexID << 1) & 2, vertexID & 2);

  // Map UV [0,2] -> clip space [-1,3], so the triangle
  // fully covers [-1,1] NDC and extends past it (gets clipped)
  output.position = float4(output.uv * 2.0f - 1.0f, 0.0f, 1.0f);

  // D3D clip space has +Y up but the UV convention above has +Y down (image-style);
  // flip Y on position only, so the PS still gets "natural" 0..1 UVs
  output.position.y = -output.position.y;

  return output;
}