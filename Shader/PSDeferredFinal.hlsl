struct VSOUT{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
};
SamplerState samLinear : register(s0); // �ǂ������ӂ��ɑ傫�����邩�@(S)�T���v���[�������Ă郌�W�X�^�[
Texture2D texAlbed	   : register(t0);  // (T)�e�N�X�`���������Ă郌�W�X�^�[
Texture2D texNormal    : register(t1);
Texture2D texDepth     : register(t2);
Texture2D texDiffuse   : register(t3);

float4 main(VSOUT inp) : SV_TARGET{
	float4 final = 0;
	float4 albed   = texAlbed.Sample(samLinear, inp.uv);
	float4 normal  = texNormal.Sample(samLinear, inp.uv);
	float depth   = texDepth.Sample(samLinear, inp.uv);
	float4 diffuse = texDiffuse.Sample(samLinear, inp.uv);

	final = diffuse;

	return final;
}