#include "stdafx.h"
#include "CSwordManAttack.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CBitmapMgr.h"




CSwordManAttack::CSwordManAttack(CGameWorld & _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, SWORDMAN::E_DIRECTION _eSwordManDir)
	:
	CObj(_rGameWorld, _fX, _fY, SWORDMAN_ATTACK_WIDTH, SWORDMAN_ATTACK_HEIGHT, _fToX, _fToY)
{
	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SWORDMAN_ATTACK"));
	float fDegree = GetPositiveDegreeByVector(_fToX, _fToY);
	OBJ::E_DIRECTION eDir = GetDirByDegree(fDegree);

	_anim_info stAnimInfo;
	switch (eDir) {
	case OBJ::DIR_LEFT:
		stAnimInfo.iState = 0;
		break;
	case OBJ::DIR_RIGHT:
		stAnimInfo.iState = 1;
		break;
	case OBJ::DIR_UP:
		switch (_eSwordManDir) {
		case SWORDMAN::DIR_LEFT:
			stAnimInfo.iState = 2;
			break;
		case SWORDMAN::DIR_RIGHT:
			stAnimInfo.iState = 3;
			break;
		default:
			break;
		}
		break;
	case OBJ::DIR_DOWN:
		switch (_eSwordManDir) {
		case SWORDMAN::DIR_LEFT:
			stAnimInfo.iState = 4;
			break;
		case SWORDMAN::DIR_RIGHT:
			stAnimInfo.iState = 5;
			break;
		default:
			break;
		}
		break;
	}

	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 0.4f;
	stAnimInfo.iFrameCount = 4;

	SetNewAnimInfo(stAnimInfo);
}

CSwordManAttack::~CSwordManAttack()
{
}

int CSwordManAttack::Update(float _fDeltaTime)
{
	if (UpdateAnim(_fDeltaTime) == 1) {
		SetValid(false);
		return 1;
	}
	return 0;
}

void CSwordManAttack::LateUpdate(void)
{
}

void CSwordManAttack::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first + 1,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second + 1,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CSwordManAttack::Release(void)
{
}
