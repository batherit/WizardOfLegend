#pragma once

// 윈도우 사이즈
#define WINCX 1750// 1400 1920 
#define WINCY 950// 800 1080

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

#define KM_R		0x00010000
#define KM_TAB		0x00020000
#define KM_M		0x00040000			// 미니맵 관련
#define KM_F		0x00080000			// 아이템과 상호작용

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

// 플레이어 바 사이즈
#define PLAYER_BAR_WIDTH	328
#define PLAYER_BAR_HEIGHT	80
#define HP_BAR_WIDTH		244
#define HP_BAR_HEIGHT		32
#define MANA_BAR_WIDTH		192
#define MANA_BAR_HEIGHT		16

// 플레이어 상수
#define PLAYER_MAX_HP		500.f
#define PLAYER_MAX_MANA		500.f
#define PLAYER_MANA_REDUCTION_PER_SEC	5.f

// UI 스킬바 비트맵 출력 사이즈
#define UI_SKILLBAR_OUTPUT_WIDTH			640
#define UI_SKILLBAR_OUTPUT_HEIGHT			160

// UI 스킬키 바인딩 메뉴 출력 사이즈
#define UI_SKILLKEY_BINDING_MENU_WIDTH		576 
#define UI_SKILLKEY_BINDING_MENU_HEIGHT		865
// UI 스킬키 바인딩 버튼 출력 사이즈
#define UI_SKILLKEY_BINDING_BUTTON_WIDTH	52
#define UI_SKILLKEY_BINDING_BUTTON_HEIGHT	52
// UI 스킬키 바인딩 설명 출력 사이즈
#define UI_SKILLKEY_BINDING_DESC_WIDTH		475
#define UI_SKILLKEY_BINDING_DESC_HEIGHT		200


// UI 스킬바 아이콘 출력 사이즈
#define UI_SKILLBAR_ICON_OUTPUT_WIDTH	54
#define UI_SKILLBAR_ICON_OUTPUT_HEIGHT	54

// UI 미니맵 출력 사이즈
#define UI_PLAYER_MINIMAP_WIDHT			45
#define UI_PLAYER_MINIMAP_HEIGHT		45

// UI 돈 비트맵 출력 사이즈
#define UI_MONEY_OUTPUT_WIDTH	31
#define UI_MONEY_OUTPU_HEIGHT	31
// 코인 비트맵 출력 사이즈
#define COIN_OUTPUT_WIDTH	24
#define COIN_OUTPUT_HEIGHT	24

// 드랍되는 카드 아이템 사이즈
#define ITEM_DROPPED_CARD_WIDTH		128
#define ITEM_DROPPED_CARD_HEIGHT	192

// 타격 이펙트 비트맵 출력 사이즈
#define HIT_EFFECT_OUTPUT_WIDTH		230
#define HIT_EFFECT_OUTPUT_HEIGHT	230

// 스워드맨 비트맵 출력 사이즈
#define SWORDMAN_OUTPUT_WIDTH		200
#define SWORDMAN_OUTPUT_HEIGHT		200

// 아쳐 비트맵 출력 사이즈
#define ARCHER_OUTPUT_WIDTH			200
#define ARCHER_OUTPUT_HEIGHT		200

// 아쳐 변수
#define ARCHER_RUN_SPEED			300.f
#define ARCHER_MAX_HP				80.f
#define ARCHER_ATTACKABLE_RANGE_IN	250.f
#define ARCHER_ATTACKABLE_RANGE_OUT	600.f
#define ARCHER_NORMAL_ATTACK_DIST	120.f
#define ARCHER_KNOCKBACK_SPEED		250.f   
// 아쳐 보우 변수
#define ARCHER_BOW_OUTPUT_WIDTH		180.f
#define ARCHER_BOW_OUTPUT_HEIGHT	170.f
// 아쳐 애로우 변수
#define ARCHER_ARROW_SPEED			1400.f

// 위자드 비트맵 출력 사이즈
#define WIZARD_OUTPUT_WIDTH			170.f
#define WIZARD_OUTPUT_HEIGHT		230.f

// 위자드 변수
#define WIZARD_RUN_SPEED			100.f
#define WIZARD_MAX_HP				80.f
#define WIZARD_KNOCKBACK_SPEED		250.f
#define WIZARD_ATTACKABLE_RANGE_IN	250.f
#define WIZARD_ATTACKABLE_RANGE_OUT	600.f

// 위자드 파이어 비트맵 출력 사이즈
#define WIZARD_FIRE_WIDTH			90.f
#define WIZARD_FIRE_HEIGHT			90.f
// 위자드 파이어 변수
#define WIZARD_FIRE_SPEED			200.f

// 위자드 볼 비트맵 출력 사이즈
#define WIZARD_BALL_OUTPUT_WIDTH	90.f
#define WIZARD_BALL_OUTPUT_HEIGHT	90.f

// 위자드 볼 변수
#define WIZARD_BALL_RUN_SPEED			100.f
#define WIZARD_BALL_ATTACK_SPEED		1800.f
#define WIZARD_BALL_MAX_HP				60.f
#define WIZARD_BALL_ATTACKABLE_RANGE	500.f
#define WIZARD_KNOCKBACK_SPEED			250.f  
#define WIZARD_BALL_BALLAST_SPEED		100.f

// 플레이어 노멀 공격 사이즈
#define PLAYER_NORMAL_ATTACK_WIDTH	200
#define PLAYER_NORMAL_ATTACK_HEIGHT	200
// 파이어 드래곤 사이즈
#define PLAYER_FIRE_DRAGON_WIDTH		180
#define PLAYER_FIRE_DRAGON_HEIGHT		180
#define PLAYER_FIRE_DRAGON_SPEED		1700.f
#define PLAYER_FIRE_DRAGON_ROT_SPEED	360.f
#define PLAYER_FIRE_DRAGON_DELTA_DEGREE 40
// 파이어 파티클 사이즈
#define FIRE_PARTICLE_WIDTH		70
#define FIRE_PARTICLE_HEIGHT	70
// 아이스 크리스탈 차일드 사이즈
#define ICE_CRYSTAL_WIDTH				200
#define ICE_CRYSTAL_HEIGHT				200
#define ICE_CRYSTAL_MIN_ROT_SPEED		20.f
#define ICE_CRYSTAL_MAX_ROT_SPEED		900.f
#define ICE_CRYSTAL_MIN_DIST			100.f
#define ICE_CRYSTAL_MAX_DIST			500.f
// 가이아 아머 사이즈
#define GAIA_ARMOR_WIDTH		100
#define GAIA_ARMOR_HEIGHT		102
// 가이아 아머 변수
#define GAIA_ARMOR_MAX_ROT_SPEED	180.f
#define GAIA_ARMOR_OFFSET			145.f

// 스워드맨 공격 사이즈
#define SWORDMAN_ATTACK_WIDTH	200
#define SWORDMAN_ATTACK_HEIGHT	200

// UI 데미지 텍스트 관련
#define UI_DAMAGE_TEXT_KEEP_TIME	0.6f;
#define UI_DAMAGE_TEXT_MAX_SIZE		50
#define UI_DAMAGE_TEXT_MIN_SIZE		20

// 상점 NPC
#define SHOP_NPC_WIDTH		128
#define SHOP_NPC_HEIGHT		197
// 상점 테이블
#define SHOP_TABLE_WIDTH	606
#define SHOP_TABLE_HEIGHT	234
// 상점 포션
#define SHOP_POTION_WIDTH	106
#define SHOP_POTION_HEIGHT	155
// 상점 가이아
#define SHOP_GAIA_WIDTH		106
#define SHOP_GAIA_HEIGHT	172