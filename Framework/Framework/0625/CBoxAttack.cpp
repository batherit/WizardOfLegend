#include "stdafx.h"
#include "CBoxAttack.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CBottomHole.h"
#include "CHitEffect.h"
#include "CGaiaArmorChild.h"
#include "CCollider.h"
#include "CStonePillar.h"



CBoxAttack::CBoxAttack(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, BOSS_BOXATTACK_WIDTH, BOSS_BOXATTACK_HEIGHT),
	m_fYToReach(_fY - 50.f)
{
	SetRenderLayer(1);
	SetDamage(25);
	SetDamageOffset(1);

	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOSS_BOXATTACK"));
	CSoundMgr::Get_Instance()->PlaySound(TEXT("BOSS_DROP.mp3"), CSoundMgr::SKILL);
	_rGameWorld.GetListObjs().emplace_back(new CBottomHole(_rGameWorld, GetX(), GetY() + 60, 2.f));
}

CBoxAttack::~CBoxAttack()
{
	Release();
}

int CBoxAttack::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	if (!m_bIsBoxThrowing) {
		if ((m_fElapsedTime += _fDeltaTime) <= 0.8f) {
			SetY(m_fYToReach + 70 * (0.8f - m_fElapsedTime));
			return 0;
		}
		// 멈춰서 대기중인 상황
		return 1;
	}	

	MoveByDeltaTime(_fDeltaTime);

	return 0;
}

void CBoxAttack::LateUpdate(void)
{
	if(m_pColliders[COLLIDER::TYPE_WALL]) m_pColliders[COLLIDER::TYPE_WALL]->LateUpdate();
}

void CBoxAttack::ThrowBoxAttack(float _fToX, float _fToY, float _fSpeed)
{
	if (m_bIsBoxThrowing) return;

	SetToXY(_fToX, _fToY);
	SetSpeed(_fSpeed);
	SetObjType(OBJ::TYPE_MONSTER_SKILL);	// 몬스터 스킬로 속성 변경
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(),this, 0.f, 24.f, 110, 110);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = m_pColliders[COLLIDER::TYPE_WALL];
	m_bIsBoxThrowing = true;
}

void CBoxAttack::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
}

void CBoxAttack::Release(void)
{
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = nullptr;
}

void CBoxAttack::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT & _rcCollisionRect)
{
	switch (_pCollider->GetObjType())
	{
	case OBJ::TYPE_WALL:
		if (!dynamic_cast<CStonePillar*>(_pCollider)) {
			GetGameWorld().GetListObjs().emplace_back(new CHitEffect(GetGameWorld(), _ptCollisionPoint.x, _ptCollisionPoint.y));
			SetValid(false);
		}		
		break;
	case OBJ::TYPE_PLAYER_SKILL:
	{
		CObj* pSkill = dynamic_cast<CGaiaArmorChild*>(_pCollider);
		if (pSkill) {
			GetGameWorld().GetListObjs().emplace_back(new CHitEffect(GetGameWorld(), _ptCollisionPoint.x, _ptCollisionPoint.y));
			SetValid(false);
		}
	}
	default:
		break;
	}
}


