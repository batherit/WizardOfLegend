#pragma once
#include "CObj.h"

template<typename T>
class CUI_Button :
	public CObj
{
public:
	CUI_Button(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, const TCHAR* _szButtonName, T* _pOwner = nullptr, void (T::*_pEvent) (void*)= nullptr, void* _pArgu = nullptr);
	~CUI_Button();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;

private:
	T* m_pOwner;
	void(T::*m_pEvent) (void*);
	void* m_pArgu;
	TCHAR m_szButtonName[64];
};

template<typename T>
inline CUI_Button<T>::CUI_Button(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, const TCHAR* _szButtonName, T* _pOwner , void (T::*_pEvent) (void*) , void* _pArgu)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight),
	m_pOwner(_pOwner),
	m_pEvent(_pEvent),
	m_pArgu(_pArgu)
{
	if (_pEvent) m_pEvent = _pEvent;
	wcscpy_s(m_szButtonName, _szButtonName);
}

template<typename T>
inline CUI_Button<T>::~CUI_Button()
{
	DeleteSafe(m_pArgu);
}

template<typename T>
int CUI_Button<T>::Update(float _fDeltaTime)
{
	POINT pt = GetClientCursorPoint();
	if (IsPointInRect(GetRect(), pt))
	{
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
			if (m_pOwner && m_pEvent) (m_pOwner->*m_pEvent)(m_pArgu);
			return 1;
		}
	}
	return 0;
}

template<typename T>
void CUI_Button<T>::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	CObj::Render(_hdc);
	TextOut(_hdc, GetLeft() + 5, GetTop() + 5, m_szButtonName, lstrlen(m_szButtonName));
}

