#include "stdafx.h"
#include "CAtlasObj.h"
#include "CCamera2D.h"
#include "CMapLoader.h"
#include "CAtlasLoader.h"
#include "CBitmapMgr.h"

const TCHAR* CAtlasObj::ctBitmapKey[3] = { L"WOL_TILE_DUNGEON", L"WOL_TILE_HOMETOWN", L"WOL_OBJECT", };

CAtlasObj::CAtlasObj(FILE* _fpIn, CGameWorld & _rGameWorld)
	:	
	CObj(_rGameWorld, 0.f, 0.f)
{	
	// 0) 오브젝트 타입과 그룹 아이디
	CObj::LoadMapData(_fpIn);
	
	// 1) 아틀라스 로더 아이디
	fscanf_s(_fpIn, " %d", &m_iAtlasID);

	// 2) 오브젝트가 타일맵에서 차지하는 면적 정보
	RECT rcRect;
	fscanf_s(_fpIn, " %d %d %d %d",
		&rcRect.left,
		&rcRect.top,
		&rcRect.right,
		&rcRect.bottom);

	// 3) 오브젝트에 대응하는 아틀라스 이미지 면적 정보
	fscanf_s(_fpIn, " %d %d %d %d",
		&m_rcOutputArea.left,
		&m_rcOutputArea.top,
		&m_rcOutputArea.right,
		&m_rcOutputArea.bottom);

	// 읽어온 데이터로부터 pos와 w, h 설정
	SetX((rcRect.right + rcRect.left) >> 1);
	SetY((rcRect.bottom + rcRect.top) >> 1);
	SetWidth(rcRect.right - rcRect.left);
	SetHeight(rcRect.bottom - rcRect.top);

	// 그릴 비트맵 정보를 저장
	m_hMemdc = CBitmapMgr::GetInstance()->FindBitmapMemDC(ctBitmapKey[m_iAtlasID]);
}

CAtlasObj::~CAtlasObj()
{
}

void CAtlasObj::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first + 1,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second + 1,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hMemdc,
		m_rcOutputArea.left,
		m_rcOutputArea.top,
		m_rcOutputArea.right - m_rcOutputArea.left,
		m_rcOutputArea.bottom - m_rcOutputArea.top,
		RGB(255, 0, 255));

	TCHAR szMode[32];
	swprintf_s(szMode, TEXT("GN : %d"), m_iGroupID);
	TextOut(_hdc, pairLeftTop.first, pairLeftTop.second, szMode, lstrlen(szMode));
}
