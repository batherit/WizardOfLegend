#pragma once

class CGameWorld;
class CEditor_Obj;
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
	void RenderCursorInfo(HDC& _hdc, CCamera2D* _pCamera);

public:
	void ClearObjs(void);
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
	// 타일맵 사이즈는 초기 설정 이후로 변경되지 않는다.
	void SetMapWidth(size_t _iMapWidth) { m_stMapRenderInfo.stMapStructureInfo.iMapWidth = _iMapWidth;}
	void SetMapHeight(size_t _iMapHeight) { m_stMapRenderInfo.stMapStructureInfo.iMapHeight = _iMapHeight; }

public:
	// Button Event
	void ChangeMapWidth(void* _pDeltaWidth);
	void ChangeMapHeight(void* _pDeltaHeight);
	void MoveMapObjs(void* _pDeltaRowCol);
	void MoveCameraToMapCenter(void*);
	void ToggleAtlas(void* _pID);
	void ChangeLayer(void* _pLayerType);
	void ChangeDrawLayerIndex(void* _pDrawLayerIndex);
	void ChangeTool(void* _pTool);
	void ChangeGroupID(void* _pGroupID);
	void ChangeMapID(void* _pMapID);
	void SaveMap(void*);
	void LoadMap(void*);
	void ChangeDoorType(void* _pDoorType);
	void MakeMapData(void*);
	void ChangeColliderWidth(void* _pColliderDeltaWidth);
	void ChangeColliderHeight(void* _pColliderDeltaHeight);

private:
	CGameWorld& m_rGameWorld;

	vector<CUI_Button<CMapEditor>*> m_vecEditorButtons;			// 에디터에서 사용하는 버튼들.
	bool m_bIsLayerVisible[MAP_EDITOR::LAYER_END];				// TODO : 기능 구현해야함.
	MAP_EDITOR::E_LAYER m_eLayerType = MAP_EDITOR::LAYER_DRAW; 
	MAP_EDITOR::E_TOOL m_eTool = MAP_EDITOR::TOOL_PAINT;
	MAP_OBJ::E_TYPE m_eDoorType = MAP_OBJ::TYPE_DOOR_HOR;
	list<CEditor_Obj*> m_listAtlasObjs[ciMaxDrawLayerNum];						// 아틀라스 오브젝트
	int m_iDrawLayerIndex = 0;									// 아틀라스 오브젝트 드로우 레이어 인덱스
	int m_iGroupID = 0;											// 할당할 트리거 아이디 최대 9개를 할당 할 수 있다. (예정)
	int m_iMapID = 0;											// 하나의 맵에 부여되는 ID 맵에디터는 최대 5개의 아이디를 부여할 수 있다. (예정)
	int m_iColliderWidth = 1;
	int m_iColliderHeight = 1;
	list<CEditor_Obj*> m_listColliders;							// 콜라이더
	list<CEditor_Obj*> m_listTriggers;							// 트리거들
	list<CEditor_Obj*> m_listDoors;								// 도어들
	CEditor_Obj* m_pSpawnPoint = nullptr;
	int m_iVisibleAtlasID = -1;									// 현재 보이는 아틀라스 ID; 
	_atlas_obj_info m_stDetectedAtlasObj;						// 검출된 타일
	_map_render_info m_stMapRenderInfo;							// 맵 렌더에 공유해서 쓰는 정보 맵 오브젝트들은 이를 공유한다.
};

/*
아틀라스 아이디는 파일에서 긁어온 순서대로 0부터 차례로 부여된다.
*/