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

float3 tonemapNeutral(float3 color) {
  const float startCompression = 0.8 - 0.04;
  const float desaturation = 0.15;

  float x = min(color.r, min(color.g, color.b));
  float offset = x < 0.08 ? x - 6.25 * x * x : 0.04;
  color -= offset;

  float peak = max(color.r, max(color.g, color.b));
  if (peak < startCompression)
    return color;

  const float d = 1.0f - startCompression;
  float newPeak = 1.0f - d * d / (peak + d - startCompression);
  color *= newPeak / peak;

  float g = 1.0f - 1.0f / (desaturation * (peak - newPeak) + 1.0f);
  return lerp(color, newPeak * float3(1, 1, 1), g);
}

float3 tonemapACES(float3 color) {
  const float a = 2.51, b = 0.03, c = 2.43, d = 0.59, e = 0.14;
  return saturate((color * (a * color + b)) / (color * (c * color + d) + e));
}

float4 PSMain(PixelInputType input) : SV_TARGET {
  float3 outCol = skyCube.Sample(sampleType, normalize(input.worldPos)).rgb;
  
  if (tonemapMethod == 1) {
    outCol = tonemapNeutral(outCol * exposure);
  } else if (tonemapMethod == 2) {
    outCol = tonemapACES(outCol * exposure);
  }
  
  return float4(outCol, 1);
}