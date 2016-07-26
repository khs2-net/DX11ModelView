struct VSOUT{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
};
SamplerState samLinear : register(s0); // �ǂ������ӂ��ɑ傫�����邩�@(S)�T���v���[�������Ă郌�W�X�^�[
Texture2D texAlbed	  : register(t0);  // (T)�e�N�X�`���������Ă郌�W�X�^�[
Texture2D texNormal  :register(t1);
float4 main(VSOUT inp) : SV_TARGET{
	float4 texcol = texAlbed.Sample(samLinear, inp.uv);
	return float4(texcol.rgb, 1);
}