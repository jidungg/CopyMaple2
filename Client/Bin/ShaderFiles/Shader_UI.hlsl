

#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;
float4 g_BorderSize;//상 하 좌 우 순
float4 g_MinMax; //MinX, MinY, MaxX, MaxY
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
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
    float2			vRatio : TEXCOORD1;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

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


technique11 DefaultTechnique
{
	pass DefaultPass
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}


}
