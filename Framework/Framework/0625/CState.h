#pragma once
template<typename T>
class CState abstract
{
public:
	CState(T& _rOwner);
	virtual ~CState();

public:
	virtual void OnLoaded(void) = 0;
	virtual int Update(float _fDeltaTime) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void OnExited(void) = 0;
	virtual int AlwaysUpdate(float _fDeltaTime) { return 0; }
	virtual bool IsMutable(void) { return m_fCurCooltime >= m_fEndCooltime; }

	// ��Ÿ�� ����
	float GetCurCooltime(void) const { return m_fCurCooltime; }
	float GetEndCooltime(void) const { return m_fEndCooltime; }
	HDC GetStateHDC(STATE_HDC::E_TYPE _eHDCType) const { return m_hDCState[_eHDCType]; }
	SKILL_COOLTIME::E_TYPE GetCooltimeType(void) const { return m_eCooltimeType; }

protected:
	T& m_rOwner;
	HDC m_hDCState[STATE_HDC::STATE_HDC_END] = { nullptr, };
	float m_fCurCooltime = 0.f;
	float m_fEndCooltime = 0.f;
	SKILL_COOLTIME::E_TYPE m_eCooltimeType = SKILL_COOLTIME::TYPE_NORMAL;
};

template<typename T>
inline CState<T>::CState(T& _rOwner)
	:
	m_rOwner(_rOwner)
{

}

template<typename T>
inline CState<T>::~CState()
{
}
