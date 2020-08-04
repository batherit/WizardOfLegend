#include "stdafx.h"
#include "CIceBlast.h"
#include "CScene.h"
#include "CMapObjsGroup.h"
#include "CIceBlastChild.h"



CIceBlast::CIceBlast(CGameWorld & _rGameWorld, CObj * _pOwner, float _fToX, float _fToY)
	:
	CObj(_rGameWorld, 0.f, 0.f, (ICE_BLAST_WIDTH *0.2f), (ICE_BLAST_HEIGHT *0.2f), _fToX, _fToY, ICE_BLAST_SPEED),
	m_pOwner(_pOwner)
{
	m_pColliders[COLLIDER::TYPE_WALL] = this;
	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
}

CIceBlast::~CIceBlast()
{
}

int CIceBlast::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	if ((m_fSpawnTime += _fDeltaTime) >= 0.06f) {
		// 스킬 생성
		GetGameWorld().GetListObjs().emplace_back(new CIceBlastChild(GetGameWorld(), GetX(), GetY()));
		m_fSpawnTime = 0.f;
	}

	if ((m_fElapsedtime += _fDeltaTime) > 0.6f) {
		float fLength = GetVectorLength(GetX() - m_pOwner->GetX(), GetY() - m_pOwner->GetY());
		if (fLength <= 10.f) {
			SetValid(false);
			return 1;
		}

		float fNewToX = m_pOwner->GetX() - GetX();
		float fNewToY = m_pOwner->GetY() - GetY();
		SetToXY(fNewToX, fNewToY);
	}

	MoveByDeltaTime(_fDeltaTime);
	return 0;
}

void CIceBlast::LateUpdate(void)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return;

	RECT rcCollidedRect;

	for (auto& pGroup : *TO_WOL(GetGameWorld()).GetSceneManager()->GetCurScene()->GetColliders()) {
		if (IsCollided(GetRect(), pGroup->GetRect())) {
			for (auto& pCollider : pGroup->GetMapObjs()) {
				if (IntersectRect(&rcCollidedRect, &GetRect(), &pCollider->GetRect())) {
					if (rcCollidedRect.bottom - rcCollidedRect.top > rcCollidedRect.right - rcCollidedRect.left) {
						if (GetX() <= pCollider->GetX()) {
							MoveTo(-(rcCollidedRect.right - rcCollidedRect.left), 0.f);
						}
						else if (GetX() >= pCollider->GetX()) {
							MoveTo((rcCollidedRect.right - rcCollidedRect.left), 0.f);
						}
					}
					else {
						if (GetY() >= pCollider->GetY()) {
							MoveTo(0.f, (rcCollidedRect.bottom - rcCollidedRect.top));
						}
						else if (GetY() <= pCollider->GetY()) {
							MoveTo(0.f, -(rcCollidedRect.bottom - rcCollidedRect.top));
						}

					}
				}
			}
		}
	}
}

void CIceBlast::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 렌더할 것이 없음
}

void CIceBlast::Release(void)
{
	// 해제할 것이 없음
}
