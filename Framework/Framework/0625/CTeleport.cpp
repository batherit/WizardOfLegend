#include "stdafx.h"
#include "CTeleport.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CBossScene.h"



CTeleport::CTeleport(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, TELEPORT_WIDTH, TELEPORT_HEIGHT)
{
	m_hDCKey = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("TELEPORT"));
}

CTeleport::~CTeleport()
{
	Release();
}

int CTeleport::Update(float _fDeltaTime)
{
	CObj* pPlayer = TO_WOL(GetGameWorld()).GetPlayer();
	float fDist = GetVectorLength(pPlayer->GetX() - GetX(), pPlayer->GetY() - GetY());

	if (fDist < 200.0f) {
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_F)) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
			GetGameWorld().GetSceneManager()->SetNextScene(new CBossScene(GetGameWorld(), "../MapDatas/Maps/Game_Map_Boss.txt"));
			GetGameWorld().GetSceneManager()->RequestSceneInit();
		}
	}
	return 0;
}

void CTeleport::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		m_hDCKey,
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CTeleport::Release(void)
{
}