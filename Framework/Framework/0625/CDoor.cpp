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
	// 0) ������Ʈ Ÿ�԰� �׷� ���̵�
	CObj::LoadMapData(_fpIn);

	// 2) ������Ʈ�� Ÿ�ϸʿ��� �����ϴ� ���� ����
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
	// �׸� ������ �����´�.
	RECT& rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);


	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		m_hMemdc,
		0,
		0,
		iWidth,
		iHeight,
		RGB(255, 0, 255));
}
