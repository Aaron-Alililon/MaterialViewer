TextureCube skyCube : register(t0);
SamplerState sampleType : register(s0);

struct PixelInputType {
  float4 position : SV_POSITION;
  float3 worldPos : TEXCOORD0;
};

float3 tonemapACES(float3 x) {
  const float a = 2.51, b = 0.03, c = 2.43, d = 0.59, e = 0.14;
  return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

float4 PSMain(PixelInputType input) : SV_TARGET {
  float4 hdr = skyCube.Sample(sampleType, normalize(input.worldPos));
  float3 sdr = tonemapACES(hdr.rgb);
  return float4(sdr, 1);
}