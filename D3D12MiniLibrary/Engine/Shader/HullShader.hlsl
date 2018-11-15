
// ���͐���_
struct VS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS;
	// TODO: ���̃X�^�b�t�̕ύX/�ǉ�
};

// �o�͐���_
struct HS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS;
};

// �o�̓p�b�`�萔�f�[�^�B
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor; // ���Ƃ��΁A�N���h �h���C���� [4] �ɂȂ�܂�
    float InsideTessFactor : SV_InsideTessFactor; // ���Ƃ��΁A�N���h �h���C���� Inside[2] �ɂȂ�܂�
	// TODO: ���̃X�^�b�t�̕ύX/�ǉ�
};

#define NUM_CONTROL_POINTS 3

// �p�b�`�萔�֐�
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

	// �����ɃR�[�h��}�����ďo�͂��v�Z���܂�
    Output.vPosition = ip[i].vPosition;

    return Output;
}
