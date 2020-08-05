#include "stdafx.h"
#include "CWizardFire.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CWOL_World.h"
#include "CScene.h"
#include "CHitEffect.h"



CWizardFire::CWizardFire(CGameWorld & _rGameWorld, float _fX, float _fY, float _fToX, float _fToY)
	:
	CObj(_rGameWorld, _fX, _fY, WIZARD_FIRE_WIDTH, WIZARD_FIRE_HEIGHT, _fToX, _fToY, WIZARD_FIRE_SPEED)
{
	SetRenderLayer(2);
	SetObjType(OBJ::TYPE_MONSTER_SKILL);
	SetDamage(12);
	SetDamageOffset(3);

	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_FIRE"));

	m_pColliders[COLLIDER::TYPE_WALL] = this;
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;

	_anim_info stAnimInfo;
	stAnimInfo.iState = GetAnimState(_fToX, _fToY);
	stAnimInfo.iCountToRepeat = 0;	// 무한 반복
	stAnimInfo.fTotalTime = 2.f;	// 2초 순회
	stAnimInfo.iStartFrameIndex =0;
	stAnimInfo.iFrameCount = 6;

	SetNewAnimInfo(stAnimInfo);
}

CWizardFire::~CWizardFire()
{
	Release();
}

int CWizardFire::Update(float _fDeltaTime)
{
	MoveByDeltaTime(_fDeltaTime);
	
	return UpdateAnim(_fDeltaTime);
}

void CWizardFire::LateUpdate(void)
{
	//// 위자드 파이어 삭제 과정
	//DO_IF_IS_VALID_OBJ(this) {
	//	for (auto& pPlayerSkill : GetGameWorld().GetListObjs()) {
	//		DO_IF_IS_VALID_OBJ(pPlayerSkill) {
	//			RECT rcCollisionRect;
	//			if (IntersectRect(&rcCollisionRect, &pPlayerSkill->GetRect(), &this->GetRect())) {
	//				GetGameWorld().GetSceneManager()->GetCurScene()->GetHitEffects()->emplace_back(
	//					new CHitEffect(GetGameWorld(),
	//						GetX(),
	//						GetY())
	//				); 
	//				SetValid(false);
	//				break;
	//			}
	//		}
	//		DO_IF_IS_NOT_VALID_OBJ(this) break;
	//	}
	//}
}

void CWizardFire::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(110, 70, 210));
	g_iRenderCount++;
}


void CWizardFire::Release(void)
{
}

int CWizardFire::GetAnimState(float _fToX, float _fToY) const
{
	float fDegree = GetPositiveDegreeByVector(_fToX, _fToY);

	if (191.25f <= fDegree && fDegree < 213.75f) return 0;
	else if (213.75f <= fDegree && fDegree < 236.25f) return 1;
	else if (236.25f <= fDegree && fDegree < 258.75f) return 2;
	else if (258.75f <= fDegree && fDegree < 281.25f) return 3;
	else if (281.25f <= fDegree && fDegree < 303.75f) return 4;
	else if (303.75f <= fDegree && fDegree < 326.25f) return 5;
	else if (326.25f <= fDegree && fDegree < 348.75f) return 6;
	else if (11.25f <= fDegree && fDegree < 33.75f) return 8;
	else if (33.75f <= fDegree && fDegree < 56.25f) return 9;
	else if (56.25f <= fDegree && fDegree < 78.75f) return 10;
	else if (78.75f <= fDegree && fDegree < 101.25f) return 11;
	else if (101.25f <= fDegree && fDegree < 123.75f) return 12;
	else if (123.75f <= fDegree && fDegree < 146.25f) return 13;
	else if (146.25f <= fDegree && fDegree < 168.75f) return 14;
	else if (168.75f <= fDegree && fDegree < 191.25f) return 15;
	else return 7;
}

void CWizardFire::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint)
{
	switch (_pCollider->GetObjType())
	{
	case OBJ::TYPE_WALL: case OBJ::TYPE_PLAYER_SKILL:
		GetGameWorld().GetListObjs().emplace_back(new CHitEffect(GetGameWorld(), _ptCollisionPoint.x, _ptCollisionPoint.y));
		SetValid(false);
		break;
	default:
		break;
	}
}
