
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"


float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_DepthTexture;



struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0; 
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

VS_OUT VS_BILLBOARD_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    float4x4 matW = g_WorldMatrix;
    
    float fSizeX = length(matW[0]);
    float fSizeY = length(matW[1]);
    vector vLook = normalize(-(transpose(g_ViewMatrix)[2]));
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * fSizeX;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * fSizeY;

    matW[0].xyz = vRight;
    matW[1].xyz = vUp;
    matW[2] = vLook;
    matrix matWV, matWVP;
    matWV = mul(matW, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

/* PixelShader */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    //Out.vColor.gb = 0.f;
    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

}
