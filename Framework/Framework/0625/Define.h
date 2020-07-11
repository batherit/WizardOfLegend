#pragma once

#define WINCX 800
#define WINCY 600

// Ű �Է� ����
//#define NOT_PRESS_(_Key)			((GetAsyncKeyState(_Key) ^ 0xFFFF0000) == 0)
//#define JUST_RELEASE_(_Key)			((GetAsyncKeyState(_Key) ^ 0xFFFF0001) == 0)
//#define KEEP_PRESSING_(_Key)		((GetAsyncKeyState(_Key) ^ 0xFFFF8000) == 0)
//#define JUST_PRESS_(_Key)			((GetAsyncKeyState(_Key) ^ 0xFFFF8001) == 0)
//#define IS_PRESS_KEY_(_Key)				(GetAsyncKeyState(_Key) & 0x8000)

// Ÿ�̸� ����
#define WORLD_TIMER	CGameWorldTimer::Instance()

// ������Ʈ ��ȿ�� üũ
#define IS_VALID_OBJ(_pObj) ((_pObj) && (_pObj->IsValid()))
#define DO_IF_IS_NOT_VALID_OBJ(_pObj) if(!IS_VALID_OBJ(_pObj))
#define DO_IF_IS_VALID_OBJ(_pObj) if(IS_VALID_OBJ(_pObj))

// �� ��ȯ ����
#define TO_SPACE(_pObj)				dynamic_cast<const CSpace*>(_pObj)
#define TO_PLAYER(_pObj)			dynamic_cast<CPlayer*>(_pObj)
#define TO_BULLET(_pObj)			dynamic_cast<CBullet*>(_pObj)
#define TO_SCREW_BULLET(_pObj)		dynamic_cast<CScrewBullet*> (_pObj)
#define TO_MONSTER(_pObj)			dynamic_cast<CMonster*>(_pObj)
#define TO_GAMEWORLD(_pGameWorld)	dynamic_cast<CMainApp&>(_pGameWorld)

// ���� ����
#define PI acos(-1.f) //== acos(cos(pi)) == pi
#define TWO_PI (2.f * PI)
#define TO_RADIAN(_fDegree) ((_fDegree) * TWO_PI / 360.f)
#define TO_DEGREE(_fRadian) ((_fRadian) * 360.f / TWO_PI)

#define DIR_X_DEGREE(_fDegree) cosf(TO_RADIAN(_fDegree))
#define DIR_Y_DEGREE(_fDegree) sinf(TO_RADIAN(_fDegree))

#define DIR_X_RADIAN(_fRadian) cosf(_fRadian)
#define DIR_Y_RADIAN(_fRadian) sinf(_fRadian)

#define CLAMP_RADIAN(_fRadian) ((_fRadian) -static_cast<int>((_fRadian) / TWO_PI) * TWO_PI)
#define CLAMP_DEGREE(_fDegree) ((_fDegree) -static_cast<int>((_fDegree) / 360.f) * 360.f)

// ���� ����
//50�ȼ� = �뷫 1cm = 0.01m �ε� ȭ��� 1cm�� 1m�� �����ϸ�...
//�߷°��ӵ��� 9.8m / s ^ 2 = > 50�ȼ� : 1m = x : 9.8m = > x = 490�ȼ�
//490�ȼ� / s ^ 2
#define GRAVITY 490.8f

// ��Ʈ����ũ �÷��� ����
#define IS_PRESS_(_Key) (GetAsyncKeyState(_Key) & 0x8000)
#define ALL_FLAG(_State, _StateSize) (flags = (1 << (size * 8)) - 1)
#define EMPTY_FLAG(_State) (_State = 0)
#define ON_FLAG(_State, _Mask) (_State |= _Mask) 
#define OFF_FLAG(_State, _Mask) (_State &= ~_Mask)
#define TOGGLE_FLAG(_State, _Mask) (_State ^= _Mask)
#define IS_ON_FLAG(_State, _Mask) ((bool)((_State & _Mask) != 0))

// Ű�Է� ����
#define	KM_LEFT		0x00000001
#define	KM_RIGHT	0x00000002 
#define KM_UP		0x00000004
#define KM_DOWN		0x00000008

#define	KM_LBUTTON	0x00000010
#define	KM_RBUTTON	0x00000020 
#define KM_Q		0x00000040
#define KM_E		0x00000080

#define KM_W		0x00000100
#define KM_A		0x00000200
#define KM_S		0x00000400
#define KM_D		0x00000800

