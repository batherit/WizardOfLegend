#pragma once
#include "CState.h"

template<typename T>
class CStateMgr
{
public:
	CStateMgr(CGameWorld& _rGameWorld, T& _rOwner);
	~CStateMgr();

public:
	bool ConfirmValidState(void);				// ���� ���°� �����Ѵٸ�, �� �������� �����Ѵ�.
	void SetNextState(CState<T>* _pNextScene, bool _bIsForced = false);
	void Update(float _fDeltaTime);
	void LateUpdate(void);
	void Release(void);

private:
	T& m_rOwner;
	CGameWorld& m_rGameWorld;
	CState<T>* m_pCurState = nullptr;
	CState<T>* m_pNextState = nullptr;
	bool m_bIsConfirmed = true;
};

template<typename T>
CStateMgr<T>::CStateMgr(CGameWorld & _rGameWorld, T& _rOwner)
	:
	m_rOwner(_rOwner),
	m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// ��.��.��. �ʱ⿡�� true�� �������־�� ��.
{
}

template<typename T>
inline CStateMgr<T>::~CStateMgr()
{
	Release();
}

template<typename T>
bool CStateMgr<T>::ConfirmValidState(void)
{
	if (!m_bIsConfirmed) {				// ���� ���� ��û�� ������ ���
		if(m_pCurState) m_pCurState->OnExited();		// ���ֱ� �� ���¸� �����Ѵ�.
		DeleteSafe(m_pCurState);		// ���� ���¸� �����. 
		m_pCurState = m_pNextState;		// ���� ���¸� ���ο� ���·� ��ü�Ѵ�.
		m_pCurState->OnLoaded();		// ���� �غ�
		m_pNextState = nullptr;			// ���ο� ���� ���� ��ȿȭ
		m_bIsConfirmed = true;			// ���� ���� �Ϸ�!
	}
	if (!m_pCurState) return false;		// �׷����� ���� ��ȿ�� ���� ���ٸ� false�� ��ȯ
	return true;						// ��� ���μ����� ����
}

template<typename T>
void CStateMgr<T>::SetNextState(CState<T> * _pNextScene, bool _bIsForced)
{
	// ���� ���� �̹� ���õ� ���, �ش� �Լ��� �����Ѵ�.
	// => ���� ���� ������� ���ο� ���·� ������ �� ����.
	// for ���¸� ����ȭ���ֱ� ����.
	if (!m_bIsConfirmed && !_bIsForced) {
		DeleteSafe(_pNextScene);	// ���� ���´� �����Ѵ�.
		return;						// ������ ���� ������ ��û�� �������� ���� ������� ���� ���
	}

	m_pNextState = _pNextScene;		// ������ ���� ���� ��û�� ���� ���.
	m_bIsConfirmed = false;			// ���� ��û�� ����.
}

template<typename T>
void CStateMgr<T>::Update(float _fDeltaTime)
{
	if(m_pCurState) m_pCurState->Update(_fDeltaTime);
}

template<typename T>
void CStateMgr<T>::LateUpdate(void)
{
	if(m_pCurState) m_pCurState->LateUpdate();
}

template<typename T>
void CStateMgr<T>::Release(void)
{
	DeleteSafe(m_pCurState);
	DeleteSafe(m_pNextState);
	m_bIsConfirmed = true;
}
