#pragma once
#include "CObj.h"
class CScrewBall final :
	public CObj
{
public:
	CScrewBall(CGameWorld& _rGameWorld, CObj* _pOwner, size_t _iWidth = 20, size_t _iHeight = 20, float _fSpeed = 10.f, float _fDegree = CLAMP_DEGREE(0.f), float _fOffset = 10.f);
	~CScrewBall();

public:
	virtual void Ready(void)				override;
	virtual int Update(float _fDeltaTime)	override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc)	override;
	virtual void Release(void)				override;

public:
	void SetScrewBallOffset(float _fOffset) { m_fScrewBallOffset = _fOffset; }
	void SetScrewBallDegree(float _fDegree) { m_fScrewBallDegree = _fDegree; }

	float GetScrewBallOffset(void) const { return m_fScrewBallOffset; }
	float GetScrewBallDegree(void) const { return m_fScrewBallDegree; }

	float GetScrewBallX(void) const { return GetX() + m_fScrewBallOffset * DIR_X_DEGREE(m_fScrewBallDegree); }
	float GetScrewBallY(void) const { return GetY() + m_fScrewBallOffset * DIR_Y_DEGREE(m_fScrewBallDegree); }

private:
	CObj* m_pOwner;
	float m_fScrewBallOffset;
	float m_fScrewBallDegree;
};
