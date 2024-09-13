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

float4 PSMain(PSInput IN): SV_TARGET0
{
	return float4(IN.color.rgb, 1);
}