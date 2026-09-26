struct PSInput {
  float4 position : SV_Position;
  float2 uv : TEXCOORD0;
};

static const float PI = 3.14159265f;
static const uint SAMPLE_COUNT = 1024u;

// Van der Corput / Hammersley for low-discrepancy 2D samples //
float radicalInverseVdC(uint bits) {
  bits = (bits << 16u) | (bits >> 16u);
  bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
  bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
  bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
  bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
  return float(bits) * 2.3283064365386963e-10;
}

float2 hammersley(uint i, uint n) {
  return float2(float(i) / float(n), radicalInverseVdC(i));
}
// --- //

float3 importanceSampleGGX(float2 xi, float3 n, float rough) {
  float a = rough * rough;

  float phi = 2.0 * PI * xi.x;
  float cosTheta = sqrt((1.0 - xi.y) / (1.0 + (a * a - 1.0) * xi.y));
  float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

  float3 h;
  h.x = sinTheta * cos(phi);
  h.y = sinTheta * sin(phi);
  h.z = cosTheta;

  float3 up_ = abs(n.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
  float3 tangentX = normalize(cross(up_, n));
  float3 tangentY = cross(n, tangentX);

  return normalize(tangentX * h.x + tangentY * h.y + n * h.z);
}

float geometrySchlickGGX(float nDotV, float rough) {
  float a = rough;
  float k = (a * a) / 2.0;
  return nDotV / (nDotV * (1.0 - k) + k);
}

float geometrySmith(float nDotV, float nDotL, float rough) {
  return geometrySchlickGGX(nDotV, rough) * geometrySchlickGGX(nDotL, rough);
}

float2 integrateBRDF(float nDotV, float roughness) {
  float3 v;
  v.x = sqrt(1.0 - nDotV * nDotV);
  v.y = 0.0;
  v.z = nDotV;

  float a = 0.0;
  float b = 0.0;

  float3 n = float3(0.0, 0.0, 1.0);

  for (uint i = 0u; i < SAMPLE_COUNT; i++) {
    float2 xi = hammersley(i, SAMPLE_COUNT);
    float3 h = importanceSampleGGX(xi, n, roughness);
    float3 l = normalize(2.0 * dot(v, h) * h - v);

    float nDotL = saturate(l.z);
    float nDotH = saturate(h.z);
    float vDotH = saturate(dot(v, h));

    if (nDotL > 0.0) {
      float g = geometrySmith(nDotV, nDotL, roughness);
      float gVis = (g * vDotH) / (nDotH * nDotV);
      float fc = pow(1.0 - vDotH, 5.0);

      a += (1.0 - fc) * gVis;
      b += fc * gVis;
    }
  }

  a /= float(SAMPLE_COUNT);
  b /= float(SAMPLE_COUNT);
  return float2(a, b);
}

float4 PSMain(PSInput input) : SV_Target {
  float nDotV = input.uv.x;
  float roughness = input.uv.y;

  float2 result = integrateBRDF(nDotV, roughness);
  return float4(result, 0.0, 1.0);
}