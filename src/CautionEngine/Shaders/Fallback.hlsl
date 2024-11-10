struct TestConstantBuffer
{
    float4x4 mvp;
    float4 color;
};

ConstantBuffer<TestConstantBuffer> cBuffer;

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

PSInput VSMain(VSInput IN)
{
	PSInput OUT;
	OUT.position = IN.position;
    OUT.color = IN.color;
    return OUT;
}

float4 PSMain(PSInput IN): SV_TARGET
{
	return float4(cBuffer.color.rgb, 1);
}