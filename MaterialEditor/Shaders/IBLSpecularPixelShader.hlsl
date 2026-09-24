TextureCube envMap : register(t0);
SamplerState linearSampler : register(s0);

cbuffer FaceDirectionBuffer : register(b0) {
  float3 forward;   float _pad0;
  float3 up;        float _pad1;
  float3 right;
  float roughness;
};

struct PSInput {
  float4 position : SV_Position;
  float2 uv : TEXCOORD0;
};

static const float PI = 3.14159265f;
static const uint SAMPLE_COUNT = 16384u;

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

float4 PSMain(PSInput input) : SV_Target {
  float2 ndc = input.uv * 2.0f - 1.0f;

  float3 n = normalize(forward + ndc.x * right - ndc.y * up);
  float3 r = n;
  float3 v = r;

  float3 prefilteredColor = float3(0, 0, 0);
  float totalWeight = 0.0;

  for (uint i = 0u; i < SAMPLE_COUNT; i++) {
    float2 xi = hammersley(i, SAMPLE_COUNT);
    float3 h = importanceSampleGGX(xi, n, roughness);
    float3 l = normalize(2.0 * dot(v, h) * h - v);

    float nDotL = saturate(dot(n, l));
    if (nDotL > 0.0) {
      prefilteredColor += envMap.Sample(linearSampler, l).rgb * nDotL;
      totalWeight += nDotL;
    }
  }

  prefilteredColor = totalWeight > 0.0 ? prefilteredColor / totalWeight : prefilteredColor;

  return float4(prefilteredColor, 1.0);
}