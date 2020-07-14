#pragma once

class CGameWorld;
class CTileMapObj;
template<typename T> class CUI_Button;
class CCamera2D;
class CAtlasLoader;

class CMapEditor
{
public:
	CMapEditor(CGameWorld& _rGameWorld);
	~CMapEditor();

//public:
//	void GenerateAtlasFromFile(void);

public:
	void Ready(void);
	void Update(float _fDeltaTime);
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);

public:
	void RenderTileBoard(HDC& _hdc, CCamera2D* _pCamera);
	void RenderZeroPoint(HDC& _hdc, CCamera2D* _pCamera);
	void RenderDetectedTile(HDC& _hdc, CCamera2D* _pCamera);
	void RenderMode(HDC& _hdc, CCamera2D* _pCamera);

public:
	LONG GetMapLeft(void) const { return m_stMapRenderInfo.stMapStructureInfo.lMapLeft; }
	LONG GetMapTop(void) const { return m_stMapRenderInfo.stMapStructureInfo.lMapTop; }
	LONG GetMapRight(void) const { return m_stMapRenderInfo.stMapStructureInfo.lMapLeft + m_stMapRenderInfo.stMapStructureInfo.iMapWidth * m_stMapRenderInfo.stMapStructureInfo.iTileWidth; }
	LONG GetMapBottom(void) const { return m_stMapRenderInfo.stMapStructureInfo.lMapTop + m_stMapRenderInfo.stMapStructureInfo.iMapHeight * m_stMapRenderInfo.stMapStructureInfo.iTileHeight; }
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
	
	size_t GetTileWidth(void) const { return m_stMapRenderInfo.stMapStructureInfo.iTileWidth; }
	size_t GetTileHeight(void) const { return m_stMapRenderInfo.stMapStructureInfo.iTileHeight; }
	size_t GetMapWidth(void) const { return m_stMapRenderInfo.stMapStructureInfo.iMapWidth; }
	size_t GetMapHeight(void) const { return m_stMapRenderInfo.stMapStructureInfo.iMapHeight; }
	void SetMapLeft(LONG _iMapLeft) { m_stMapRenderInfo.stMapStructureInfo.lMapLeft = _iMapLeft; }
	void SetMapTop(LONG _iMapTop) { m_stMapRenderInfo.stMapStructureInfo.lMapTop = _iMapTop; }
	// Ÿ�ϸ� ������� �ʱ� ���� ���ķ� ������� �ʴ´�.
	void SetMapWidth(size_t _iMapWidth) { m_stMapRenderInfo.stMapStructureInfo.iMapWidth = _iMapWidth;}
	void SetMapHeight(size_t _iMapHeight) { m_stMapRenderInfo.stMapStructureInfo.iMapHeight = _iMapHeight; }

public:
	// Button Event
	void ChangeMapWidth(void* _pDeltaWidth);
	void ChangeMapHeight(void* _pDeltaHeight);
	void MoveCameraToMapCenter(void*);
	void ToggleAtlas(void* _pID);
	void ChangeLayer(void* _pLayerType);
	void ChangeDrawLayerIndex(void* _pDrawLayerIndex);
	void ChangeTool(void* _pTool);

private:
	CGameWorld& m_rGameWorld;

	vector<CUI_Button<CMapEditor>*> m_vecEditorButtons;			// �����Ϳ��� ����ϴ� ��ư��.
	bool m_bIsLayerVisible[MAP_EDITOR::LAYER_END];				// TODO : ��� �����ؾ���.
	MAP_EDITOR::E_LAYER m_eLayerType = MAP_EDITOR::LAYER_DRAW; 
	MAP_EDITOR::E_TOOL m_eTool = MAP_EDITOR::TOOL_PAINT;
	list<CTileMapObj*> m_listAtlasObjs[3];						// ��Ʋ�� ������Ʈ
	int m_iDrawLayerIndex = 0;									// ��Ʋ�� ������Ʈ ��ο� ���̾� �ε���
	list<CTileMapObj*> m_listColliders;							// �ݶ��̴�
	int m_iVisibleAtlasID = -1;									// ���� ���̴� ��Ʋ�� ID; 
	_atlas_obj_info m_stDetectedAtlasObj;						// ����� Ÿ��
	_map_render_info m_stMapRenderInfo;							// �� ������ �����ؼ� ���� ���� �� ������Ʈ���� �̸� �����Ѵ�.
};

/*
��Ʋ�� ���̵�� ���Ͽ��� �ܾ�� ������� 0���� ���ʷ� �ο��ȴ�.
*/