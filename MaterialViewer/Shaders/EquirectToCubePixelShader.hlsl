Texture2D equirectMap : register(t0);
SamplerState linearSampler : register(s0);

cbuffer FaceDirectionBuffer : register(b0) {
  float3 forward;
  float _pad0;
  float3 up;
  float _pad1;
  float3 right;
  float _pad2;
};

struct PSInput {
  float4 position : SV_Position;
  float2 uv : TEXCOORD0;
};

static const float PI = 3.14159265f;

float2 dirToEquirectUV(float3 dir) {
  float2 uv;
  uv.x = atan2(dir.z, dir.x) / (2.0f * PI) + 0.5f;
  uv.y = acos(clamp(dir.y, -1.0f, 1.0f)) / PI;
  return uv;
}

float4 PSMain(PSInput input) : SV_Target {
  float2 ndc = input.uv * 2.0f - 1.0f;

  float3 dir = normalize(forward + ndc.x * right - ndc.y * up);

  float2 equirectUV = dirToEquirectUV(dir);

  return equirectMap.Sample(linearSampler, equirectUV);
}