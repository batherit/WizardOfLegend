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
	// ����� Ÿ���� ���� ��Ʋ�� ��Ʈ���� �����´�.
	HDC memdc = CreateCompatibleDC(_hdc);

	//HBITMAP m_bitmapOldAtlas = (HBITMAP)SelectObject(memdc, m_rMapRenderInfo.vecAtlasLoaders[m_stAtlasObjInfo.iAtlasID]->GetBitmap());

	// �׸� ������ �����´�.
	RECT rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	const vector<CAtlasLoader*>& vecAtloasLoaders = m_rMap.GetAtlasLoaders();

	StretchBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first + 1,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second + 1,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		vecAtloasLoaders[m_stAtlasObjInfo.iAtlasID]->GetMemDC(),
		m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.top,
		m_stAtlasObjInfo.rcOutputArea.right - m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.bottom - m_stAtlasObjInfo.rcOutputArea.top,
		SRCCOPY);
	DeleteDC(memdc);
}
