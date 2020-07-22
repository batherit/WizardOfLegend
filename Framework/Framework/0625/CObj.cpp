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
	// ��ȯ�� ����
	// 0 : Anim�� ���������� ������Ʈ �Ǿ���.
	// 1 : ���� Anim ���°� ��ȿȭ �����̸� ���ο� ���°� �Է��� �䱸��.

	if (m_stAnimInfo.iState < 0) return 1;
	if (m_stAnimInfo.iCountToRepeat != 0 && m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) return 1;
	// _anim_processing_info�� �����Ѵ�.

	m_stAnimProcessingInfo.fAnimElapsedTime += _fDeltaTime;
	if (m_stAnimProcessingInfo.fAnimElapsedTime > m_stAnimInfo.fTotalTime) {
		// �ִϸ��̼� �ѹ����� �� ��.
		if(0 != m_stAnimInfo.iCountToRepeat) {
			// ���� �ִ��� �ƴ϶��.
			m_stAnimProcessingInfo.iRepeatedCount++;
			if (m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) {
				m_stAnimProcessingInfo.fAnimElapsedTime = m_stAnimInfo.fTotalTime;
				return 1;
			}
		}
		else {
			// ���� �ִ��̶��.
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
			// �浹 ����Ʈ�� ���µ� �浹�ߴٸ�,
			RECT rcCollided;
			if (IntersectRect(&rcCollided, &_pObj->GetRect(), &this->GetRect())) {
				POINT ptCollisionPoint = {
					((rcCollided.right + rcCollided.left) >> 1),
					((rcCollided.bottom + rcCollided.top) >> 1)
				};
				if (_pCollisionPoint) *_pCollisionPoint = ptCollisionPoint;
				_pObj->Attacked(m_iDamage, ptCollisionPoint);					// �������� �ְ�
				m_listCollidedObjs.emplace_back(_pObj);		// �浹 ����Ʈ�� ����ִ´�.
				return true;
			}
		}
		else {
			// �浹 ����Ʈ�� �ִµ� �浹���� �ʾҴٸ�,
			if (!IsCollided(_pObj, this)) {
				m_listCollidedObjs.erase(iter);				// �浹 ����Ʈ���� �����Ѵ�.
			}
		}
	}
	return false;
}
