
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4 g_BoneMatrices[512];

vector g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);

texture2D g_BaseTexture;
texture2D g_DarkTexture;
texture2D g_DetailTexture;
texture2D g_GlossTexture;
texture2D g_GlowTexture;
texture2D g_BumpTexture;
texture2D g_NormalTexture;
texture2D g_ParallaxTexture;
texture2D g_Decal0Texture;
texture2D g_Decal1Texture;
texture2D g_Decal2Texture;
texture2D g_Decal3Texture;


float3 g_vMaterialDiffuse = float3(0.3f, 0.3f, 0.3f);
float3 g_vMaterialAmbient = float3(0.3f, 0.3f, 0.3f);
float3 g_vMaterialSpecular = float3(0.f, 0.f, 0.f);
float3 g_vMaterialEmissive = float3(0.f, 0.f, 0.f);
float g_fMaterialGlossiness = 0.f;
float g_fMaterialAlpha = 1.f;

//texture2D	g_TextureArray[12];
float2 g_vTexcoordScale[12];
float g_fTexcoordRotate[12]; //Radian
float2 g_vTexcoordTranslate[12];
float2 g_vTexcoordCenter[12];

vector g_vCamPosition;

uint g_TexFlags = 0;

#define BASE_TEX 1<<0
#define DARK_TEX 1<<1
#define DETAIL_TEX 1<<2
#define GLOSS_TEX 1<<3
#define GLOW_TEX 1<<4
#define BUMP_TEX 1<<5
#define NORMAL_TEX 1<<6
#define PAALLAX_TEX 1<<7
#define DECAL0_TEX 1<<8
#define DECAL1_TEX 1<<9
#define DECAL2_TEX 1<<10
#define DECAL3_TEX 1<<11

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndices : BLENDINDEX;
    float4 vBlendWeights : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vWorldPos : TEXCOORD0;
    float2 vTexcoord[12] : TEXCOORD1;

};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    //matWV = mul(g_WorldMatrix, g_ViewMatrix);
    //CPU에서 루트 본에 미리 WorldMatrix를 곱해서 넘겼을 때
   // matWVP = mul(g_ViewMatrix, g_ProjMatrix);
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

    matrix BoneMatrix = mul(g_BoneMatrices[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_BoneMatrices[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_BoneMatrices[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_BoneMatrices[In.vBlendIndices.w], fWeightW);

    vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
    for (int i = 0; i < 12; ++i)
    {
      
        float2 uv = In.vTexcoord;
        uv -= g_vTexcoordCenter[i];
        uv = uv * g_vTexcoordScale[i];
        uv = float2(uv.x * cos(g_fTexcoordRotate[i]) - uv.y * sin(g_fTexcoordRotate[i]), uv.x * sin(g_fTexcoordRotate[i]) + uv.y * cos(g_fTexcoordRotate[i]));
        uv += g_vTexcoordTranslate[i] + g_vTexcoordCenter[i];
        //uv = saturate(uv);
        Out.vTexcoord[i] = uv;
    }

    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vWorldPos : TEXCOORD0;
    float2 vTexcoord[12] : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

/* PixelShader */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
 
    float4 vBaseColor = g_BaseTexture.Sample(LinearSampler, In.vTexcoord[0]);
    float4 vDarkColor = g_DetailTexture.Sample(LinearSampler, In.vTexcoord[1]);
    float4 vDetailColor = g_DetailTexture.Sample(LinearSampler, In.vTexcoord[2]);
    float4 vGlowColor = g_GlowTexture.Sample(LinearSampler, In.vTexcoord[3]);
    float4 vDecal0Color = g_Decal0Texture.Sample(LinearSampler, In.vTexcoord[8]);
		
    
    float4 vMtrlDiffuse = vBaseColor;
    vMtrlDiffuse.rgb = vMtrlDiffuse.rgb * (g_vMaterialDiffuse);
    vMtrlDiffuse *= vDetailColor;
    vMtrlDiffuse += vGlowColor * vMtrlDiffuse.a;
    vMtrlDiffuse.a *= g_fMaterialAlpha;
	
    float4 vMtrlAmbient = (g_vMaterialAmbient, g_fMaterialAlpha);
    float4 vMtrlSpecualr = (g_vMaterialSpecular, g_fMaterialAlpha);
	
    float4 vShade = saturate(max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f) + (g_vLightAmbient * vMtrlAmbient));
    //float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    //float4 vLook = In.vWorldPos - g_vCamPosition;

    //float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);
	

    Out.vColor = ( /*g_vLightDiffuse **/vMtrlDiffuse) * saturate(vShade) /*+ (g_vLightSpecular * vMtrlSpecualr) * fSpecular*/;

	//데칼 블렌딩
    //Out.vColor.rgb = (1 - vDecal0Color.a) * Out.vColor.rgb + vDecal0Color.a * vDecal0Color.rgb;
	//Detail 텍스처
    if (Out.vColor.a <= 0.05)
        discard;
    return Out;
}


technique11 DefaultTechnique
{
	
    pass DefaultPass
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }


}
