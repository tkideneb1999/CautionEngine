struct VSInput
{
	float4 position: POSITION;
	float4 color: COLOR;
};

struct PSInput
{
	float4 position: SV_POSITION;
	float4 color : COLOR;
};

void VSMain(VSInput IN)
{
	PSInput OUT;
	OUT.position = IN.position;
	OUT.color = IN.color;
}

float4 PSMain(PSInput IN): SV_TARGET0
{
	return float4(IN.color.rgb, 1);
}