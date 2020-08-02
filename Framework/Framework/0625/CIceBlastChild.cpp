#include "stdafx.h"
#include "CIceBlastChild.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"



CIceBlastChild::CIceBlastChild(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, ICE_BLAST_WIDTH, ICE_BLAST_HEIGHT),
	m_fLifeTime(0.7f)
{
	SetDamage(12);
	SetDamageOffset(3);

	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SKILL_ICE_BLAST"));

	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;


	_anim_info stAnimInfo;
	stAnimInfo.iState = 0;
	stAnimInfo.fTotalTime = 0.2f;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iFrameCount = 3;
	stAnimInfo.iStartFrameIndex = 0;
	
	SetNewAnimInfo(stAnimInfo);

	int iRandSountIndex = rand() % 4;
	if (iRandSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("ICE_BLAST_1.mp3"), CSoundMgr::SKILL);
	else if (iRandSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("ICE_BLAST_2.mp3"), CSoundMgr::SKILL);
	else if (iRandSountIndex == 2)		CSoundMgr::Get_Instance()->PlaySound(TEXT("ICE_BLAST_3.mp3"), CSoundMgr::SKILL);
	else if (iRandSountIndex == 3)		CSoundMgr::Get_Instance()->PlaySound(TEXT("ICE_BLAST_4.mp3"), CSoundMgr::SKILL);
}

CIceBlastChild::~CIceBlastChild()
{
	Release();
}

int CIceBlastChild::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	if ((m_fElapsedTime += _fDeltaTime) < m_fLifeTime) {
		if (!m_bDestroyAnimInit && UpdateAnim(_fDeltaTime) == 1) {
			if (m_fElapsedTime >= m_fLifeTime * 0.7f) {
				// 파괴 애니메이션 재생
				_anim_info stAnimInfo;
				stAnimInfo.iState = 0;
				stAnimInfo.fTotalTime = m_fLifeTime * 0.3f;
				stAnimInfo.iCountToRepeat = 1;
				stAnimInfo.iFrameCount = 5;
				stAnimInfo.iStartFrameIndex = 3;
				SetNewAnimInfo(stAnimInfo);
				m_bDestroyAnimInit = true;
			}
		}
		return 0;
	}

	SetValid(false);
	return 1;
}

void CIceBlastChild::LateUpdate(void)
{
	UpdateCollidedObjs();
}

void CIceBlastChild::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
}

void CIceBlastChild::Release(void)
{
}
