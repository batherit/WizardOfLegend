#include "stdafx.h"
#include "CArcherBow.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CMonster_Archer.h"



CArcherBow::CArcherBow(CGameWorld & _rGameWorld, CObj& _rOwner)
	:
	CObj(_rGameWorld, 0.f, 0.f, ARCHER_BOW_OUTPUT_WIDTH, ARCHER_BOW_OUTPUT_HEIGHT, 0.f, 0.f),
	m_rOwner(_rOwner)
{
	m_hDCKeyAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_LEFT"));
	m_hDCKeyAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_RIGHT"));
	
	_anim_info stAnimInfo;

	stAnimInfo.iState = GetAnimState();
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iFrameCount = 4;
	stAnimInfo.fTotalTime = 1.5f;
	stAnimInfo.iStartFrameIndex = 0;

	SetNewAnimInfo(stAnimInfo);
}

CArcherBow::~CArcherBow()
{
}

void CArcherBow::Ready(void)
{
	_anim_info stAnimInfo;
	stAnimInfo.iState = GetAnimState();
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iFrameCount = 4;
	stAnimInfo.fTotalTime = 2.0f;
	stAnimInfo.iStartFrameIndex = 0;

	SetNewAnimInfo(stAnimInfo);
}

int CArcherBow::Update(float _fDeltaTime)
{
	SetXY(m_rOwner.GetX(), m_rOwner.GetY());
	SetToXY(m_rOwner.GetToX(), m_rOwner.GetToY());
	m_stAnimInfo.iState = GetAnimState();
	return UpdateAnim(_fDeltaTime);
}

void CArcherBow::LateUpdate(void)
{
}

void CArcherBow::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	ARCHER::E_DIRECTION eArcherDir = dynamic_cast<CMonster_Archer&>(m_rOwner).GetArcherDir();

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas[eArcherDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(0, 255, 255));
	g_iRenderCount++;
}

void CArcherBow::Release(void)
{
}

int CArcherBow::GetAnimState(void) const
{
	float fDegree = GetPositiveDegreeByVector(m_rOwner.GetToX(), m_rOwner.GetToY());
	// 22.5
	if (258.75f <= fDegree && fDegree < 281.25f)		return 0;
	else if (281.25f <= fDegree && fDegree < 303.75f)	return 1;
	else if (303.75f <= fDegree && fDegree < 326.25f)	return 2;
	else if (326.25f <= fDegree && fDegree < 348.75f)	return 3;
	else if (11.25f <= fDegree && fDegree < 33.75f)		return 5;
	else if (33.75f <= fDegree && fDegree < 56.25f)		return 6;
	else if (56.25f <= fDegree && fDegree < 78.75f)		return 7;
	else if (78.75f <= fDegree && fDegree < 101.25f)	return 8;
	else if (101.25f <= fDegree && fDegree < 123.75f)	return 7;
	else if (123.75f <= fDegree && fDegree < 146.25f)	return 6;
	else if (146.25f <= fDegree && fDegree < 168.75f)	return 5;
	else if (168.75f <= fDegree && fDegree < 191.25f)	return 4;
	else if (191.25f <= fDegree && fDegree < 213.75f)	return 3;
	else if (213.75f <= fDegree && fDegree < 236.25f)	return 2;
	else if (236.25f <= fDegree && fDegree < 258.75f)	return 1;
	return 4;
}
