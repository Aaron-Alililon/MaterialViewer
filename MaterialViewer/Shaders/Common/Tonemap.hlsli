#ifndef TONEMAP_HLSLI
#define TONEMAP_HLSLI

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

float3 applyTonemap(float3 color, int method, float exposure) {
  color *= exposure;
  if (method == 1)
    return tonemapNeutral(color);
  if (method == 2)
    return tonemapACES(color);
  return color;
}

#endif