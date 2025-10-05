struct TestConstantBuffer
{
    float4x4 mvp;
};

ConstantBuffer<TestConstantBuffer> cBuffer : register(b0);

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
    OUT.position = mul(cBuffer.mvp, IN.position);
    OUT.color = IN.color;
    return OUT;
}

float4 PSMain(PSInput IN): SV_TARGET
{
    return float4(IN.color.rgb, 1);
}