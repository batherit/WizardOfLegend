#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "ScrewBullet.h"
#include "GuidedBullet.h"
#include "GunBarrel.h"
#include "Shield.h"
//#include "MainApp.h"
#include "CTimer.h"

CPlayer::CPlayer(CGameWorld& _rGameWorld)
	:
	CObj(_rGameWorld, 0, 0, ciPlayerSize, ciPlayerSize, 0.f, 0.f, cfPlayerSpeed, Rectangle)
{
}

CPlayer::CPlayer(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth /*= ciPlayerSize*/, size_t _iHeight /*= ciPlayerSize*/, float _fSpeed /*= cfPlayerSpeed*/)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, 0.f, 0.f, _fSpeed, Rectangle),
	m_pGunBarrel(new CGunBarrel(_rGameWorld, this, 90.f, GetWidth(), 5.f)),
	m_fVelocityY(0.f)
{

}


CPlayer::~CPlayer()
{
}

void CPlayer::Ready(void)
{
}

int CPlayer::Update(float _fDeltaTime)
{
	CKeyMgr::GetInstance()->Update();
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_RIGHT)) {
		DO_IF_IS_VALID_OBJ(m_pGunBarrel) { m_pGunBarrel->RotateCW(_fDeltaTime); }
		MoveTo(cfDeltaX[OBJ::DIR_RIGHT] * m_fSpeed * _fDeltaTime, 0.f);
	}

	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_LEFT)) {
		DO_IF_IS_VALID_OBJ(m_pGunBarrel) { m_pGunBarrel->RotateCCW(_fDeltaTime); }
		MoveTo(cfDeltaX[OBJ::DIR_LEFT] * m_fSpeed * _fDeltaTime, 0.f);
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_UP)) {
		/*DO_IF_IS_VALID_OBJ(m_pGunBarrel) {
			MoveTo(m_pGunBarrel->GetDirX() * m_fSpeed, m_pGunBarrel->GetDirY() * m_fSpeed);
		}*/
		//MoveTo(cfDeltaX[OBJ::DIR_UP] * m_fSpeed, cfDeltaY[OBJ::DIR_UP] * m_fSpeed);
		m_fVelocityY = -250.f;
	}
	m_fVelocityY += GRAVITY * _fDeltaTime;
	MoveTo(0.f, m_fVelocityY * _fDeltaTime);

	if (IS_PRESS_(VK_DOWN)) {
		/*DO_IF_IS_VALID_OBJ(m_pGunBarrel) {
			MoveTo(-m_pGunBarrel->GetDirX()*m_fSpeed, -m_pGunBarrel->GetDirY()*m_fSpeed);
		}*/
		//MoveTo(cfDeltaX[OBJ::DIR_DOWN] * m_fSpeed, cfDeltaY[OBJ::DIR_DOWN] * m_fSpeed);
	}
	if (IS_PRESS_('D')) {
		DO_IF_IS_VALID_OBJ(m_pGunBarrel) {
			CObj* pBullet = new CScrewBullet(
				GetGameWorld(),
				m_pGunBarrel->GetEndX(), m_pGunBarrel->GetEndY(),
				m_pGunBarrel->GetDirX() + GetNumberMinBetweenMax(-0.01f, 0.01f),
				m_pGunBarrel->GetDirY() + GetNumberMinBetweenMax(-0.01f, 0.01f));
			pBullet->Ready();
			TO_GAMEWORLD(GetGameWorld()).GetBullets().emplace_back(pBullet);
		}
	}
	if (IS_PRESS_('S')) {
		DO_IF_IS_VALID_OBJ(m_pGunBarrel) {
			CObj* pBullet = new CBullet(
				GetGameWorld(),
				m_pGunBarrel->GetEndX(), m_pGunBarrel->GetEndY(),
				m_pGunBarrel->GetDirX() + GetNumberMinBetweenMax(-0.01f, 0.01f),
				m_pGunBarrel->GetDirY() + GetNumberMinBetweenMax(-0.01f, 0.01f));
			pBullet->Ready();
			TO_GAMEWORLD(GetGameWorld()).GetBullets().emplace_back(pBullet);
		}
	}
	if (IS_PRESS_('A')) {
		CObj* pShield = new CShield(GetGameWorld(), this);
		m_listShield.emplace_back(pShield);
	}
	if (IS_PRESS_('F')) {
		CObj* pBullet = new CGuidedBullet(
			GetGameWorld(),
			m_pGunBarrel->GetEndX(), m_pGunBarrel->GetEndY(),
			m_pGunBarrel->GetDirX() + GetNumberMinBetweenMax(-0.01f, 0.01f),
			m_pGunBarrel->GetDirY() + GetNumberMinBetweenMax(-0.01f, 0.01f));
		pBullet->Ready();
		TO_GAMEWORLD(GetGameWorld()).GetBullets().emplace_back(pBullet);
	}

	for (auto& pShield : m_listShield) { DO_IF_IS_VALID_OBJ(pShield) { pShield->Update(_fDeltaTime); } }

	// ÃÑ¾Ë »ý¼º
	//for (int dir = 0; dir < OBJ::DIR_END; dir++) {
	//	// DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN ¼øÀ¸·Î Å½»ö
	//	if (GetAsyncKeyState(ccDirKey[dir]) & 0x8000) {
	//		CObj* pBullet = new CBullet(
	//			GetX() + cfDeltaX[dir] * (GetWidth() >> 1), 
	//			GetY() + cfDeltaY[dir] * (GetHeight() >> 1),
	//			cfDeltaX[dir] + (cfDeltaX[dir] == 0.f ? GetNumberMinBetweenMax(-0.05f, 0.05f) : 0),
	//			cfDeltaY[dir] + (cfDeltaY[dir] == 0.f ? GetNumberMinBetweenMax(-0.05f, 0.05f) : 0));
	//		pBullet->Ready();
	//		m_plistBulletsW->emplace_back(pBullet);
	//	}
	//}

	DO_IF_IS_VALID_OBJ(m_pGunBarrel) { m_pGunBarrel->Update(_fDeltaTime); }

	return 0;
}

void CPlayer::LateUpdate(void)
{
	RECT rectCollidedPoint;
	DO_IF_IS_VALID_OBJ(this) {
		for (auto& pMonster : TO_GAMEWORLD(GetGameWorld()).GetMonsters()) {
			DO_IF_IS_VALID_OBJ(pMonster) {
				if (MyIntersectRect(GetRect(), pMonster->GetRect(), &rectCollidedPoint)) {
					if (rectCollidedPoint.bottom - rectCollidedPoint.top > rectCollidedPoint.right - rectCollidedPoint.left) {
						if (GetX() < pMonster->GetX()) {
							SetX(rectCollidedPoint.left - (GetWidth() >> 1) - 10);
						}
						else if(GetX() >= pMonster->GetX()){
							SetX(rectCollidedPoint.right + (GetWidth() >> 1) + 10);
						}
					}
					else {
						if (GetY() > pMonster->GetY()) {
							SetY(rectCollidedPoint.bottom + (GetHeight() >> 1) + 10);
						}
						else if (GetY() <= pMonster->GetY()) {
							SetY(rectCollidedPoint.top - (GetHeight() >> 1) - 10);
						}
						
					}
				}
			}
		}

		
		vector<pair<float, float>>& vecPoints = TO_GAMEWORLD(GetGameWorld()).GetPoints();
		for (int i = 0; i < vecPoints.size() - 1; i++) {
			if (vecPoints[i].first <= GetX() && GetX() <= vecPoints[i + 1].first) {
				float fInclination = (vecPoints[i].second - vecPoints[i + 1].second) / (vecPoints[i].first - vecPoints[i + 1].first);
				float fNewY = vecPoints[i].second + fInclination * (GetX() - vecPoints[i].first);

				if (fNewY <= GetY()) {
					SetY(fNewY);
					m_fVelocityY = 0.f;
				}
				break;
			}
		}
	}
}

void CPlayer::Render(const HDC & _hdc, CCamera2D* _pCamera)
{
	CObj::Render(_hdc, _pCamera);
	DO_IF_IS_VALID_OBJ(m_pGunBarrel) { m_pGunBarrel->Render(_hdc, _pCamera); }
	for (auto& pShield : m_listShield) { DO_IF_IS_VALID_OBJ(pShield) { pShield->Render(_hdc, _pCamera); } }
}

void CPlayer::Release(void)
{
	DeleteSafe(m_pGunBarrel);
	DeleteListSafe(m_listShield);
}
