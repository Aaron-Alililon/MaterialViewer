struct PixelInputType {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

float4 PSMain(PixelInputType input) : SV_TARGET {
    float3 sunDir = normalize(float3(1, 1, -1));
    float global = 0.1;
    
    float3 nNormal = normalize(input.normal);
    float lambert = max(global, saturate(dot(nNormal, sunDir)));
    float3 diffuse = float3(0.4, 0.5, 1.0) * lambert;
    
    return float4(diffuse, 1);
}