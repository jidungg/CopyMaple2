
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"


float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_DepthTexture;

int g_NumberTextureStride = 54;

int g_arrDigitWidth[10] ;
int g_arrDigitNumber[10];
int g_DigitCount = 0;

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

    matrix matW = g_WorldMatrix;
    
    float fSizeX = length(matW[0]);
    float fSizeY = length(matW[1]);
    vector vLook = normalize(-matW[2]);
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
/* PixelShader */
PS_OUT PS_DMGCOUNT_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float fTotalPrintingWidth;
    for (int i = 0; i < g_DigitCount; ++i)
    {
        fTotalPrintingWidth += g_arrDigitWidth[g_arrDigitNumber[i]];
    }
    float fPrintingPosition = fTotalPrintingWidth * In.vTexcoord.x;
    
    float fStart = 0;
    float fEnd = fStart;
    int iPrintingNumber = 0;
    for (int i = 0; i < g_DigitCount; ++i)
    {
        fEnd = fStart + g_arrDigitWidth[g_arrDigitNumber[i]];
        
        if (fStart <= fPrintingPosition && fPrintingPosition < fEnd)
        {
            iPrintingNumber = g_arrDigitNumber[i];
            break;
        }
        fStart = fEnd;
    }
    float fCurrentNumberCoord = (fPrintingPosition - fStart) / (fEnd - fStart);
    float fCurrentNumberCoordStart = iPrintingNumber / 10;
    float fCurrentNumberCoordEnd = (iPrintingNumber * g_NumberTextureStride + g_arrDigitWidth[iPrintingNumber]) / (g_NumberTextureStride * 10);
    float fResultCoord = (fCurrentNumberCoordStart - fCurrentNumberCoordEnd) * fCurrentNumberCoord ;
    Out.vColor = g_Texture.Sample(LinearSampler, float2(fResultCoord, In.vTexcoord.y));

    if(Out.vColor.a <= 0.1f)
    {


    }
    Out.vColor = (1, 1, 1, 1);
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
    pass BillboardPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_BILLBOARD_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass DmgCountPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_BILLBOARD_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DMGCOUNT_MAIN();
    }



}
