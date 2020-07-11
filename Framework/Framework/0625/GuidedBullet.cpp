#include "stdafx.h"
#include "GuidedBullet.h"
#include "MainApp.h"

CGuidedBullet::CGuidedBullet(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, size_t _iWidth, size_t _iHeight, float _fSpeed, float _fDamage)
	:
	CBullet(_rGameWorld, _fX, _fY, _fToX, _fToY, _iWidth, _iHeight, _fSpeed, _fDamage)
{
}


CGuidedBullet::~CGuidedBullet()
{
	Release();
}

void CGuidedBullet::Ready(void)
{
}

int CGuidedBullet::Update(float _fDeltaTime)
{
	float fMinDist = 987654321.f;
	float fToX = 0.f;
	float fToY = 0.f;
	for (auto& pMonster : TO_GAMEWORLD(GetGameWorld()).GetMonsters()) {
		fToX = pMonster->GetX() - GetX();
		fToY = pMonster->GetY() - GetY();
		if (fMinDist > GetVectorLength(pMonster->GetX() - GetX(), pMonster->GetY() - GetY())) {
			fToX = pMonster->GetX() - GetX();
			fToY = pMonster->GetY() - GetY();
		}
	}
	
	if (fToX != 0.f || fToY != 0.f) {
		NormalizeVector(fToX, fToY);
		fToX = fToX * 0.15f + GetToX() * 0.85f;
		fToY = fToY * 0.15f + GetToY() * 0.85f;
		NormalizeVector(fToX, fToY);
		SetToX(fToX);
		SetToY(fToY);
	}
	CBullet::Update(_fDeltaTime);
	return 0;
}

void CGuidedBullet::LateUpdate(void)
{
	CBullet::LateUpdate();
}

void CGuidedBullet::Render(const HDC & _hdc, CCamera2D* _pCamera)
{
	CBullet::Render(_hdc, _pCamera);
}

void CGuidedBullet::Release(void)
{
	// 부모 것을 릴리즈할 필요는 없는 것 같다.
	CBullet::Release();
}
