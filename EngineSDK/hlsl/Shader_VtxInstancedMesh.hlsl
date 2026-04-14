
#include "Engine_Shader_Define.hlsli"

// 뷰/프로젝션 행렬 (공통 cbuffer)
float4x4    g_ViewMatrix, g_ProjMatrix;

// 단순 단일 텍스처 (iTextureIndex == 0 일 때 사용)
texture2D           g_DiffuseTexture;
// Texture2DArray (여러 종류 텍스처를 인스턴스마다 다르게 쓸 때)
Texture2DArray      g_DiffuseTextureArray;
bool                g_bUseTextureArray = false;

// ── 정점 (슬롯 0, PER_VERTEX_DATA) + 인스턴스 (슬롯 1, PER_INSTANCE_DATA)
struct VS_IN
{
    // 메시 정점
    float3  vPosition   : POSITION;
    float3  vNormal     : NORMAL;
    float2  vTexcoord   : TEXCOORD0;
    float3  vTangent    : TANGENT;

    // INSTANCE_DATA: WorldMatrix (4 row) + iTextureIndex
    row_major float4x4  matWorld    : WORLD;
    uint                iTexIndex   : TEXCOORD1;
};

struct VS_OUT
{
    float4  vPosition   : SV_POSITION;
    float4  vNormal     : NORMAL;
    float2  vTexcoord   : TEXCOORD0;
    float4  vProjPos    : TEXCOORD2;
    uint    iTexIndex   : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT  Out = (VS_OUT)0;

    float4x4 matWV  = mul(In.matWorld, g_ViewMatrix);
    float4x4 matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal   = normalize(mul(float4(In.vNormal, 0.f), In.matWorld));
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos  = Out.vPosition;
    Out.iTexIndex = In.iTexIndex;

    return Out;
}

struct PS_IN
{
    float4  vPosition   : SV_POSITION;
    float4  vNormal     : NORMAL;
    float2  vTexcoord   : TEXCOORD0;
    float4  vProjPos    : TEXCOORD2;
    uint    iTexIndex   : TEXCOORD3;
};

// 디퍼드 렌더링 GBuffer 출력
struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal  : SV_TARGET1;
    float4 vDepth   : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    float4 vDiffuse;
    if (g_bUseTextureArray)
        vDiffuse = g_DiffuseTextureArray.Sample(LinearSampler, float3(In.vTexcoord, (float)In.iTexIndex));
    else
        vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    if (vDiffuse.a < 0.1f)
        discard;

    Out.vDiffuse = vDiffuse;
    Out.vNormal  = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth   = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

    return Out;
}

technique11 DefaultTechnique
{
    pass Instanced
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader  = compile ps_5_0 PS_MAIN();
    }
}
