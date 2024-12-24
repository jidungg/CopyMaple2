

#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;
float4 g_BorderSize;//상 하 좌 우 순
float4 g_MinMax; //MinX, MinY, MaxX, MaxY

int g_NumberTextureStride = 54;
int g_arrDigitWidth[10];
int g_arrDigitNumber[10];
int g_DigitCount = 0;

float g_fVerticalRatio = 1.0f;

float g_fDarkRatio = 0.5f;
struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;	
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
    float2			vRatio : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;
    float fXSize = (float) g_MinMax[2] - (float) g_MinMax[0];
    float fYSize = (float) g_MinMax[3] - (float) g_MinMax[1];
    uint width, height;
    g_Texture.GetDimensions(width, height);

    Out.vRatio.x = (float) fXSize / (float) width;
    Out.vRatio.y = (float) fYSize / (float) height;

	return Out;
}

struct VS_NOBORDER_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_NOBORDER_OUT VS_NOBORDER_MAIN(VS_IN In)
{
    VS_NOBORDER_OUT Out = (VS_NOBORDER_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;


    return Out;
}
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
    float2			vRatio : TEXCOORD1;
};
struct PS_NOBORDER_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;

};

/* PixelShader */
PS_OUT PS_DMGCOUNT_MAIN(PS_NOBORDER_IN In)
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

    float fCurrentNumberCoord = float(fPrintingPosition - fStart) / float(fEnd - fStart);
    float fCurrentNumberCoordStart = float(iPrintingNumber) / float(10);
    float fCurrentNumberCoordEnd = float(iPrintingNumber * g_NumberTextureStride + g_arrDigitWidth[iPrintingNumber]) / float(g_NumberTextureStride * 10);
    float fResultCoord = (fCurrentNumberCoordEnd - fCurrentNumberCoordStart) * fCurrentNumberCoord + fCurrentNumberCoordStart;
    Out.vColor = g_Texture.Sample(LinearSampler, float2(fResultCoord, In.vTexcoord.y));
    //Out.vColor.r = (float) iPrintingNumber / (float) 10;
    //Out.vColor.g = fCurrentNumberCoordStart;
    //Out.vColor.b = fCurrentNumberCoordEnd;
    //Out.vColor.a = 1.0;

    if (Out.vColor.a <= 0.05f)
    {
        discard;

    }
    return Out;
}
/* PixelShader */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	
    float fXSize = g_MinMax[2] - g_MinMax[0];
    float fYSize = g_MinMax[3] - g_MinMax[1];
	//좌 테두리
    if (g_MinMax[0] + g_BorderSize[2] >= In.vPosition.x)
    {
        In.vTexcoord.x *= In.vRatio.x;
    }
    //우 테두리
    else if (g_MinMax[2] - g_BorderSize[3] <= In.vPosition.x)
    {
        In.vTexcoord.x = 1 - (1 - In.vTexcoord.x) * In.vRatio.x;
    }
    //가운데
    else
    {
        float fLeftBorderCoordLen = g_BorderSize[2] / fXSize * In.vRatio.x;
        float fRightBorderCoordLen = g_BorderSize[3] / fXSize * In.vRatio.x;
        float fCenterCoordLen = 1 - fLeftBorderCoordLen - fRightBorderCoordLen;
        In.vTexcoord.x = In.vTexcoord.x *fCenterCoordLen + fLeftBorderCoordLen;
    }
 //   //상 테두리
    if (g_MinMax[1] + g_BorderSize[0] >= In.vPosition.y)
    {
        In.vTexcoord.y *= In.vRatio.y;
    }
    //하 테두리
    else if (g_MinMax[3] - g_BorderSize[1] <= In.vPosition.y)
    {   
        In.vTexcoord.y = 1 - (1 - In.vTexcoord.y) * In.vRatio.y;
    }
    else
    {
        float fTopBorderCoordLen = g_BorderSize[0] / fYSize * In.vRatio.y;
        float fBotBorderCoordLen = g_BorderSize[1] / fYSize * In.vRatio.y;
        float fCenterCoordLen = 1 - fTopBorderCoordLen - fBotBorderCoordLen;
        In.vTexcoord.y = In.vTexcoord.y * fCenterCoordLen + fTopBorderCoordLen;
    }
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);


	return Out;
}

/* PixelShader */
PS_OUT PS_NOBORDER_MAIN(PS_NOBORDER_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    if (Out.vColor.a <= 0)
        discard;
    return Out;
}

PS_OUT PS_VERTICALFILL_MAIN(PS_NOBORDER_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (In.vTexcoord.y < g_fVerticalRatio)
    {
        discard;
    }
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    return Out;
}

PS_OUT PS_VERTICALDARK_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float fXSize = g_MinMax[2] - g_MinMax[0];
    float fYSize = g_MinMax[3] - g_MinMax[1];
	//좌 테두리
    if (g_MinMax[0] + g_BorderSize[2] >= In.vPosition.x)
    {
        In.vTexcoord.x *= In.vRatio.x;
    }
    //우 테두리
    else if (g_MinMax[2] - g_BorderSize[3] <= In.vPosition.x)
    {
        In.vTexcoord.x = 1 - (1 - In.vTexcoord.x) * In.vRatio.x;
    }
    //가운데
    else
    {
        float fLeftBorderCoordLen = g_BorderSize[2] / fXSize * In.vRatio.x;
        float fRightBorderCoordLen = g_BorderSize[3] / fXSize * In.vRatio.x;
        float fCenterCoordLen = 1 - fLeftBorderCoordLen - fRightBorderCoordLen;
        In.vTexcoord.x = In.vTexcoord.x * fCenterCoordLen + fLeftBorderCoordLen;
    }
 //   //상 테두리
    if (g_MinMax[1] + g_BorderSize[0] >= In.vPosition.y)
    {
        In.vTexcoord.y *= In.vRatio.y;
    }
    //하 테두리
    else if (g_MinMax[3] - g_BorderSize[1] <= In.vPosition.y)
    {
        In.vTexcoord.y = 1 - (1 - In.vTexcoord.y) * In.vRatio.y;
    }
    else
    {
        float fTopBorderCoordLen = g_BorderSize[0] / fYSize * In.vRatio.y;
        float fBotBorderCoordLen = g_BorderSize[1] / fYSize * In.vRatio.y;
        float fCenterCoordLen = 1 - fTopBorderCoordLen - fBotBorderCoordLen;
        In.vTexcoord.y = In.vTexcoord.y * fCenterCoordLen + fTopBorderCoordLen;
    }
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);


    if (In.vTexcoord.y > g_fVerticalRatio)
    {
        Out.vColor.rgb = Out.vColor.rgb * g_fDarkRatio;
    }
    else
        Out.vColor.a = 0.f;

    return Out;
}
technique11 DefaultTechnique
{
	pass DefaultPass
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
    pass DamgCntPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_NOBORDER_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DMGCOUNT_MAIN();
    }
    pass BackGroundPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_NOBORDER_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_NOBORDER_MAIN();
    }
    pass VerticalFillPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_NOBORDER_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_VERTICALFILL_MAIN();
    }
    pass VerticalDarkPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_VERTICALDARK_MAIN();
    }
    pass ModelPadPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_NOBORDER_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_NOBORDER_MAIN();
    }
}
