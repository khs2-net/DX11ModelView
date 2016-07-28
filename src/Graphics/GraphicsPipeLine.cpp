#include <./Graphics/GraphicsPipeLine.h>


GraphicsPipeLine::GraphicsPipeLine(){
	this->status.reserve(8);
}
GraphicsPipeLine::GraphicsPipeLine(unsigned long size){
	this->status.reserve(size);
}

GraphicsPipeLine::~GraphicsPipeLine()
{
}
void GraphicsPipeLine::setStatus(){
	//�p�C�v���C���ɃX�e�[�^�X�̐ݒ�
	for (auto v : this->status) {
		if(v)v->SetStatus();
	}
}

bool GraphicsPipeLine::Draw() {
	//�p�C�v���C���ɃX�e�[�^�X�̐ݒ�
	this->setStatus();
	//�`��Ăяo��

	return false;
}
bool GraphicsPipeLine::Attach(GraphicStatus* ptr) {
	for (auto &v : this->status) {
		if (v == nullptr) {
			ptr->AddRef();
			v = ptr;
			return true;
		}
		else if (v == ptr) {
			return false;
		}
	}
	return false;
}
bool GraphicsPipeLine::Remove(GraphicStatus* ptr) {
	for (auto &v : this->status) {
		if (v == ptr) {
			v->Release();
			v = nullptr;
			return true;
		}
	}
	return false;
}

void GraphicsPipeLine::Release() {
	for (auto &v : this->status) {
		if (v) {
			v->Release();
			v = nullptr;
		}
	}
}