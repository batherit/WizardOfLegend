#include "stdafx.h"
#include "CPlayerNormalAttack.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CWOL_World.h"
#include "CSpace.h"


CPlayerNormalAttack::CPlayerNormalAttack(CGameWorld & _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, PLAYER::E_STATE _eAttackType)
	:
	CObj(_rGameWorld, _fX, _fY, PLAYER_NORMAL_ATTACK_WIDTH, PLAYER_NORMAL_ATTACK_HEIGHT, _fToX, _fToY)
{
	SetObjType(OBJ::TYPE_PLAYER_SKILL);
	SetDamage(10);
	SetDamageOffset(3);

	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_NORMAL_ATTACK"));
	
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	//m_pColliders[COLLIDER::TYPE_WALL] = this;
	
	float fDegree = GetPositiveDegreeByVector(_fToX, _fToY);
	_anim_info stAnimInfo;
	if (337.5f <= fDegree || fDegree < 22.5f)		stAnimInfo.iState = 0;
	else if (292.5f <= fDegree && fDegree < 337.5f)	stAnimInfo.iState = 1;
	else if (247.5f <= fDegree && fDegree < 292.5f)	stAnimInfo.iState = 2;
	else if (202.5f <= fDegree && fDegree < 247.5f) stAnimInfo.iState = 3;
	else if (157.5f <= fDegree && fDegree < 202.5f) stAnimInfo.iState = 4;
	else if (112.5f <= fDegree && fDegree < 157.5f) stAnimInfo.iState = 5;
	else if (67.5f <= fDegree && fDegree < 112.5f)	stAnimInfo.iState = 6;
	else if (22.5f <= fDegree && fDegree < 67.5f)	stAnimInfo.iState = 7;
	
	OBJ::E_DIRECTION eDir = GetDirByDegree(fDegree);
	switch (eDir)
	{
	case OBJ::DIR_LEFT:
		switch (_eAttackType)
		{
		case PLAYER::STATE_ATTACK1:
			stAnimInfo.iStartFrameIndex = 4;
			break;
		case PLAYER::STATE_ATTACK2:
			stAnimInfo.iStartFrameIndex = 0;
			break;
		default:
			break;
		}
		break;
	case OBJ::DIR_RIGHT:
		switch (_eAttackType)
		{
		case PLAYER::STATE_ATTACK1:
			stAnimInfo.iStartFrameIndex = 0;
			break;
		case PLAYER::STATE_ATTACK2:
			stAnimInfo.iStartFrameIndex = 4;
			break;
		default:
			break;
		}
		break;
	case OBJ::DIR_UP:
		switch (_eAttackType)
		{
		case PLAYER::STATE_ATTACK1:
			stAnimInfo.iStartFrameIndex = 0;
			break;
		case PLAYER::STATE_ATTACK2:
			stAnimInfo.iStartFrameIndex = 4;
			break;
		default:
			break;
		}
		break;
	case OBJ::DIR_DOWN:
		switch (_eAttackType)
		{
		case PLAYER::STATE_ATTACK1:
			stAnimInfo.iStartFrameIndex = 0;
			break;
		case PLAYER::STATE_ATTACK2:
			stAnimInfo.iStartFrameIndex = 4;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 0.4f;
	stAnimInfo.iFrameCount = 4;

	SetNewAnimInfo(stAnimInfo);

	int iRandSoundIndex = rand() % 3;
	if (iRandSoundIndex == 0) CSoundMgr::Get_Instance()->PlaySound(TEXT("NORMAL_ATTACK_1.mp3"), CSoundMgr::SKILL);
	if (iRandSoundIndex == 1) CSoundMgr::Get_Instance()->PlaySound(TEXT("NORMAL_ATTACK_2.mp3"), CSoundMgr::SKILL);
	if (iRandSoundIndex == 2) CSoundMgr::Get_Instance()->PlaySound(TEXT("NORMAL_ATTACK_3.mp3"), CSoundMgr::SKILL);
}

CPlayerNormalAttack::~CPlayerNormalAttack()
{
}

int CPlayerNormalAttack::Update(float _fDeltaTime)
{
	if (UpdateAnim(_fDeltaTime) == 1) {
		SetValid(false);
		return 1;
	}
	return 0;
}

void CPlayerNormalAttack::LateUpdate(void)
{
	UpdateCollidedObjs();
	//m_pColliders[COLLIDER::TYPE_DAMAGED]->LateUpdate();
}

void CPlayerNormalAttack::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CPlayerNormalAttack::Release(void)
{
}
