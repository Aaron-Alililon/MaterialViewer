#include "Common/Tonemap.hlsli"

TextureCube skyCube : register(t0);
SamplerState sampleType : register(s0);

cbuffer PropertiesBuffer : register(b1) {
  int tonemapMethod;
  float exposure;
};

struct PixelInputType {
  float4 position : SV_POSITION;
  float3 worldPos : TEXCOORD0;
};

float4 PSMain(PixelInputType input) : SV_TARGET {
  float3 hdrCol = skyCube.Sample(sampleType, normalize(input.worldPos)).rgb;
  
  float3 sdrCol = applyTonemap(hdrCol, tonemapMethod, exposure);
  
  return float4(sdrCol, 1);
}