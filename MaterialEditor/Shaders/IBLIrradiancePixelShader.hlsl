TextureCube envMap : register(t0);
SamplerState linearSampler : register(s0);

cbuffer FaceDirectionBuffer : register(b0) {
  float3 forward;   float _pad0;
  float3 up;        float _pad1;
  float3 right;     float _pad2;
};

struct PSInput {
  float4 position : SV_Position;
  float2 uv : TEXCOORD0;
};

static const float PI = 3.14159265f;

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

float3 cosineSampleHemisphere(float2 xi) {
  float phi = 2.0 * PI * xi.x;
  float cosTheta = sqrt(1.0 - xi.y);
  float sinTheta = sqrt(xi.y);

  return float3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
}

float4 PSMain(PSInput input) : SV_Target {
  float2 ndc = input.uv * 2.0f - 1.0f;
  float3 normal = normalize(forward + ndc.x * right - ndc.y * up);

  float3 up_ = abs(normal.y) < 0.999 ? float3(0, 1, 0) : float3(1, 0, 0);
  float3 tangentX = normalize(cross(up_, normal));
  float3 tangentY = cross(normal, tangentX);

  static const uint SAMPLE_COUNT = 65536u;
  static const float ENV_RESOLUTION = 1024.0f;

  float3 irradiance = float3(0, 0, 0);

  for (uint i = 0u; i < SAMPLE_COUNT; i++) {
    float2 xi = hammersley(i, SAMPLE_COUNT);
    float3 tangentSample = cosineSampleHemisphere(xi);
    float3 sampleDir = tangentSample.x * tangentX + tangentSample.y * tangentY + tangentSample.z * normal;
    
    float pdf = tangentSample.z / PI;
    float sampleSolidAngle = 1.0 / (SAMPLE_COUNT * pdf);
    float texelSolidAngle = 4.0 * PI / (6.0 * ENV_RESOLUTION * ENV_RESOLUTION);
    float mipLevel = max(0.5 * log2(sampleSolidAngle / texelSolidAngle), 0.0);

    irradiance += envMap.SampleLevel(linearSampler, sampleDir, mipLevel).rgb;
  }

  irradiance /= float(SAMPLE_COUNT);
  return float4(irradiance, 1.0);
}