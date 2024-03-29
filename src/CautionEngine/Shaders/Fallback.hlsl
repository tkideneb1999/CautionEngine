cbuffer testCBuffer: register(b0)
{
    float testVal;
    float4x4 testMtx;
    float3 testfloat3;
    int data;
}

struct TestCBuffer2
{
    float another;
    float2 testFloat2;
};

//ConstantBuffer<TestCBuffer2> testBuffer2;

struct VSInput
{
	float test: TEXCOORD0;
	float4 position: POSITION;
	float4 color: COLOR;
    float4 color2 : COLOR2;
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
    OUT.color = IN.color + testVal + float4(testfloat3, 0);
    return OUT;
}

float4 PSMain(PSInput IN): SV_TARGET0
{
    float4 col = IN.color * testVal;
	return float4(IN.color.rgb, 1);
}