#pragma once

class CGameWorld;

class CObj //abstract
{
public:
	CObj(CGameWorld& _rGameWorld);
	CObj(CGameWorld& _rGameWorld, float _fX = 0, float _fY = 0, size_t _iWidth = 10, size_t _iHeight = 10, float _fSpeed = 0.f, BOOL(__stdcall *_pDrawFunc) (HDC hdc, int _left, int _right, int _top, int _bottom) = Rectangle);
	virtual ~CObj();

public: 
	virtual void Ready(void) {};
	virtual int Update(float _fDeltaTime) { return 0; };
	virtual void LateUpdate(void) {};
	virtual void Render(const HDC& _hdc) { if(m_pDrawFunc) m_pDrawFunc(_hdc, GetLeft(), GetTop(), GetRight(), GetBottom()); };
	virtual void Release(void) {};

public:
	CGameWorld& GetGameWorld(void) const { return m_rGameWorld; }

public:
	void MoveTo(float _fDeltaX, float _fDeltaY) { m_fX += _fDeltaX; m_fY += _fDeltaY; }
	void SetX(float _fX) { m_fX = _fX; }
	void SetY(float _fY) { m_fY = _fY; }
	void SetXY(float _fX, float _fY) { SetX(_fX); SetY(_fY); }
	float GetX(void) const { return m_fX; }
	float GetY(void) const { return m_fY; }
	pair<float, float> GetXY(void) const { return make_pair(GetX(), GetY()); }
	int GetLeft(void) const { return static_cast<int>(m_fX - (m_iWidth >> 1)); }
	int GetTop(void) const { return static_cast<int>(m_fY - (m_iHeight >> 1)); }
	int GetRight(void) const { return static_cast<int>(m_fX + (m_iWidth >> 1)); }
	int GetBottom(void) const { return static_cast<int>(m_fY + (m_iHeight >> 1)); }
	RECT GetRect(void) const {
		RECT rc = {
		static_cast<LONG>(m_fX - (m_iWidth >> 1)),
		static_cast<LONG>(m_fY - (m_iHeight >> 1)),
		static_cast<LONG>(m_fX + (m_iWidth >> 1)),
		static_cast<LONG>(m_fY + (m_iHeight >> 1))
		};
		return rc;
	}
	void SetWidth(size_t _iWidth) { m_iWidth = _iWidth; }
	void SetHeight(size_t _iHeight) { m_iHeight = _iHeight; }
	size_t GetWidth(void) const { return m_iWidth; }
	size_t GetHeight(void) const { return m_iHeight; }
	float GetSpeed(void) const { return m_fSpeed; }
	void SetValid(bool _bIsValid) { m_bIsValid = _bIsValid; }
	bool IsValid(void) const { return m_bIsValid; }
	/*void SetActive(bool _bIsActive) { m_bIsActive = _bIsActive; }
	bool IsActive(void) { return m_bIsActive; }*/

protected:
	bool m_bIsValid;
	float m_fX;
	float m_fY;
	float m_fSpeed;
	size_t m_iWidth;
	size_t m_iHeight;
	BOOL (__stdcall *m_pDrawFunc) (HDC hdc, int _left, int _right, int _top, int _bottom);

private:
	// CObj는 무조건 GameWorld 객체를 지녀야만 한다.
	CGameWorld& m_rGameWorld;
};

