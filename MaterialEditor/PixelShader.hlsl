cbuffer PropertiesBuffer : register(b1) {
    float4 albedo;
    float4 sunDirection;
    float globalIllumination;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

float4 PSMain(PixelInputType input) : SV_TARGET {
    float3 nNormal = normalize(input.normal);
    float lambert = max(globalIllumination, saturate(dot(nNormal, normalize(sunDirection.xyz))));
    float3 diffuse = albedo.rgb * lambert;
    
    return float4(diffuse, 1);
}