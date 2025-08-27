
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

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

    matrix BoneMatrix = mul(g_BoneMatrices[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_BoneMatrices[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_BoneMatrices[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_BoneMatrices[In.vBlendIndices.w], In.vBlendWeights.w);

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

    
//    float4 vShade = saturate(max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f));

//    float4 vMaterial;
//    vMaterial.rgb = g_vMaterialDiffuse * vShade.rgb;
//    vMaterial.a = g_fMaterialAlpha;
//    vMaterial = saturate(vMaterial);
//    float4 vColor = vMaterial;
//    if (g_TexFlags & BASE_TEX)
//    {
//        In.vTexcoord[0] = saturate(In.vTexcoord[0]);
//        float4 vBaseColor = g_BaseTexture.Sample(LinearSampler, In.vTexcoord[0]);
//        vColor *= vBaseColor;
//    }
   
//    //EMISSIVE
//    vColor.rgb += g_vMaterialEmissive;
//    if (g_TexFlags & GLOW_TEX)
//    {
//        //빛에는 영향받지 않음.
//        //
//        In.vTexcoord[4] = saturate(In.vTexcoord[4]);
//        float4 vGlowColor = g_GlowTexture.Sample(LinearSampler, In.vTexcoord[4]);
//        vColor.rgb += vGlowColor;
//    }
//    //AMBIENT
//    vColor.rgb += (g_vMaterialAmbient / 5) * g_vLightAmbient.rgb;
    
//    //DECAL
//    if (g_TexFlags & DECAL0_TEX)
//    {
//        In.vTexcoord[8] = saturate(In.vTexcoord[8]);
//        float4 vDecal0Color = g_Decal0Texture.Sample(LinearSampler, In.vTexcoord[8]);
//        vColor.rgb = vColor.rgb * (1 - vDecal0Color.a) + vDecal0Color.a * vDecal0Color.rgb;
//    }
//        //빛 * 머티리얼 디퓨즈
    
//    if (Out.vColor.a <= 0.05)
//        discard;
//    return Out;
    //=======================================================================
    
    // Sample textures

     //Combine Base and Dark Texture
    float4 vShade = saturate(max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f));
    float4 vMaterialColor;
    vMaterialColor.rgb = g_vMaterialDiffuse * vShade.rgb;
    vMaterialColor.rgb += g_vMaterialAmbient * g_vLightAmbient.rgb;
    vMaterialColor.rgb += g_vMaterialEmissive;
    vMaterialColor.a = g_fMaterialAlpha;
    vMaterialColor = saturate(vMaterialColor);
    
    float4 vFinalColor;
    vFinalColor = vMaterialColor;
    
    if (g_TexFlags & BASE_TEX)
    {
        In.vTexcoord[0] = saturate(In.vTexcoord[0]);
        float4 vBaseColor = g_BaseTexture.Sample(LinearSampler, In.vTexcoord[0]);
        vFinalColor *= vBaseColor;
    }
    //if (g_TexFlags & DARK_TEX)
    //{
    //    In.vTexcoord[1] = saturate(In.vTexcoord[1]);
    //    float4 vDarkColor = g_DarkTexture.Sample(LinearSampler, In.vTexcoord[1]);
    //    vFinalColor *= vDarkColor;
    //}
        
    if (g_TexFlags & GLOW_TEX)
    {
        In.vTexcoord[4] = saturate(In.vTexcoord[4]);
        float4 vGlowColor = g_GlowTexture.Sample(LinearSampler, In.vTexcoord[4]);
        vFinalColor.rgb += vGlowColor.rgb;
    }
    
    if (g_TexFlags & DECAL0_TEX)
    {
        In.vTexcoord[8] = saturate(In.vTexcoord[8]);
        float4 vDecalColor = g_Decal0Texture.Sample(LinearSampler, In.vTexcoord[8]);
        
        vFinalColor.rgb = vFinalColor.rgb * (1 - vDecalColor.a) + vDecalColor.rgb * vDecalColor.a;
    }
    Out.vColor = vFinalColor;
    return Out;
 }


technique11 DefaultTechnique
{
	
    pass DefaultPass
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        //SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }


}
