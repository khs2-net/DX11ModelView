#ifndef _Xbox360_
#define _Xbox360_
#include <Windows.h>

class Xbox360
{
public:
	// �V���O���g��
	static Xbox360* GetInstance()
	{
		static Xbox360 instance;
		return &instance;
	}

	void Update(); // �X�V
	// ���X�e�B�b�N����
	bool LeftStickCheck(int i);   // ���X�e�B�b�N������Ă��邩�`�F�b�N
	bool LeftStickUP(int i);
	bool LeftStickDown(int i);
	bool LeftStickLeft(int i);
	bool LeftStickRight(int i);
	// �E�X�e�B�b�N����
	bool RightStickCheck(int i);  // �E�X�e�B�b�N������Ă��邩�`�F�b�N
	bool RightStickUP(int i);
	bool RightStickDown(int i);
	bool RightStickLeft(int i);
	bool RightStickRight(int i);
	// �{�^������
	bool Xbuttun(int i);
	bool Ybuttun(int i);
	bool Abuttun(int i);
	bool Bbuttun(int i);
	bool STARTbuttun(int i);
	bool BACKbuttun(int i);
	bool LBbuttun(int i);
	bool RBbuttun(int i);
	bool LTbuttun(int i);
	bool RTbuttun(int i);
	bool PADUPbuttun(int i);
	bool PADDownbuttun(int i);
	bool PADLeftbuttun(int i);
	bool PADRightbuttun(int i);
private:
	Xbox360(){}  // �R���X�g���N�^
};


#endif // !_Xbox360_
