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

	vector<CUI_Button<CMapEditor>*> m_vecEditorButtons;				// 에디터에서 사용하는 버튼들.

	CMapObj** m_ppMapObjsArr[MAP_EDITOR::LAYER_END];	// 맵 오브젝트들(중요!)
	float m_fMapStartX;									// 맵의 좌상단 시작 X
	float m_fMapStartY;									// 맵의 좌상단 시작 Y
	size_t m_iTileWidth;								// 맵을 구성하는 타일의 너비 (픽셀)
	size_t m_iTileHeight;								// 맵을 구성하는 타일의 높이 (픽셀)
	size_t m_iMapWidth;									// 가로로 놓일 수 있는 오브젝트의 최대 수
	size_t m_iMapHeight;								// 세로로 놓일 수 있는 오브젝트의 최대 수
};

