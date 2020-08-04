#include "stdafx.h"
#include "CDoor.h"
#include "CCamera2D.h"
#include "CBitmapMgr.h"
#include "CBitmapObj.h"


CDoor::CDoor(FILE * _fpIn, CGameWorld & _rGameWorld)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0, 0)
{
	SetObjType(OBJ::TYPE_WALL);
	// 0) 오브젝트 타입과 그룹 아이디
	CObj::LoadMapData(_fpIn);

	// 2) 오브젝트가 타일맵에서 차지하는 면적 정보
	RECT rcRect;
	fscanf_s(_fpIn, " %d %d %d %d",
		&rcRect.left,
		&rcRect.top,
		&rcRect.right,
		&rcRect.bottom);

	SetX((rcRect.right + rcRect.left) >> 1);
	SetY((rcRect.bottom + rcRect.top) >> 1);
	SetWidth(rcRect.right - rcRect.left);
	SetHeight(rcRect.bottom - rcRect.top);

	switch (m_eMapObjType) {
	case MAP_OBJ::TYPE_DOOR_HOR:
		m_tDoorBitmapKey = TEXT("PRISON_HOR");
		break;
	case MAP_OBJ::TYPE_DOOR_VER:
		m_tDoorBitmapKey = TEXT("PRISON_VER");
		break;
	}

	m_hMemdc = CBitmapMgr::GetInstance()->GetBitmapMemDC(m_tDoorBitmapKey);
	m_pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(m_tDoorBitmapKey);
	iWidth = m_pBitmapObj->GetWitdh();
	iHeight = m_pBitmapObj->GetHeight();
}

CDoor::~CDoor()
{
}

void CDoor::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 그릴 영역을 가져온다.
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);


	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hMemdc,
		0,
		0,
		iWidth,
		iHeight,
		RGB(255, 0, 255));
}
