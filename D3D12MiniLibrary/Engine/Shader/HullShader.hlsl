
// 入力制御点
struct VS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS;
	// TODO: 他のスタッフの変更/追加
};

// 出力制御点
struct HS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS;
};

// 出力パッチ定数データ。
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor; // たとえば、クワド ドメインの [4] になります
    float InsideTessFactor : SV_InsideTessFactor; // たとえば、クワド ドメインの Inside[2] になります
	// TODO: 他のスタッフの変更/追加
};

#define NUM_CONTROL_POINTS 3

// パッチ定数関数
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;
   
    float3 retf;
    float ritf, uitf;
    float3 TestTessFactor = float3(0.5f, 0.5f, 0.5f);
    float TestIncidentFactor = 0.5f;
    ProcessTriTessFactorsAvg(TestTessFactor.xyz, TestIncidentFactor, retf, ritf, uitf);
    Output.EdgeTessFactor[0] = retf.x;
    Output.EdgeTessFactor[1] = retf.y;
    Output.EdgeTessFactor[2] = retf.z;
    Output.InsideTessFactor = ritf;

    return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONTROL_POINT_OUTPUT Output;

	// ここにコードを挿入して出力を計算します
    Output.vPosition = ip[i].vPosition;

    return Output;
}
