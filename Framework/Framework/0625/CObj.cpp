#include "stdafx.h"
#include "CObj.h"
#include "CCamera2D.h"

CObj::CObj(CGameWorld& _rGameWorld)
	:
	m_rGameWorld(_rGameWorld),
	m_bIsValid(true),
	m_fX(0.f),
	m_fY(0.f),
	m_iWidth(10),
	m_iHeight(10),
	m_fSpeed(0.f),
	m_pDrawFunc(Rectangle)
{
	ZeroMemory(m_pColliders, sizeof(m_pColliders));
}

CObj::CObj(CGameWorld& _rGameWorld, float _fX /*= 0*/, float _fY /*= 0*/, size_t _iWidth /*= 10*/, size_t _iHeight /*= 10*/,
	float _fToX /*= 0.f*/, float _fToY /*= -1.f*/, float _fSpeed /*= 0.f*/, BOOL(__stdcall *_pDrawFunc) (HDC hdc, int _left, int _right, int _top, int _bottom) /*= Rectangle*/)
	:
	m_rGameWorld(_rGameWorld),
	m_bIsValid(true),
	m_fX(_fX),
	m_fY(_fY),
	m_fToX(_fToX),
	m_fToY(_fToY),
	m_iWidth(_iWidth),
	m_iHeight(_iHeight),
	m_fSpeed(_fSpeed),
	m_pDrawFunc(_pDrawFunc)
{
	ZeroMemory(m_pColliders, sizeof(m_pColliders));
}


CObj::~CObj()
{
}

void CObj::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (_pCamera) {
		RECT rc = _pCamera->GetScreenRect(GetRect());
		if (m_pDrawFunc) m_pDrawFunc(_hdc, rc.left, rc.top, rc.right, rc.bottom);
	}
	else {
		if (m_pDrawFunc) m_pDrawFunc(_hdc, GetLeft(), GetTop(), GetRight(), GetBottom());
	}
}

void CObj::LoadMapData(FILE * _fpIn)
{
	fscanf_s(_fpIn, " %d %d", &m_eObjType, &m_iGroupID);
}

int CObj::UpdateAnim(float _fDeltaTime)
{
	// 반환값 내용
	// 0 : Anim이 정상적으로 업데이트 되었음.
	// 1 : 현재 Anim 상태가 무효화 상태이며 새로운 상태값 입력을 요구함.

	if (m_stAnimInfo.iState < 0) return 1;
	if (m_stAnimInfo.iCountToRepeat != 0 && m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) return 1;
	// _anim_processing_info를 갱신한다.

	m_stAnimProcessingInfo.fAnimElapsedTime += _fDeltaTime;
	if (m_stAnimProcessingInfo.fAnimElapsedTime > m_stAnimInfo.fTotalTime) {
		// 애니메이션 한바퀴를 다 돎.
		if(0 != m_stAnimInfo.iCountToRepeat) {
			// 무한 애님이 아니라면.
			m_stAnimProcessingInfo.iRepeatedCount++;
			if (m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) {
				m_stAnimProcessingInfo.fAnimElapsedTime = m_stAnimInfo.fTotalTime;
				return 1;
			}
		}
		else {
			// 무한 애님이라면.
			m_stAnimProcessingInfo.fAnimElapsedTime = 0.f;
			m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex;
		}
	}

	float fCutTime = m_stAnimInfo.fTotalTime / m_stAnimInfo.iFrameCount;
	int iIndexOffset = m_stAnimProcessingInfo.fAnimElapsedTime / fCutTime;
	if (m_stAnimInfo.bIsReversePlay) iIndexOffset *= -1;
	m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex + iIndexOffset;

	return 0;
}

void CObj::Attacked(float _fDamageAmount, POINT _ptCollisionPoint)
{
	Clamp(&(m_fHp -= _fDamageAmount), 0.f, cfPlayerMaxHp);
	CCamera2D* pCamera = m_rGameWorld.GetCamera();
	if (pCamera) pCamera->Shake(0.6f, 7.f, 4);
}

bool CObj::CheckCollision(CObj * _pObj, POINT* _pCollisionPoint /*= nullptr*/)
{
	DO_IF_IS_VALID_OBJ(_pObj) {
		auto iter = find(m_listCollidedObjs.begin(), m_listCollidedObjs.end(), _pObj);
		if (iter == m_listCollidedObjs.end()) {
			// 충돌 리스트에 없는데 충돌했다면,
			RECT rcCollided;
			if (IntersectRect(&rcCollided, &_pObj->GetCollider(COLLIDER::TYPE_DAMAGED)->GetRect(), &this->GetCollider(COLLIDER::TYPE_DAMAGED)->GetRect())) {
				POINT ptCollisionPoint = {
					((rcCollided.right + rcCollided.left) >> 1),
					((rcCollided.bottom + rcCollided.top) >> 1)
				};
				if (_pCollisionPoint) *_pCollisionPoint = ptCollisionPoint;
				_pObj->Attacked(m_iDamage, ptCollisionPoint);					// 데미지를 주고
				m_listCollidedObjs.emplace_back(_pObj);		// 충돌 리스트에 집어넣는다.
				return true;
			}
		}
		else {
			// 충돌 리스트에 있는데 충돌하지 않았다면,
			if (!IsCollided(_pObj, this)) {
				m_listCollidedObjs.erase(iter);				// 충돌 리스트에서 제거한다.
			}
		}
	}
	return false;
}
