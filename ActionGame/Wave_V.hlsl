//// ���_�V�F�[�_�[
//
//// ���_�̃f�[�^��\���\���́i�󂯎��p�j
//struct VS_IN
//{
//    float4 pos : POSITION;
//    float4 col : COLOR0;
//    float2 tex : TEX;   //UV���W
//};
//
//// ���_�̃f�[�^��\���\���́i���M�p�j 
//struct VS_OUT
//{
//    float4 pos : SV_POSITION;
//    float4 col : COLOR0;
//    float2 tex : TEXCOORD;  //UV���W
//};
//
//// �O���[�o���ϐ���錾
//// �萔�o�b�t�@�󂯎��p
//cbuffer ConstantBuffer : register(b0) {
//    // ���_�J���[
//    float4 vertexColor;
//    // UV���W�ړ��s��
//    matrix matrixTex;
//    // �v���W�F�N�V�����ϊ��s��
//    matrix matrixProj;
//    // ���[���h�ϊ��s��
//    matrix matrixWorld;
//    // ���Ԃ̃p�����[�^
//    float time;
//    // �g�̃p�����[�^
//    float waveAmplitude; // �g�̐U��
//    float waveFrequency; // �g�̎��g��
//}
//
//// ���_�V�F�[�_�[�̃G���g���|�C���g
//VS_OUT vs_main(VS_IN input)
//{
//    VS_OUT output;
//
//    // �g�̍������v�Z�i���Ԃƈʒu�Ɋ�Â���sin�g�𐶐��j
//    float waveHeight = waveAmplitude * sin(input.pos.x * waveFrequency + time);
//
//    // �g�̍�����Z�������ɉ��Z
//    input.pos.z += waveHeight;
//
//    // ���[���h�ϊ��s��𒸓_���W�Ɋ|���āA�ړ��A��]�A�g��k������
//    output.pos = mul(input.pos, matrixWorld);
//
//    // ���_���W�ɓ��e�s����|���āA���Ֆʏ�̍��W�ɂ���
//    output.pos = mul(output.pos, matrixProj);
//
//    // UV���W���ړ�������
//    float4 uv;
//    uv.xy = input.tex;  // �s��|���Z�̂���float4�^
//    uv.z = 0.0f;
//    uv.w = 1.0f;
//    uv = mul(uv, matrixTex);    // UV���W�ƈړ��s����|���Z
//    output.tex = uv.xy;         // �|���Z�̌��ʂ𑗐M�p�ϐ��ɃZ�b�g
//
//    // ���_�J���[��ݒ�
//    output.col = input.col * vertexColor;
//
//    return output;
//}

// ���_�̃f�[�^��\���\���́i�󂯎��p�j
struct VS_IN
{
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;  // UV���W
};

// ���_�̃f�[�^��\���\���́i���M�p�j 
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;  // UV���W
};

// �萔�o�b�t�@�i�O���[�o���ϐ��Ƃ��Đ錾�j
cbuffer ConstantBuffer : register(b0)
{
	float4 vertexColor; // ���_�J���[
	matrix matrixTex;   // UV���W�ړ��s��
	matrix matrixProj;  // �v���W�F�N�V�����ϊ��s��
	matrix matrixWorld; // ���[���h�ϊ��s��
	float time;         // ����
	float waveAmplitude; // �g�̐U��
	float waveFrequency; // �g�̎��g��
}

// ���_�V�F�[�_�[�̃G���g���|�C���g
VS_OUT vs_main(VS_IN input)
{
	VS_OUT output;

	// �g�̍������v�Z�iX���̈ʒu�Ǝ��Ԃ�sin�g�𐶐��j
	float waveHeight = waveAmplitude * sin(input.pos.x * waveFrequency + time);

	// �l�𐧌�
	waveHeight = clamp(waveHeight, -0.1, 0.1);

	// �ʒu��ύX�iY���ɔg��K�p�j
	float4 modifiedPos = input.pos;
	modifiedPos.y += waveHeight;

	// ���[���h�ϊ��s���K�p�i�ړ��E��]�E�X�P�[���j
	modifiedPos = mul(modifiedPos, matrixWorld);

	// ���e�ϊ��s���K�p�i��ʍ��W�֕ϊ��j
	output.pos = mul(modifiedPos, matrixProj);

	// UV���W��ϊ�
	output.tex = mul(float4(input.tex, 0.0f, 1.0f), matrixTex).xy;

	// ���_�J���[��K�p
	output.col = input.col * vertexColor;


	return output;
}
