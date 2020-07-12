#pragma once
#include "CObj.h"

template<typename T>
class CUI_Button :
	public CObj
{
public:
	CUI_Button(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, const TCHAR* _szButtonName, T* _pOwner, void (T::*_pEvent) ());
	~CUI_Button();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(const HDC& _hdc, CCamera2D* _pCamera) override;

private:
	T* m_pOwner;
	void(T::*m_pEvent) (void);
	TCHAR m_szButtonName[64];
};

template<typename T>
inline CUI_Button<T>::CUI_Button(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, const TCHAR* _szButtonName, T* _pOwner = nullptr, void (T::*_pEvent) () = nullptr)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight),
	m_pOwner(_pOwner),
	m_pEvent(_pEvent)
{
	if (_pEvent) m_pEvent = _pEvent;
	wcscpy_s(m_szButtonName, _szButtonName);
}

template<typename T>
inline CUI_Button<T>::~CUI_Button()
{
}

template<typename T>
int CUI_Button<T>::Update(float _fDeltaTime)
{
	POINT pt = {};
	GetCursorPos(&pt);				// 데스크탑 기준 좌표
	ScreenToClient(g_hWND, &pt);	// 윈도우창 기준 좌표
	if ((pt.x > GetLeft() && GetRight() > pt.x) && (pt.y > GetTop() && GetBottom() > pt.y))
	{
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
			if (m_pOwner && m_pEvent) (m_pOwner->*m_pEvent)();
		}
	}
	return 0;
}

template<typename T>
void CUI_Button<T>::Render(const HDC & _hdc, CCamera2D * _pCamera)
{
	CObj::Render(_hdc);
	TextOut(_hdc, GetX() + 2, GetY() + 2, m_szButtonName, lstrlen(m_szButtonName));
}

