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

// �萔�o�b�t�@�i�I�u�W�F�N�g�p�j
cbuffer ObjectBuffer : register(b0)
{
	float4 vertexColor; // ���_�J���[
	matrix matrixTex;   // UV���W�ړ��s��
	matrix matrixProj;  // �v���W�F�N�V�����ϊ��s��
	matrix matrixWorld; // ���[���h�ϊ��s��
}

// �萔�o�b�t�@�i�g�̉e���p�j
cbuffer WaveBuffer : register(b1)
{
	float time;         // ����
	float waveAmplitude; // �g�̐U��
	float waveFrequency; // �g�̎��g��
	float padding;
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
