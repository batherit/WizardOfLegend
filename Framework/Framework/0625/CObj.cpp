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
	// 반환값 내용
	// 0 : Anim이 정상적으로 업데이트 되었음.
	// 1 : 현재 Anim 상태가 무효화 상태이며 새로운 상태값 입력을 요구함.

	if (m_stAnimInfo.iState < 0) return 1;
	// _anim_processing_info를 갱신한다.
	m_stAnimProcessingInfo.fAnimElapsedTime += _fDeltaTime;

	if (m_stAnimProcessingInfo.fAnimElapsedTime > m_stAnimInfo.fTotalTime) {
		// 애니메이션 한바퀴를 다 돌았다!
		m_stAnimProcessingInfo.fAnimElapsedTime = 0.f;
		m_stAnimProcessingInfo.iCurrentIndex = 0;

		if(0 != m_stAnimInfo.iCountToRepeat) {
			m_stAnimProcessingInfo.iRepeatedCount++;
			if (m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) {
				//SetNewAnimInfo(_anim_info()); // 무효화 상태를 집어넣는다.
				return 1;
			}
		}
	}

	float fCutTime = m_stAnimInfo.fTotalTime / (m_stAnimInfo.iFrameCount - m_stAnimInfo.iStartFrameIndex);
	m_stAnimProcessingInfo.iCurrentIndex = m_stAnimProcessingInfo.fAnimElapsedTime / fCutTime;

	return 0;
}
