TextureCube skyCube : register(t0);
SamplerState sampleType : register(s0);

struct PixelInputType {
  float4 position : SV_POSITION;
  float3 worldPos : TEXCOORD0;
};

float4 PSMain(PixelInputType input) : SV_TARGET {
  float4 hdr = skyCube.Sample(sampleType, normalize(input.worldPos));
  float4 sdr = hdr / (hdr + 1);
  return sdr;
}