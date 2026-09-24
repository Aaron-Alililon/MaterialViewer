cbuffer MatrixBuffer : register(b0) {
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
  matrix worldInverseTranspose;
};

cbuffer PropertiesBuffer : register(b0) {
  int tonemapMethod;
  float exposure;
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
  float3 worldPos : TEXCOORD0;
};

PixelInputType VSMain(VertexInputType input) {
  PixelInputType output;
    
  input.position.w = 1.0f;
  
  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  output.worldPos = mul(input.position, worldMatrix).xyz;
  
  return output;
}