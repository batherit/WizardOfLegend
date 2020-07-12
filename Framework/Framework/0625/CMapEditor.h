#pragma once

class CGameWorld;
class CMapObj;
template<typename T> class CUI_Button;
class CCamera2D;
class CAtlasLoader;

class CMapEditor
{
public:
	CMapEditor(CGameWorld& _rGameWorld);
	~CMapEditor();

public:
	void GenerateAtlasFromFile(void);

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
	// Ÿ�ϸ� ������� �ʱ� ���� ���ķ� ������� �ʴ´�.
	void SetMapWidth(size_t _iMapWidth) { m_iMapWidth = _iMapWidth;}
	void SetMapHeight(size_t _iMapHeight) { m_iMapHeight = _iMapHeight; }

public:
	// Button Event
	void ChangeMapWidth(void* _pDeltaWidth);
	void ChangeMapHeight(void* _pDeltaHeight);
	void MoveCameraToMapCenter(void*);

private:
	CGameWorld& m_rGameWorld;

	vector<CUI_Button<CMapEditor>*> m_vecEditorButtons;				// �����Ϳ��� ����ϴ� ��ư��.
	vector<CAtlasLoader*> m_vecAtlasLoaders;

	CMapObj** m_ppMapObjsArr[MAP_EDITOR::LAYER_END];	// �� ������Ʈ��(�߿�!)
	LONG m_lMapLeft = 0;								// ���� �»�� ���� X
	LONG m_lMapTop = 0;								// ���� �»�� ���� Y
	size_t m_iTileWidth = 73;							// ���� �����ϴ� Ÿ���� �ʺ� (�ȼ�)
	size_t m_iTileHeight = 73;							// ���� �����ϴ� Ÿ���� ���� (�ȼ�)
	size_t m_iMapWidth = 10;							// ���η� ���� �� �ִ� ������Ʈ�� �ִ� ��
	size_t m_iMapHeight = 10;							// ���η� ���� �� �ִ� ������Ʈ�� �ִ� ��
};

