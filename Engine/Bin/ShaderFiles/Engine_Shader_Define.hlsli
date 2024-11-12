

/* D3D11_SAMPLER_DESC */
sampler			LinearSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

sampler			PointSampler = sampler_state
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = wrap;
	AddressV = wrap;
};


RasterizerState		RS_Default
{
	FillMode = SOLID;
	CullMode = BACK;
	FrontCounterClockwise = false;
};

RasterizerState		RS_Cull_Front
{
	FillMode = SOLID;
	CullMode = Front;
	FrontCounterClockwise = false;
};

RasterizerState		RS_Wireframe
{
	FillMode = Wireframe;
	CullMode = BACK;
	FrontCounterClockwise = false;
};


DepthStencilState		DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState		DSS_None
{
	DepthEnable = false;
	DepthWriteMask = zero;
};


BlendState		BS_Default
{
	BlendEnable[0] = false;


};

BlendState		BS_AlphaBlend
{
	BlendEnable[0] = true;

	// sourColor.rgb * SourColor.a + DestColor.rgb * (1.f - SourColor.a)
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = Add;	
};

BlendState		BS_OneBlend
{

};


