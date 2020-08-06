#include "stdafx.h"
#include "CStonePillar.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CCollider.h"
#include "CBottomHole.h"



CStonePillar::CStonePillar(CGameWorld & _rGameWorld, float _fX, float _fY, float _fDelayTime, float _fLifeTime)
	:
	CObj(_rGameWorld, _fX, _fY - 90.f, BOSS_SKILL_WIDTH, BOSS_SKILL_HEIGHT),
	m_fDelayTime(_fDelayTime),
	m_fLifeTime(_fLifeTime)
{
	SetRenderLayer(1);
	SetDamage(14);
	SetDamageOffset(3);

	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOSS_SKILL"));

	_anim_info stAnimInfo;
	stAnimInfo.fTotalTime = 0.4f;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iFrameCount = 3;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iState = 0;

	SetNewAnimInfo(stAnimInfo);

	_rGameWorld.GetListObjs().emplace_back(new CBottomHole(_rGameWorld, GetX(), GetY() + 90.f));
}

CStonePillar::~CStonePillar()
{
}

int CStonePillar::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;
	if ((m_fElapsedTime += _fDeltaTime) >= m_fDelayTime) {
		if (UpdateAnim(_fDeltaTime) == 1) {
			// 석주가 솟아나는 애니메이션이 끝나면 객체 속성을 벽으로 설정한다.
			if (m_bAttackState) {
				SetObjType(OBJ::TYPE_WALL);
				m_pColliders[COLLIDER::TYPE_WALL] = m_pColliders[COLLIDER::TYPE_DAMAGED];
				m_bAttackState = false;
			}
		}
		else {
			if (m_bAttackState == false) {
				SetObjType(OBJ::TYPE_MONSTER_SKILL);
				m_pColliders[COLLIDER::TYPE_DAMAGED] = new CCollider(GetGameWorld(), this, 0.f, 90.f, 90, 90);
				m_bAttackState = true;
			}
		}
	}

	if (m_fElapsedTime >= m_fLifeTime) {
		SetValid(false);
		return 1;
	}
 	return 0;
}

void CStonePillar::LateUpdate(void)
{
}

void CStonePillar::Render(HDC & _hdc, CCamera2D * _pCamera)
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

	//if(m_pColliders[COLLIDER::TYPE_WALL])m_pColliders[COLLIDER::TYPE_WALL]->Render(_hdc, _pCamera);
}

void CStonePillar::Release(void)
{
	DeleteSafe(m_pColliders[COLLIDER::TYPE_DAMAGED]);
	m_pColliders[COLLIDER::TYPE_WALL] = nullptr;
}

void CStonePillar::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT& _rcCollisionRect)
{
	switch (_pCollider->GetObjType())
	{
	case OBJ::TYPE_PLAYER:
		// PlayScene과 더불어 문제있는 코드지만, 현 상황에선 괜찮음.
		if (GetObjType() == OBJ::TYPE_WALL) {
			CObj* pCollider1 = m_pColliders[COLLIDER::TYPE_WALL];
			CObj* pCollider2 = _pCollider->GetCollider(COLLIDER::TYPE_WALL);

			if (!pCollider1 || !pCollider2) return;

			if (IntersectRect(&_rcCollisionRect, &pCollider1->GetRect(), &pCollider2->GetRect())) {
				switch (GetDirByDegree(
					GetPositiveDegreeByVector(pCollider2->GetX() - pCollider1->GetX(), pCollider2->GetY() - pCollider1->GetY()), 1.f, 1.f, 0.f)) {
				case OBJ::DIR_RIGHT:
					_pCollider->MoveTo((_rcCollisionRect.right - _rcCollisionRect.left) + max(0, pCollider1->GetRect().right - _rcCollisionRect.right), 0.f);
					break;
				case OBJ::DIR_LEFT:
					_pCollider->MoveTo(-(_rcCollisionRect.right - _rcCollisionRect.left) - max(0, pCollider1->GetRect().left - _rcCollisionRect.left), 0.f);
					break;
				case OBJ::DIR_DOWN:
					_pCollider->MoveTo(0.f, (_rcCollisionRect.bottom - _rcCollisionRect.top) + max(0, pCollider1->GetRect().bottom - _rcCollisionRect.bottom));
					break;
				case OBJ::DIR_UP:
					_pCollider->MoveTo(0.f, -(_rcCollisionRect.bottom - _rcCollisionRect.top) - max(0, pCollider1->GetRect().top - _rcCollisionRect.top));
					break;
				}
				pCollider2->LateUpdate();
			}
			
		}
		break;
	default:
		break;
	}
}
