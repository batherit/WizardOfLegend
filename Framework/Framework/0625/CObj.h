#pragma once

class CGameWorld;
class CCamera2D;

class CObj abstract
{
public:
	CObj(CGameWorld& _rGameWorld);
	CObj(CGameWorld& _rGameWorld, 
		float _fX = 0.f, float _fY = 0.f, size_t _iWidth = 10, size_t _iHeight = 10, 
		float _fToX = 0.f, float _fToY = -1.f, float _fSpeed = 0.f, 
		BOOL(__stdcall *_pDrawFunc) (HDC hdc, int _left, int _right, int _top, int _bottom) = Rectangle);
	virtual ~CObj();

public: 
	virtual void LoadInfo(FILE* fpIn) {};
	virtual void SaveInfo(FILE* fpOut) {};
	virtual void Ready(void) {};
	virtual int Update(float _fDeltaTime) { return 0; };
	virtual void LateUpdate(void) {};
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera = nullptr);
	virtual void Release(void) {};
	virtual void LoadMapData(FILE* _fpIn);

public:
	CGameWorld& GetGameWorld(void) const { return m_rGameWorld; }

public:
	void MoveTo(float _fDeltaX, float _fDeltaY) { m_fX += _fDeltaX; m_fY += _fDeltaY; }
	void MoveByDeltaTime(float _fDeltaTime) { m_fX += (m_fToX * m_fSpeed * _fDeltaTime); m_fY += (m_fToY * m_fSpeed * _fDeltaTime); }
	void SetX(float _fX) { m_fX = _fX; }
	void SetY(float _fY) { m_fY = _fY; }
	void SetXY(float _fX, float _fY) { SetX(_fX); SetY(_fY); }
	void SetToX(float _fToX) { m_fToX = _fToX; }
	void SetToY(float _fToY) { m_fToY = _fToY; }
	void SetToXY(float _fToX, float _fToY) { NormalizeVector(_fToX, _fToY); SetToX(_fToX); SetToY(_fToY); }
	float GetX(void) const { return m_fX; }
	float GetY(void) const { return m_fY; }
	pair<float, float> GetXY(void) const { return make_pair(GetX(), GetY()); }
	float GetToX(void) const { return m_fToX; }
	float GetToY(void) const { return m_fToY; }
	pair<float, float> GetToXY(void) const { return make_pair(GetToX(), GetToY()); }
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
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetValid(bool _bIsValid) { m_bIsValid = _bIsValid; }
	bool IsValid(void) const { return m_bIsValid; }
	const int GetGroupID(void) const { return m_iGroupID; }
	//void UpdateAnimation(float _fDeltaTime);
	//void ChangeAnimation(int )


protected:
	int m_iGroupID = -1;
	MAP_OBJ::E_TYPE m_eObjType = MAP_OBJ::TYPE_END;
	_anim_processing_info m_stAnimProcessingInfo;
	bool m_bIsValid = true;
	float m_fX = 0.f;
	float m_fY = 0.f;
	float m_fToX = 0.f;
	float m_fToY = -1.f;
	float m_fSpeed = 0.f;
	size_t m_iWidth = 10;
	size_t m_iHeight = 10;
	BOOL (__stdcall *m_pDrawFunc) (HDC hdc, int _left, int _right, int _top, int _bottom) = Rectangle;

private:
	// CObj는 무조건 GameWorld 객체를 지녀야만 한다.
	CGameWorld& m_rGameWorld;
};

