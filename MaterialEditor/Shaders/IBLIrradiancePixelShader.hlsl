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

float4 PSMain(PSInput input) : SV_Target {
  float2 ndc = input.uv * 2.0f - 1.0f;

  float3 normal = normalize(forward + ndc.x * right - ndc.y * up);
  
  float3 up_ = abs(normal.y) < 0.999 ? float3(0, 1, 0) : float3(1, 0, 0);
  float3 tangentX = normalize(cross(up_, normal));
  float3 tangentY = cross(normal, tangentX);

  float3 irradiance = float3(0, 0, 0);
  float sampleCount = 0.0f;

  static const float deltaPhi = 2.0f * PI / 180.0f; // ~2 degree steps
  static const float deltaTheta = 0.5f * PI / 64.0f;

  for (float phi = 0.0; phi < 2.0 * PI; phi += deltaPhi) {
    for (float theta = 0.0; theta < 0.5 * PI; theta += deltaTheta) {
      // spherical -> tangent-space direction
      float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
      // tangent space -> world space (around the normal)
      float3 sampleDir = tangentSample.x * tangentX + tangentSample.y * tangentY + tangentSample.z * normal;

      irradiance += envMap.Sample(linearSampler, sampleDir).rgb * cos(theta) * sin(theta);
      sampleCount += 1.0;
    }
  }

  irradiance = PI * irradiance / sampleCount;

  return float4(irradiance, 1.0);
}