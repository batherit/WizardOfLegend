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

protected:
	T& m_rOwner;
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
