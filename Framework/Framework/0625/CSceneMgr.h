#pragma once

class CScene;
class CGameWorld;

class CSceneMgr
{
public:
	CSceneMgr(CGameWorld& _rGameWorld);
	~CSceneMgr();

public:
	bool ConfirmValidScene(void);				// ���� ���� �����Ѵٸ�, �ش������ �����Ѵ�.
	void SetNextScene(CScene* _pNextScene);		// nullptr�� �����ϰų�, �ʱ� ���� ���õ��� ������ ���α׷��� ����ȴ�.
	void RequestSceneInit(void) { m_bInit = false; }
	void Update(float _fDeltaTime);
	void LateUpdate(void);
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);
	CScene* GetCurScene(void) const { return m_pCurScene; }

private:
	CGameWorld& m_rGameWorld;
	CScene* m_pCurScene = nullptr;
	CScene* m_pNextScene = nullptr;
	bool m_bIsConfirmed = true;
	bool m_bInit = true;		// �� �����ڿ��� �ʱ� �̴��� �����ϹǷ� ���ô� true
};

