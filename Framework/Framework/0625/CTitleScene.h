#pragma once
#include "CScene.h"

template<typename T> class CUI_Button;

class CTitleScene :
	public CScene
{
public:
	enum TITLE_TYPE { TYPE_READY, TYPE_START, TYPE_QUIT, TYPE_END };

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
	HDC m_hDCTitle[TYPE_END] = { nullptr, };
	TITLE_TYPE m_eType = TYPE_READY;
	int m_iWidth[TYPE_END] = { 0, };
	int m_iHeight[TYPE_END] = { 0, };
	CUI_Button<CTitleScene>* m_pButtons[TYPE_END] = { nullptr, };
};

