#include "stdafx.h"
#include "CFireDragon.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CFireParticle.h"
#include "CCollider.h"
#include "CHitEffect.h"
#include "CGaiaArmorChild.h"


CFireDragon::CFireDragon(CGameWorld & _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, PLAYER::E_STATE _eAttackType)
	:
	CObj(_rGameWorld, _fX, _fY, PLAYER_FIRE_DRAGON_WIDTH, PLAYER_FIRE_DRAGON_HEIGHT, _fToX, _fToY, PLAYER_FIRE_DRAGON_SPEED)
{
	SetObjType(OBJ::TYPE_PLAYER_SKILL);
	SetDamage(14);
	SetDamageOffset(3);

	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SKILL_FIREDRAGON"));

	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(_rGameWorld, this, 0.0f, 0.0f, 80.f, 80.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = m_pColliders[COLLIDER::TYPE_WALL];

	// 우 : 0, 좌 : 1
	m_iState = (_fToX > 0 ? 0 : 1);

	m_fInitDegree = GetPositiveDegreeByVector(_fToX, _fToY);
	OBJ::E_DIRECTION eDir = GetDirByDegree(m_fInitDegree);
	switch (eDir)
	{
	case OBJ::DIR_LEFT:
		switch (_eAttackType)
		{
		case PLAYER::STATE_ATTACK1:
			// 아래로
			m_fInitDegree -= (PLAYER_FIRE_DRAGON_DELTA_DEGREE >> 1);
			m_fDeltaDegree = PLAYER_FIRE_DRAGON_DELTA_DEGREE;
			break;
		case PLAYER::STATE_ATTACK2:
			// 위로
			m_fInitDegree += (PLAYER_FIRE_DRAGON_DELTA_DEGREE >> 1);
			m_fDeltaDegree = -PLAYER_FIRE_DRAGON_DELTA_DEGREE;
			break;
		default:
			break;
		}
		break;
	case OBJ::DIR_RIGHT:
		switch (_eAttackType)
		{
		case PLAYER::STATE_ATTACK1:
			// 아래로
			m_fInitDegree += (PLAYER_FIRE_DRAGON_DELTA_DEGREE >> 1);
			m_fDeltaDegree = -PLAYER_FIRE_DRAGON_DELTA_DEGREE;
			break;
		case PLAYER::STATE_ATTACK2:
			// 위로
			m_fInitDegree -= (PLAYER_FIRE_DRAGON_DELTA_DEGREE >> 1);
			m_fDeltaDegree = PLAYER_FIRE_DRAGON_DELTA_DEGREE;
			break;
		default:
			break;
		}
		break;
	case OBJ::DIR_UP:
		switch (_eAttackType)
		{
		case PLAYER::STATE_ATTACK1:
			// 오른쪽
			m_fInitDegree += (PLAYER_FIRE_DRAGON_DELTA_DEGREE >> 1);
			m_fDeltaDegree = -PLAYER_FIRE_DRAGON_DELTA_DEGREE;
			break;
		case PLAYER::STATE_ATTACK2:
			// 왼쪽
			m_fInitDegree -= (PLAYER_FIRE_DRAGON_DELTA_DEGREE >> 1);
			m_fDeltaDegree = PLAYER_FIRE_DRAGON_DELTA_DEGREE;
			break;
		default:
			break;
		}
		break;
	case OBJ::DIR_DOWN:
		switch (_eAttackType)
		{
		case PLAYER::STATE_ATTACK1:
			// 왼쪽 내기준
			m_fInitDegree += PLAYER_FIRE_DRAGON_DELTA_DEGREE >> 1;
			m_fDeltaDegree = -PLAYER_FIRE_DRAGON_DELTA_DEGREE;
			break;
		case PLAYER::STATE_ATTACK2:
			// 오른쪽 내기준
			m_fInitDegree -= PLAYER_FIRE_DRAGON_DELTA_DEGREE >> 1;
			m_fDeltaDegree = PLAYER_FIRE_DRAGON_DELTA_DEGREE;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	int iRandSountIndex = rand() % 3;
	if (iRandSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("FIRE_DRAGON_1.mp3"), CSoundMgr::MONSTER);
	else if (iRandSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("FIRE_DRAGON_2.mp3"), CSoundMgr::MONSTER);
	else if (iRandSountIndex == 2)		CSoundMgr::Get_Instance()->PlaySound(TEXT("FIRE_DRAGON_3.mp3"), CSoundMgr::MONSTER);
}

CFireDragon::~CFireDragon()
{
	Release();
	int iRandSountIndex = rand() % 3;
	if (iRandSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("FIRE_DRAGON_DIE_1.mp3"), CSoundMgr::MONSTER);
	else if (iRandSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("FIRE_DRAGON_DIE_2.mp3"), CSoundMgr::MONSTER);
	else if (iRandSountIndex == 2)		CSoundMgr::Get_Instance()->PlaySound(TEXT("FIRE_DRAGON_DIE_3.mp3"), CSoundMgr::MONSTER);
}

int CFireDragon::Update(float _fDeltaTime)
{
	m_fLifeTime += _fDeltaTime;
	m_fElapsedTime += _fDeltaTime;
	if (m_fElapsedTime >= 0.02f) {
		TO_WOL(GetGameWorld()).GetListParticles().emplace_back(new CFireParticle(GetGameWorld(), GetX(), GetY()));
		m_fElapsedTime = 0.f;
	}

	if (m_fLifeTime >= 5.f) {
		SetValid(false);
		return 1;
	}

	m_fCycleDegree += _fDeltaTime * PLAYER_FIRE_DRAGON_ROT_SPEED;
	if (m_fCycleDegree >= 360.f) m_fCycleDegree -= 360.f;
	m_fCurrentDegree = m_fInitDegree + m_fDeltaDegree * (-cosf(TO_RADIAN(m_fCycleDegree)) * 0.5f + 0.5f);

	if (m_fCurrentDegree >= 360.f) m_fCurrentDegree -= 360.f;
	else if (m_fCurrentDegree < 0.f) m_fCurrentDegree += 360.f;

	SetToX(cosf(TO_RADIAN(m_fCurrentDegree)));
	SetToY(sinf(TO_RADIAN(m_fCurrentDegree)));

	MoveByDeltaTime(_fDeltaTime);

	return 0;
}

void CFireDragon::LateUpdate(void)
{
	UpdateCollidedObjs();
	m_pColliders[COLLIDER::TYPE_WALL]->LateUpdate();
}

void CFireDragon::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	int iSpriteIndex = GetSpriteIndex();

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas,
		iSpriteIndex * PLAYER_FIRE_DRAGON_WIDTH,
		m_iState * PLAYER_FIRE_DRAGON_HEIGHT,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CFireDragon::Release(void)
{
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = nullptr;	// 얕은 복사이므로.
}

void CFireDragon::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint)
{
	switch (_pCollider->GetObjType())
	{
	case OBJ::TYPE_WALL:
		GetGameWorld().GetListObjs().emplace_back(new CHitEffect(GetGameWorld(), _ptCollisionPoint.x, _ptCollisionPoint.y));
		SetValid(false);
		break;
	case OBJ::TYPE_PLAYER_SKILL:
	{
		CObj* pSkill = dynamic_cast<CGaiaArmorChild*>(_pCollider);
		if (GetObjType() == OBJ::TYPE_MONSTER_SKILL && pSkill) {
			GetGameWorld().GetListObjs().emplace_back(new CHitEffect(GetGameWorld(), _ptCollisionPoint.x, _ptCollisionPoint.y));
			SetValid(false);
		}
	}
	break;
	default:
		break;
	}
}

int CFireDragon::GetSpriteIndex(void)
{
	int iIndex = 0;
	float fDegree = 0.f;
	if (m_iState == 0) {
		// 우방향
		fDegree = m_fCurrentDegree;
		if (180.f < m_fCurrentDegree && m_fCurrentDegree < 270.f) fDegree = 270.f;
		else if (180.f >= m_fCurrentDegree && m_fCurrentDegree > 90.f) fDegree = 90.f;

		fDegree -= 270.f;
		if (fDegree < 0.f) fDegree += 360.f;
		iIndex = fDegree / (180.f / 13.f);
		if (iIndex > 12) iIndex = 12;
		else if (iIndex < 0) iIndex = 0;
		iIndex = 15 - iIndex;
	}
	else {
		// 좌방향
		fDegree = m_fCurrentDegree;
		if (270.f < m_fCurrentDegree && m_fCurrentDegree < 360.f) fDegree = 270.f;
		else if (90.f > m_fCurrentDegree && m_fCurrentDegree >= 0.f) fDegree = 90.f;

		fDegree -= 90.f;
		if (fDegree < 0.f) fDegree += 360.f;
		iIndex = fDegree / (180.f / 13.f);
		if (iIndex > 12) iIndex = 12;
		else if (iIndex < 0) iIndex = 0;
		iIndex = 15 - iIndex;
	}
	return iIndex;
}
