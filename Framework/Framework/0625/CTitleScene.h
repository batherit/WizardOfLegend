#pragma once
#include "CScene.h"
class CTitleScene :
	public CScene
{
public:
	CTitleScene(CGameWorld& _rGameWorld);
	virtual ~CTitleScene();

	// CScene��(��) ���� ��ӵ�
	//virtual void Ready(void) override;
	virtual void ResetScene(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void Release(void) override;

private:
	HDC m_hMemdc = nullptr;
	int m_iWidth = 0;
	int m_iHeight = 0;
};

