

#include <Windows.h>
#include <xnamath.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <./Device/WindowDevice.h>
#include <./Device/DirectXDevice.h>
#include <./Vertex.h>
#include <./Resource/Texture.h>


// Shader�ɑ���J�������
struct ConstantBuffer{
	XMMATRIX mWorld;		//���[���h�ϊ��s��
	XMMATRIX mView;			//�r���[�ϊ��s��
	XMMATRIX mProjection;	//�������e�ϊ��s��
};

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	WindowDevice &window = WindowDevice::getInstance();
	DX11Device &device = DX11Device::getInstance();
	window.Init(hInstance, nCmdShow, TEXT("test window"), 1280, 720, true);
	device.Init(window.getHandle(), window.getWidth(), window.getHeight(), window.getWindowMode());

	// ���_�̐���
	Vertex4UV vertex[4] = {
		{  500.f,  500.f, 1.0f,0.0f, 1.0f, 0.0f },
		{ -500.f,  500.f, 1.0f,0.0f, 0.0f, 0.0f },
		{  500.f, -500.f, 1.0f,0.0f, 1.0f, 1.0f },
		{ -500.f, -500.f, 1.0f,0.0f, 0.0f, 1.0f },
	};

	// VetexBuffer�̊i�[���錾
	ID3D11Buffer *vertexbuffer = nullptr; 

	D3D11_BUFFER_DESC bd; // �������@(�o�b�t�@�[ ���\�[�X)
	ZeroMemory(&bd, sizeof(bd)); // ���g���[���N���A
	// Buffer�̐������@�̊i�[
	bd.Usage = D3D11_USAGE_DYNAMIC; // �o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@������
	bd.ByteWidth = sizeof(vertex);  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �Ȃ�̃o�b�t�@�ł����H
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU����͏������݂̂ݍs���܂�
	// Buffer�̐���
	device.getDevice()->CreateBuffer(&bd, nullptr, &vertexbuffer);

	//�@���_�����i�[���Ă���
	D3D11_MAPPED_SUBRESOURCE ms; // Buffer���i�[����ׂɂƂ肠�������b�N�������Ȃ��Ƃ����Ȃ��B�ǂ��܂Ń��b�N��������H�T�u���\�[�X �f�[�^�ɃA�N�Z�X�ł���悤�ɂ���
	device.getContext()->Map(vertexbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // �A�N�Z�X��ms
	memcpy(ms.pData, vertex, sizeof(vertex));// pData = vetex�R�s�[�@��������
	device.getContext()->Unmap(vertexbuffer, NULL); // ���b�N����

	// �p�C�v���C���ɒ��_�o�b�t�@�̃Z�b�g
	UINT stride = sizeof(Vertex4UV); // ���_�̃T�C�Y
	UINT offset = 0;			   // ����̒���
	device.getContext()->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);

	// �V�F�[�_�[�̐���
	ID3D11VertexShader *vs_buf = nullptr;  // shader��buffer�@�R���p�C�������V�F�[�_�[�̊i�[�� 
	ID3D11PixelShader  *ps_buf = nullptr;  // shader��buffer�@�R���p�C�������V�F�[�_�[�̊i�[��


	// 
	ID3D10Blob *vsblob, *psblob; // �C�Ӓ��̃f�[�^��Ԃ��ۂɎg�p
	// �t�@�C�������ɃG�t�F�N�g���R���p�C��
	D3DX11CompileFromFile(TEXT("./Shader/VS.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
	D3DX11CompileFromFile(TEXT("./Shader/PS.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob, 0, 0);
	// blob��_buf�Ɋi�[
	device.getDevice()->CreateVertexShader(vsblob->GetBufferPointer(), vsblob->GetBufferSize(), nullptr ,&vs_buf); // �R���p�C���ς݃V�F�[�_�[����A���_�V�F�[�_�[ �I�u�W�F�N�g���쐬
	device.getDevice()->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // �s�N�Z�� �V�F�[�_�[���쐬
	// Shager�̐ݒ�
	device.getContext()->VSSetShader(vs_buf, nullptr, 0); // ���_�V�F�[�_�[���f�o�C�X�ɐݒ�
	device.getContext()->PSSetShader(ps_buf, nullptr, 0); // �s�N�Z�� �V�F�[�_�[���f�o�C�X�ɐݒ�

	//�@�C���v�b�g���C�A�E�g���g�����߂ɕK�v�Ȃ��� 
	D3D11_INPUT_ELEMENT_DESC element[] = { // ���̓A�Z���u���[ �X�e�[�W�̒P��̗v�f( HLSL �Z�}���e�B�N�X,�v�f�̃Z�}���e�B�N�X �C���f�b�N�X,�v�f�f�[�^�̃f�[�^�^,���̓A�Z���u���[�����ʂ��鐮���l,�e�v�f�Ԃ̃I�t�Z�b�g (�o�C�g�P��),�P��̓��̓X���b�g�̓��̓f�[�^ �N���X������,�C���X�^���X�P�ʂ̓����f�[�^���g�p���ĕ`�悷��C���X�^���X�̐�)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },// �ʒu���
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 } // UV��� 
	};
	ID3D11InputLayout *inputlayout; // ���̓A�Z���u���[ �X�e�[�W�̓��̓f�[�^�ɃA�N�Z�X
	device.getDevice()->CreateInputLayout(element, ARRAYSIZE(element), vsblob->GetBufferPointer(), vsblob->GetBufferSize(), &inputlayout); // �i�[(���̓A�Z���u���[ �X�e�[�W�Ŏg�p�������̓o�b�t�@�[ �f�[�^)
	//�C���v�b�g���C�A�E�g�̐ݒ�
	device.getContext()->IASetInputLayout(inputlayout); // ���̓A�Z���u���[ �X�e�[�W�ɓ��̓��C�A�E�g �I�u�W�F�N�g���o�C���h
	// ���X���C�U�̐���
	ID3D11RasterizerState* rasterizer = nullptr; // ���X�^���C�U�[ �X�e�[�g�ɃA�N�Z�X
	D3D11_RASTERIZER_DESC rasterizerDesc = {
		D3D11_FILL_SOLID, // ���C���[�t���[�� (�����_�����O���Ɏg�p����`�惂�[�h������)
		D3D11_CULL_FRONT, // ���ʃ|���S�����J�����O(�w��̕����������Ă���O�p�`���`�悳��Ȃ����Ƃ�����)
		FALSE,			  // �O�p�`���O�������������������肷��
		0,				  // �w��̃s�N�Z���ɉ��Z����[�x�l
		0.0f,             // �s�N�Z���̍ő�[�x�o�C�A�X
		FALSE,			  // �w��̃s�N�Z���̃X���[�v�ɑ΂���X�J��
		FALSE,			  // �����Ɋ�Â��ăN���b�s���O��L��
		FALSE,            // �V�U�[��`�J�����O��L��
		FALSE,			  // �}���`�T���v�����O�̃A���`�G�C���A�V���O��L��
		FALSE			  //�@���̃A���`�G�C���A�V���O��L��
	};
	device.getDevice()->CreateRasterizerState(&rasterizerDesc, &rasterizer); // ���X�^���C�U�[�̐����i���X�^���C�U�[ �X�e�[�W�ɓ�����w�����郉�X�^���C�U�[ �X�e�[�g �I�u�W�F�N�g�j
	device.getContext()->RSSetState(rasterizer); // ���X�^���C�U�[�ݒ�(�p�C�v���C���̃��X�^���C�U�[ �X�e�[�W�̃��X�^���C�U�[ �X�e�[�g��ݒ�)

	device.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // ���_�̕��ѕ��̐ݒ�(�v���~�e�B�u �^�C�v����уf�[�^�̏����Ɋւ�������o�C���h)

	// Shader�ɑ���s��̐���
	ConstantBuffer mtx;

	XMVECTOR hEye = XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f);
	XMVECTOR hAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR hUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mtx.mView = XMMatrixLookAtLH(hEye, hAt, hUp);
	mtx.mWorld = XMMatrixIdentity();
	mtx.mProjection = XMMatrixOrthographicLH(1280, 720, 1, 5000);

	// constant�o�b�t�@����
	ID3D11Buffer *constantbuffer = nullptr;
	ZeroMemory(&bd, sizeof(bd)); // ���g���N���A
	// Buffer�̐������@�̊i�[
	bd.ByteWidth = sizeof(ConstantBuffer); // size�̎w��
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // �Ȃ�̃o�b�t�@�ł����H
	bd.CPUAccessFlags = 0; // CPU����͏������݂̂ݍs���܂�
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	device.getDevice()->CreateBuffer(&bd, NULL, &constantbuffer); // �o�b�t�@�̐���
	// Buffer�ɍX�V�������� 
	device.getContext()->UpdateSubresource(constantbuffer, 0, NULL, &mtx, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	// Buffer���p�C�v���C���ɃZ�b�g
	device.getContext()->VSSetConstantBuffers(0, 1, &constantbuffer); // ���_�V�F�[�_�[�̃p�C�v���C�� �X�e�[�W�Ŏg�p�����萔�o�b�t�@�[��ݒ�

	// �e�N�X�`���̓ǂݍ���
	Texture2D tex,tex2;
	tex.LoadFile("./Resource/Lenna.png");
	tex2.LoadFile("./Resource/lenna_normal.png");
	ID3D11ShaderResourceView *srv[] = {
		tex.getSRV(),
		tex2.getSRV()
	};
	device.getContext()->PSSetShaderResources(0, 2, srv); // �s�N�Z�� �V�F�[�_�[ �X�e�[�W�ɃV�F�[�_�[ ���\�[�X�̔z����o�C���h


	int ret = 0;
	while (ret != WM_QUIT){
		ret = window.MessageLoop();
		float clear[] = { 0.3f, 0.3f, 0.3f, 0.0f };
		device.getContext()->ClearRenderTargetView(device.getRTV(), clear);
		device.getContext()->ClearDepthStencilView(device.getDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		// Box�̕`��
		device.getContext()->Draw(4, 0);// ���_��:���Ԗڂ̒��_�����邩


		device.getSwapChain()->Present(0, 0);
	}

	// �}�N�������[�X
	SAFE_RELEASE(vertexbuffer);
	SAFE_RELEASE(vs_buf);
	SAFE_RELEASE(ps_buf);
	SAFE_RELEASE(inputlayout);
	SAFE_RELEASE(rasterizer);
	SAFE_RELEASE(constantbuffer);


	return ret;
}