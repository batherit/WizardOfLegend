#pragma once

// 윈도우 사이즈
#define WINCX 1700// 1400 1920 
#define WINCY 1000// 800 1080

// 타이머 관련
#define WORLD_TIMER	CGameWorldTimer::Instance()

// 오브젝트 유효성 체크
#define IS_VALID_OBJ(_pObj) ((_pObj) && (_pObj->IsValid()))
#define DO_IF_IS_NOT_VALID_OBJ(_pObj) if(!IS_VALID_OBJ(_pObj))
#define DO_IF_IS_VALID_OBJ(_pObj) if(IS_VALID_OBJ(_pObj))

// 형 변환 관련
#define TO_SPACE(_pObj)				dynamic_cast<const CSpace*>(_pObj)
#define TO_PLAYER(_pObj)			dynamic_cast<CPlayer*>(_pObj)
#define TO_PLAYER_WOL(_pObj)		dynamic_cast<CPlayerWOL*>(_pObj)
#define TO_BULLET(_pObj)			dynamic_cast<CBullet*>(_pObj)
#define TO_SCREW_BULLET(_pObj)		dynamic_cast<CScrewBullet*> (_pObj)
#define TO_MONSTER(_pObj)			dynamic_cast<CMonster*>(_pObj)
#define TO_CAMERA2D(_pObj)			dynamic_cast<CCamera2D*>(_pObj)
#define TO_GAMEWORLD(_pGameWorld)	dynamic_cast<CMainApp&>(_pGameWorld)
#define TO_MAPTOOL(_pGameWorld)		dynamic_cast<CMapTool&>(_pGameWorld)
#define TO_WOL(_pGameWorld)			dynamic_cast<CWOL_World&>(_pGameWorld)

// 수학 관련
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

// 물리 관련
//50픽셀 = 대략 1cm = 0.01m 인데 화면속 1cm를 1m로 간주하면...
//중력가속도는 9.8m / s ^ 2 = > 50픽셀 : 1m = x : 9.8m = > x = 490픽셀
//490픽셀 / s ^ 2
#define GRAVITY 490.8f

// 비트마스크 플래그 관련
#define IS_PRESS_(_Key) (GetAsyncKeyState(_Key) & 0x8000)
#define ALL_FLAG(_State, _StateSize) (flags = (1 << (size * 8)) - 1)
#define EMPTY_FLAG(_State) (_State = 0)
#define ON_FLAG(_State, _Mask) (_State |= _Mask) 
#define OFF_FLAG(_State, _Mask) (_State &= ~_Mask)
#define TOGGLE_FLAG(_State, _Mask) (_State ^= _Mask)
#define IS_ON_FLAG(_State, _Mask) ((bool)((_State & _Mask) != 0))

// 키입력 관련
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

#define KM_LSHIFT	0x00001000
#define KM_P		0x00002000
#define KM_G		0x00004000
#define KM_SPACE	0x00008000

// 파일 입출력 관련
#define FB_WRITE_DATA(_valueName, _fpOut) fwrite(&(_valueName), sizeof(_valueName), 1, _fpOut)
#define FB_READ_DATA(_valueName, _fpIn) fread(&(_valueName), sizeof(_valueName), 1, _fpIn)
#define FB_WRITE_HEAD(_valueName, _fpOut) fwrite(&(_valueName), sizeof(_valueName), 1, _fpOut)
#define FB_READ_HEAD(_valueName, _fpIn) fread(&(_valueName), sizeof(_valueName), 1, _fpIn)




// 마우스 커서 사이즈
#define MOUSE_CURSOR_WITDH	60
#define MOUSE_CURSOR_HEIGHT 60

// 플레이어 비트맵 출력 사이즈
#define PLAYER_OUTPUT_WITDH			180
#define PLAYER_OUTPUT_HEIGHT		182

// 스워드맨 비트맵 출력 사이즈
#define SWORDMAN_OUTPUT_WIDTH		200
#define SWORDMAN_OUTPUT_HEIGHT		200