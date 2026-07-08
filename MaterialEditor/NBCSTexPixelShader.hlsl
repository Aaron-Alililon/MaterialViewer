struct PixelInputType {
    float4 position : SV_POSITION;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float sTangent : TEXCOORD0;
    float sBinormal : TEXCOORD1;
};

struct PSOutput {
    float4 tangentOut : SV_TARGET0;
    float4 binormalOut : SV_TARGET1;
};

PSOutput PSMain(PixelInputType input) {
    PSOutput output;
    
    output.tangentOut = float4(input.tangent, input.sTangent);
    output.binormalOut = float4(input.binormal, input.sBinormal);
    
    return output;
}