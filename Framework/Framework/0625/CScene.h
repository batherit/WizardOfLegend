#pragma once

class CGameWorld;

class CScene abstract
{
public:
	CScene(CGameWorld& _rGameWorld);
	virtual ~CScene();

public:
	//virtual void Ready(void) = 0;
	virtual void ResetScene(void) = 0;
	virtual int Update(float _fDeltaTime) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) = 0;
	virtual void Release(void) = 0;

	virtual list<CObj*>* GetHitEffects() { return nullptr; }

protected:
	CGameWorld& m_rGameWorld;
};

