#pragma once

class CScene;
class CGameWorld;

class CSceneMgr
{
public:
	CSceneMgr(CGameWorld& _rGameWorld);
	~CSceneMgr();

public:
	bool ConfirmValidScene(void);				// 다음 씬이 존재한다면, 해당씬으로 변경한다.
	void SetNextScene(CScene* _pNextScene);		// nullptr을 전달하거나, 초기 씬이 세팅되지 않으면 프로그램이 종료된다.
	void Update(float _fDeltaTime);
	void LateUpdate(void);
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);

private:
	CGameWorld& m_rGameWorld;
	CScene* m_pCurScene = nullptr;
	CScene* m_pNextScene = nullptr;
	bool m_bIsConfirmed = true;
};

