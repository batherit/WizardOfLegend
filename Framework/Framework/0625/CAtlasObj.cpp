#include "stdafx.h"
#include "CAtlasObj.h"
#include "CCamera2D.h"
#include "CMapLoader.h"
#include "CAtlasLoader.h"
#include "CBitmapMgr.h"
#include "CSpace.h"

const TCHAR* CAtlasObj::ctBitmapKey[3] = { L"WOL_TILE_DUNGEON", L"WOL_TILE_HOMETOWN", L"WOL_OBJECT", };

CAtlasObj::CAtlasObj(FILE* _fpIn, CGameWorld & _rGameWorld)
	:	
	CObj(_rGameWorld, 0.f, 0.f)
{	
	// 0) ������Ʈ Ÿ�԰� �׷� ���̵�
	CObj::LoadMapData(_fpIn);
	
	// 1) ��Ʋ�� �δ� ���̵�
	fscanf_s(_fpIn, " %d", &m_iAtlasID);

	// 2) ������Ʈ�� Ÿ�ϸʿ��� �����ϴ� ���� ����
	RECT rcRect;
	fscanf_s(_fpIn, " %d %d %d %d",
		&rcRect.left,
		&rcRect.top,
		&rcRect.right,
		&rcRect.bottom);

	// 3) ������Ʈ�� �����ϴ� ��Ʋ�� �̹��� ���� ����
	fscanf_s(_fpIn, " %d %d %d %d",
		&m_rcOutputArea.left,
		&m_rcOutputArea.top,
		&m_rcOutputArea.right,
		&m_rcOutputArea.bottom);

	// �о�� �����ͷκ��� pos�� w, h ����
	SetX((rcRect.right + rcRect.left) >> 1);
	SetY((rcRect.bottom + rcRect.top) >> 1);
	SetWidth(rcRect.right - rcRect.left);
	SetHeight(rcRect.bottom - rcRect.top);

	// �׸� ��Ʈ�� ������ ����
	m_hMemdc = CBitmapMgr::GetInstance()->GetBitmapMemDC(ctBitmapKey[m_iAtlasID]);
}

CAtlasObj::~CAtlasObj()
{
}

void CAtlasObj::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// �׸� ������ �����´�.
	RECT& rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first + 1,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second + 1,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		m_hMemdc,
		m_rcOutputArea.left,
		m_rcOutputArea.top,
		m_rcOutputArea.right - m_rcOutputArea.left,
		m_rcOutputArea.bottom - m_rcOutputArea.top,
		RGB(255, 0, 255));
	g_iRenderCount++;
	/*TCHAR szMode[32];
	swprintf_s(szMode, TEXT("GN : %d"), m_iGroupID);
	TextOut(_hdc, pairLeftTop.first, pairLeftTop.second, szMode, lstrlen(szMode));*/
}
