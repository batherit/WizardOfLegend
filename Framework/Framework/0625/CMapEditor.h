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
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);

public:
	void RenderTileBoard(const HDC& _hdc, CCamera2D* _pCamera);
	void RenderZeroPoint(const HDC& _hdc, CCamera2D* _pCamera);

public:
	LONG GetMapLeft(void) const { return m_lMapLeft; }
	LONG GetMapTop(void) const { return m_lMapTop; }
	LONG GetMapRight(void) const { return m_lMapLeft + m_iMapWidth * m_iTileWidth; }
	LONG GetMapBottom(void) const { return m_lMapTop + m_iMapHeight * m_iTileHeight; }
	RECT GetMapRect(void) const {
		RECT rc = {
			GetMapLeft(),
			GetMapTop(),
			GetMapRight(),
			GetMapBottom()
		};
		return rc;
	}
	LONG GetMapMiddleX(void) const { return (GetMapLeft() + GetMapRight()) >> 1; }
	LONG GetMapMiddleY(void) const { return (GetMapTop() + GetMapBottom()) >> 1; }
	
	size_t GetTileWidth(void) const { return m_iTileWidth; }
	size_t GetTileHeight(void) const { return m_iTileHeight; }
	size_t GetMapWidth(void) const { return m_iMapWidth; }
	size_t GetMapHeight(void) const { return m_iMapHeight; }
	void SetMapLeft(LONG _iMapLeft) { m_lMapLeft = _iMapLeft; }
	void SetMapTop(LONG _iMapTop) { m_lMapTop = _iMapTop; }
	// 타일맵 사이즈는 초기 설정 이후로 변경되지 않는다.
	void SetMapWidth(size_t _iMapWidth) { m_iMapWidth = _iMapWidth;}
	void SetMapHeight(size_t _iMapHeight) { m_iMapHeight = _iMapHeight; }

public:
	// Button Event
	void ChangeMapWidth(void* _pDeltaWidth);
	void ChangeMapHeight(void* _pDeltaHeight);
	void MoveCameraToMapCenter(void*);

private:
	CGameWorld& m_rGameWorld;

	vector<CUI_Button<CMapEditor>*> m_vecEditorButtons;				// 에디터에서 사용하는 버튼들.

	CMapObj** m_ppMapObjsArr[MAP_EDITOR::LAYER_END];	// 맵 오브젝트들(중요!)
	LONG m_lMapLeft = 0;								// 맵의 좌상단 시작 X
	LONG m_lMapTop = 0;								// 맵의 좌상단 시작 Y
	size_t m_iTileWidth = 73;							// 맵을 구성하는 타일의 너비 (픽셀)
	size_t m_iTileHeight = 73;							// 맵을 구성하는 타일의 높이 (픽셀)
	size_t m_iMapWidth = 10;							// 가로로 놓일 수 있는 오브젝트의 최대 수
	size_t m_iMapHeight = 10;							// 세로로 놓일 수 있는 오브젝트의 최대 수
};

