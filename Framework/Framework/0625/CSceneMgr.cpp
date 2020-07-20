#include "stdafx.h"
#include "CSceneMgr.h"
#include "CScene.h"


CSceneMgr::CSceneMgr(CGameWorld & _rGameWorld)
	:
	m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// 반.드.시. 초기에는 true로 설정해주어야 함.
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

bool CSceneMgr::ConfirmValidScene(void)
{
	if (!m_bIsConfirmed) {				// 상태 변경 요청이 접수된 경우
		DeleteSafe(m_pCurScene);		// 기존 상태를 지운다. 
		m_pCurScene = m_pNextScene;		// 기존 상태를 새로운 상태로 교체한다.
		m_pNextScene = nullptr;			// 새로운 상태 변수 무효화
		m_bIsConfirmed = true;			// 상태 변경 완료!
	}
	if (!m_pCurScene) return false;		// 그럼에도 현재 유효한 씬이 없다면 false를 반환
	if (!m_bInit) {
		m_pCurScene->ResetScene();
		m_bInit = true;
	}
	
	return true;						// 계속 프로세스를 진행
}

void CSceneMgr::SetNextScene(CScene * _pNextScene)
{
	// 다음 씬이 이미 세팅된 경우, 해당 함수를 무시한다.
	// => 다음 상태 변경까지 새로운 상태로 세팅할 수 없다.
	// for 상태를 동기화해주기 위함.
	if (!m_bIsConfirmed) {
		DeleteSafe(_pNextScene);	// 해당 요청은 삭제한다.
		return;	// 이전에 상태 변경이 요청이 들어왔지만 아직 변경되지 않은 경우
	}

	m_pNextScene = _pNextScene;		// 새로이 상태 변경 요청이 들어온 경우.
	m_bIsConfirmed = false;			// 변경 요청이 들어옴.
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
