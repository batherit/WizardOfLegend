#include "stdafx.h"
#include "CAtlasObj.h"
#include "CCamera2D.h"
#include "CMap.h"
#include "CAtlasLoader.h"


CAtlasObj::CAtlasObj(CGameWorld & _rGameWorld, CMap & _rMap, const _atlas_obj_info & _rAtlasObjInfo, float _fX, float _fY, size_t _iWidth, size_t _iHeight)
	:	
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight),
	m_rMap(_rMap),
	m_stAtlasObjInfo(_rAtlasObjInfo)
{
}

CAtlasObj::~CAtlasObj()
{
}

void CAtlasObj::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 검출된 타일이 속한 아틀라스 비트맵을 가져온다.
	HDC memdc = CreateCompatibleDC(_hdc);

	//HBITMAP m_bitmapOldAtlas = (HBITMAP)SelectObject(memdc, m_rMapRenderInfo.vecAtlasLoaders[m_stAtlasObjInfo.iAtlasID]->GetBitmap());

	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	const vector<CAtlasLoader*>& vecAtloasLoaders = m_rMap.GetAtlasLoaders();

	StretchBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first + 1,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second + 1,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		vecAtloasLoaders[m_stAtlasObjInfo.iAtlasID]->GetMemDC(),
		m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.top,
		m_stAtlasObjInfo.rcOutputArea.right - m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.bottom - m_stAtlasObjInfo.rcOutputArea.top,
		SRCCOPY);
	DeleteDC(memdc);
}
