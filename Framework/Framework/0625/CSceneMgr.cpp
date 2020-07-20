#include "stdafx.h"
#include "CSceneMgr.h"
#include "CScene.h"


CSceneMgr::CSceneMgr(CGameWorld & _rGameWorld)
	:
	m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// ��.��.��. �ʱ⿡�� true�� �������־�� ��.
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

bool CSceneMgr::ConfirmValidScene(void)
{
	if (!m_bIsConfirmed) {				// ���� ���� ��û�� ������ ���
		DeleteSafe(m_pCurScene);		// ���� ���¸� �����. 
		m_pCurScene = m_pNextScene;		// ���� ���¸� ���ο� ���·� ��ü�Ѵ�.
		m_pNextScene = nullptr;			// ���ο� ���� ���� ��ȿȭ
		m_bIsConfirmed = true;			// ���� ���� �Ϸ�!
	}
	if (!m_pCurScene) return false;		// �׷����� ���� ��ȿ�� ���� ���ٸ� false�� ��ȯ
	if (!m_bInit) {
		m_pCurScene->ResetScene();
		m_bInit = true;
	}
	
	return true;						// ��� ���μ����� ����
}

void CSceneMgr::SetNextScene(CScene * _pNextScene)
{
	// ���� ���� �̹� ���õ� ���, �ش� �Լ��� �����Ѵ�.
	// => ���� ���� ������� ���ο� ���·� ������ �� ����.
	// for ���¸� ����ȭ���ֱ� ����.
	if (!m_bIsConfirmed) {
		DeleteSafe(_pNextScene);	// �ش� ��û�� �����Ѵ�.
		return;	// ������ ���� ������ ��û�� �������� ���� ������� ���� ���
	}

	m_pNextScene = _pNextScene;		// ������ ���� ���� ��û�� ���� ���.
	m_bIsConfirmed = false;			// ���� ��û�� ����.
}

void CSceneMgr::Update(float _fDeltaTime)
{
	m_pCurScene->Update(_fDeltaTime);
}

void CSceneMgr::LateUpdate(void)
{
	m_pCurScene->LateUpdate();
}

void CSceneMgr::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	m_pCurScene->Render(_hdc, _pCamera);
}

void CSceneMgr::Release(void)
{
	DeleteSafe(m_pCurScene);
	DeleteSafe(m_pNextScene);
	m_bIsConfirmed = true;
}
