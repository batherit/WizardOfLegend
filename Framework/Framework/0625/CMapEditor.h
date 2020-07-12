#pragma once

class CGameWorld;
class CMapObj;
template<typename T> class CUI_Button;
class CCamera2D;

class CMapEditor
{
public:
	CMapEditor(CGameWorld& _rGameWorld);
	~CMapEditor();

public:
	void Ready(void);
	void Update(float _fDeltaTime);
	void Render(const HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);
void TestEvent();
private:
	

private:
	CGameWorld& m_rGameWorld;

	vector<CUI_Button<CMapEditor>*> m_vecEditorButtons;				// �����Ϳ��� ����ϴ� ��ư��.

	CMapObj** m_ppMapObjsArr[MAP_EDITOR::LAYER_END];	// �� ������Ʈ��(�߿�!)
	float m_fMapStartX;									// ���� �»�� ���� X
	float m_fMapStartY;									// ���� �»�� ���� Y
	size_t m_iTileWidth;								// ���� �����ϴ� Ÿ���� �ʺ� (�ȼ�)
	size_t m_iTileHeight;								// ���� �����ϴ� Ÿ���� ���� (�ȼ�)
	size_t m_iMapWidth;									// ���η� ���� �� �ִ� ������Ʈ�� �ִ� ��
	size_t m_iMapHeight;								// ���η� ���� �� �ִ� ������Ʈ�� �ִ� ��
};

