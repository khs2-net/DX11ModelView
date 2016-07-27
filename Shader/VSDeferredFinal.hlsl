cbuffer ConstantBuffer : register(b0){
	matrix World;
	matrix View;
	matrix Projection;
};
// VertexShaderInput(���j
struct VSIN{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD;
};
// VertexShaderOutput(�o)
struct VSOUT{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
};
VSOUT main(VSIN inp){
	VSOUT outp = (VSOUT)0;
	outp.pos = inp.pos;
	outp.uv = inp.uv;
	return outp;
}