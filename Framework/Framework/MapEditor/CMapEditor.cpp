#include "stdafx.h"
#include "CMapEditor.h"
#include "UI_Button.h"
#include "CCamera2D.h"
#include "CAtlasLoader.h"
#include "CEditor_Obj.h"
#include "CEditor_AtlasObj.h"
#include "CEditor_Collider.h"
#include "CEditor_Trigger.h"
#include "CEditor_Door.h"
#include "CBitmapMgr.h"


CMapEditor::CMapEditor(CGameWorld& _rGameWorld)
	:
	m_rGameWorld(_rGameWorld)
{
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_TILE_DUNGEON.bmp"), TEXT("WOL_TILE_DUNGEON"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_TILE_HOMETOWN.bmp"), TEXT("WOL_TILE_HOMETOWN"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_OBJECT.bmp"), TEXT("WOL_OBJECT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/PRISON_HOR.bmp"), TEXT("PRISON_HOR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/PRISON_VER.bmp"), TEXT("PRISON_VER"));

		
	// ���Ϸκ��� ��Ʋ�� �δ� ���� (�� ���� ���� ����ü�� �� ����)
	CMapFileMgr::GetInstance()->GenerateAtlasLoadersFromFile("../MapDatas/AtlasLoaders.txt", m_stMapRenderInfo.vecAtlasLoaders);

	// �� ���� ������ ����
	CMapFileMgr::GetInstance()->GenerateMapStructureFromFile("../MapDatas/MapStructure.txt", m_stMapRenderInfo.stMapStructureInfo);

	// �� ���� ��ư
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 1, WINCY - 55, 40, 25, TEXT("W--"), this, &CMapEditor::ChangeMapWidth, new int(-5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 2, WINCY - 55, 40, 25, TEXT("W-"), this, &CMapEditor::ChangeMapWidth, new int(-1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 3, WINCY - 55, 40, 25, TEXT("W+"), this, &CMapEditor::ChangeMapWidth, new int(1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 4, WINCY - 55, 40, 25, TEXT("W++"), this, &CMapEditor::ChangeMapWidth, new int(5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 1, WINCY - 25, 40, 25, TEXT("H--"), this, &CMapEditor::ChangeMapHeight, new int(-5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 2, WINCY - 25, 40, 25, TEXT("H-"), this, &CMapEditor::ChangeMapHeight, new int(-1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 3, WINCY - 25, 40, 25, TEXT("H+"), this, &CMapEditor::ChangeMapHeight, new int(1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 4, WINCY - 25, 40, 25, TEXT("H++"), this, &CMapEditor::ChangeMapHeight, new int(5)));

	// ī�޶� ���� ��ư
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 1, WINCY - 85, 40, 25, TEXT("Cent"), this, &CMapEditor::MoveCameraToMapCenter, nullptr));

	// ��Ʋ�� ������ ��ū ��ư
	TCHAR szBuf[10];
	for (int id = 0; id < m_stMapRenderInfo.vecAtlasLoaders.size(); id++) {
		swprintf_s(szBuf, TEXT("Atlas_%d"), id);
		m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, WINCX - 100, 60 + id * 30, 60, 25, szBuf, this, &CMapEditor::ToggleAtlas, new int(id)));
	}

	// Layer ����
	for (int i = 0; i < ciMaxDrawLayerNum; i++) {
		swprintf_s(szBuf, TEXT("Layer_%d"), i);
		m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 330, WINCY - 25 - 30 * i, 60, 25, szBuf, this, &CMapEditor::ChangeDrawLayerIndex, new int(i)));
	}
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 400, WINCY - 55, 40, 25, TEXT("Draw"), this, &CMapEditor::ChangeLayer, new MAP_EDITOR::E_LAYER(MAP_EDITOR::LAYER_DRAW)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 450, WINCY - 55, 40, 25, TEXT("Col"), this, &CMapEditor::ChangeLayer, new MAP_EDITOR::E_LAYER(MAP_EDITOR::LAYER_COLLISION)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 500, WINCY - 55, 40, 25, TEXT("Trig"), this, &CMapEditor::ChangeLayer, new MAP_EDITOR::E_LAYER(MAP_EDITOR::LAYER_TRIGGER)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 550, WINCY - 55, 40, 25, TEXT("Door"), this, &CMapEditor::ChangeLayer, new MAP_EDITOR::E_LAYER(MAP_EDITOR::LAYER_DOOR)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 650, WINCY - 55, 70, 25, TEXT("Door_H"), this, &CMapEditor::ChangeDoorType, new MAP_OBJ::E_TYPE(MAP_OBJ::LAYER_DOOR_HOR)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 650, WINCY - 25, 70, 25, TEXT("Door_V"), this, &CMapEditor::ChangeDoorType, new MAP_OBJ::E_TYPE(MAP_OBJ::LAYER_DOOR_VER)));

	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 120, (WINCY >> 1) + 95, 25, 25, TEXT("<"), this, &CMapEditor::ChangeGroupID, new int(-1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 160, (WINCY >> 1) + 95, 25, 25, TEXT(">"), this, &CMapEditor::ChangeGroupID, new int(1)));

	// TriggerID ����
	/*int iNewTriggerID = -1;
	for (int i = 0; i < ciMaxTriggerGroupRow; i++) {
		for (int j = 0; j < ciMaxTirggerGroupCol; j++) {
			iNewTriggerID = 3 * i + j;
			swprintf_s(szBuf, TEXT("TrigID_%d"), iNewTriggerID);
			m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 620 + 90 * j, WINCY - 25 - 30 * i, 80, 25, szBuf, this, &CMapEditor::ChangeGroupID, new int(iNewTriggerID)));
		}
	}*/

	// MapID ����
	/*for (int i = 0; i < ciMaxMapNum; i++) {
		swprintf_s(szBuf, TEXT("MapID_%d"), i);
		m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, WINCX - 100, (WINCY >> 1) + i * 30, 80, 25, szBuf, this, &CMapEditor::ChangeMapID, new int(i)));
	}*/

	// Tool ����
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 410, WINCY - 25, 60, 25, TEXT("Paint"), this, &CMapEditor::ChangeTool, new MAP_EDITOR::E_TOOL(MAP_EDITOR::TOOL_PAINT)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 480, WINCY - 25, 60, 25, TEXT("Erase"), this, &CMapEditor::ChangeTool, new MAP_EDITOR::E_TOOL(MAP_EDITOR::TOOL_ERASE)));

	// Save&Load
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, WINCX - 65, (WINCY >> 1) + 170, 50, 25, TEXT("Save"), this, &CMapEditor::SaveMap, nullptr));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, WINCX - 135, (WINCY >> 1) + 170, 50, 25, TEXT("Load"), this, &CMapEditor::LoadMap, nullptr));

	// �� ������ ����
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, WINCX - 100, (WINCY >> 1) + 200, 120, 25, TEXT("Make Map Data"), this, &CMapEditor::MakeMapData, nullptr));

	TO_MAPTOOL(m_rGameWorld).GetCamera()->SetXY(GetMapMiddleX(), GetMapMiddleY());
}


CMapEditor::~CMapEditor()
{
	Release();
}

void CMapEditor::Update(float _fDeltaTime)
{
	CKeyMgr* pKeyMgrInst = CKeyMgr::GetInstance();
	
	// �� �巡�� ����(��Ŭ������ ���� �巡���Ѵ�.)
	if (pKeyMgrInst->IsKeyDown(KEY::KEY_RBUTTON)) {
		pKeyMgrInst->SetOldClickedPoint(GetClientCursorPoint());
	}
	if (pKeyMgrInst->IsKeyPressing(KEY::KEY_RBUTTON)) {
		POINT ptOldClickedPoint = pKeyMgrInst->GetOldClickedPoint();
		POINT ptCurClickedPoint = GetClientCursorPoint();
		CObj* pCamera = TO_MAPTOOL(m_rGameWorld).GetCamera();
		POINT ptDeltaMove = {
			(ptOldClickedPoint.x - ptCurClickedPoint.x) / TO_CAMERA2D(pCamera)->GetZoomMultiple(),
			(ptOldClickedPoint.y - ptCurClickedPoint.y) / TO_CAMERA2D(pCamera)->GetZoomMultiple()
		};
		TO_CAMERA2D(pCamera)->MoveTo(ptDeltaMove.x, ptDeltaMove.y);
		pKeyMgrInst->SetOldClickedPoint(ptCurClickedPoint);
	}
	if (pKeyMgrInst->IsKeyUp(KEY::KEY_RBUTTON)) {
		pKeyMgrInst->SetOldClickedPoint(POINT{0, 0});
	}

	
	if (pKeyMgrInst->IsKeyDown(KEY::KEY_LBUTTON)) {
		if (m_iVisibleAtlasID != -1) {
			// Ŭ���� Ÿ�� ���� ����. (Detect Tile)
			m_stDetectedAtlasObj = m_stMapRenderInfo.vecAtlasLoaders[m_iVisibleAtlasID]->GetDetectedTileRowCol(GetClientCursorPoint());
			m_stMapRenderInfo.vecAtlasLoaders[m_iVisibleAtlasID]->SetVisible(false);
			m_iVisibleAtlasID = -1;
		}
		else {
			// Ÿ�ϸʿ� Ÿ���� ĥ�Ѵ�. (Paint Tile)
			CObj* pCamera = TO_MAPTOOL(m_rGameWorld).GetCamera();
			POINT ptScreenPoint = GetClientCursorPoint();
			// ��ũ�� Ŀ�� ��ǥ -> ���� Ŀ�� ��ǥ ��ȯ
			pair<float, float> pairWorldPoint = TO_CAMERA2D(pCamera)->GetWorldPoint(ptScreenPoint.x, ptScreenPoint.y);
			POINT ptWorldPoint;
			ptWorldPoint.x = pairWorldPoint.first;
			ptWorldPoint.y = pairWorldPoint.second;

			if (IsPointInRect(GetMapRect(), ptWorldPoint)) {
				// Ŀ�� ��ǥ�� �� ������ ���� ���.
				int iCol = pairWorldPoint.first / GetTileWidth();
				int iRow = pairWorldPoint.second / GetTileHeight();

				switch (m_eLayerType)
				{
				case MAP_EDITOR::LAYER_DRAW:
				{
					CEditor_Obj* pPickedObj = nullptr;
					for (auto& obj : m_listAtlasObjs[m_iDrawLayerIndex]) {
						if (IsPointInRect(obj->GetRowColRect(), POINT{ iCol, iRow })) {
							pPickedObj = obj;
							break;
						}
					}
					switch (m_eTool)
					{
					case MAP_EDITOR::TOOL_PAINT:
						if (!pPickedObj && m_stDetectedAtlasObj.iAtlasID >= 0) {
							CEditor_Obj* pObj = new CEditor_AtlasObj(m_stMapRenderInfo, iRow, iCol, m_stDetectedAtlasObj);
							pObj->SetGroupID(m_iGroupID);
							m_listAtlasObjs[m_iDrawLayerIndex].emplace_back(pObj);
						}
						break;
					case MAP_EDITOR::TOOL_ERASE:
						if (pPickedObj) {
							m_listAtlasObjs[m_iDrawLayerIndex].erase(find(m_listAtlasObjs[m_iDrawLayerIndex].begin(), m_listAtlasObjs[m_iDrawLayerIndex].end(), pPickedObj));
							DeleteSafe(pPickedObj);
						}
						break;
					default:
						break;
					}
				break;
				}
				case MAP_EDITOR::LAYER_COLLISION:
				{
					CEditor_Obj* pPickedObj = nullptr;
					for (auto& obj : m_listColliders) {
						if (IsPointInRect(obj->GetRowColRect(), POINT{ iCol, iRow })) {
							pPickedObj = obj;
							break;
						}
					}
					switch (m_eTool)
					{
					case MAP_EDITOR::TOOL_PAINT:
						if (!pPickedObj)
						{
							CEditor_Obj* pObj = new CEditor_Collider(m_stMapRenderInfo, iRow, iCol);
							pObj->SetGroupID(m_iGroupID);
							m_listColliders.emplace_back(pObj);
						}
						break;
					case MAP_EDITOR::TOOL_ERASE:
						if (pPickedObj) {
							m_listColliders.erase(find(m_listColliders.begin(), m_listColliders.end(), pPickedObj));
							DeleteSafe(pPickedObj);
						}	
						break;
					default:
						break;
					}
				break;
				}
				case MAP_EDITOR::LAYER_TRIGGER:
				{
					CEditor_Obj* pPickedObj = nullptr;
					for (auto& obj : m_listTriggers) {
						if (IsPointInRect(obj->GetRowColRect(), POINT{ iCol, iRow })) {
							pPickedObj = obj;
							break;
						}
					}
					switch (m_eTool)
					{
					case MAP_EDITOR::TOOL_PAINT:
						if (!pPickedObj)
						{
							CEditor_Obj* pObj = new CEditor_Trigger(m_stMapRenderInfo, iRow, iCol);
							pObj->SetGroupID(m_iGroupID);
							m_listTriggers.emplace_back(pObj);
						}
						break;
					case MAP_EDITOR::TOOL_ERASE:
						if (pPickedObj) {
							m_listTriggers.erase(find(m_listTriggers.begin(), m_listTriggers.end(), pPickedObj));
							DeleteSafe(pPickedObj);
						}
						break;
					default:
						break;
					}
					break;
				}
				case MAP_EDITOR::LAYER_DOOR:
				{
					CEditor_Obj* pPickedObj = nullptr;
					for (auto& obj : m_listDoors) {
						if (IsPointInRect(obj->GetRowColRect(), POINT{ iCol, iRow })) {
							pPickedObj = obj;
							break;
						}
					}
					switch (m_eTool)
					{
					case MAP_EDITOR::TOOL_PAINT:
						if (!pPickedObj)
						{
							CEditor_Obj* pObj = new CEditor_Door(m_stMapRenderInfo, iRow, iCol, m_eDoorType);
							pObj->SetGroupID(m_iGroupID);
							m_listDoors.emplace_back(pObj);
						}
						break;
					case MAP_EDITOR::TOOL_ERASE:
						if (pPickedObj) {
							m_listDoors.erase(find(m_listDoors.begin(), m_listDoors.end(), pPickedObj));
							DeleteSafe(pPickedObj);
						}
						break;
					default:
						break;
					}
					break;
				}
				default:
					break;
				}
			}
		}
	}

	for (auto& pButton : m_vecEditorButtons) {
		pButton->Update(_fDeltaTime);
	}
}

void CMapEditor::Render(HDC & _hdc, CCamera2D* _pCamera)
{
	RenderTileBoard(_hdc, _pCamera);
	for (int i = 0; i < ciMaxDrawLayerNum; i++) {
		for (auto& pObj : m_listAtlasObjs[i]) {
			pObj->Render(_hdc, _pCamera);
		}
	}
	for (auto& pObj : m_listColliders) {
		pObj->Render(_hdc, _pCamera);
	}
	for (auto& pObj : m_listTriggers) {
		pObj->Render(_hdc, _pCamera);
	}
	for (auto& pObj : m_listDoors) {
		pObj->Render(_hdc, _pCamera);
	}

	RenderZeroPoint(_hdc, _pCamera);
	for (auto& pButton : m_vecEditorButtons) {
		pButton->Render(_hdc, nullptr);
	}
	RenderMode(_hdc, _pCamera);
	for (auto& pAtlas : m_stMapRenderInfo.vecAtlasLoaders) {
		pAtlas->Render(_hdc, _pCamera);
	}
	RenderDetectedTile(_hdc, _pCamera);
}

void CMapEditor::Release(void)
{
	ClearObjs();
	for (auto& pButton : m_vecEditorButtons) {
		DeleteSafe(pButton);
	}
	m_vecEditorButtons.clear();
	m_vecEditorButtons.shrink_to_fit();
}

void CMapEditor::RenderTileBoard(HDC & _hdc, CCamera2D * _pCamera)
{
	pair<float, float> pairConvPoint;
	// ������ �׸���
	for (int i = 0; i < m_stMapRenderInfo.stMapStructureInfo.iMapHeight + 1; i++) {
		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft(), GetMapTop() + GetTileHeight() * i);
		MoveToEx(_hdc, pairConvPoint.first, pairConvPoint.second, nullptr);

		pairConvPoint = _pCamera->GetScreenPoint(GetMapRight(), GetMapTop() + GetTileHeight() * i);
		LineTo(_hdc, pairConvPoint.first, pairConvPoint.second);
	}

	// ������ �׸���
	for (int i = 0; i < m_stMapRenderInfo.stMapStructureInfo.iMapWidth + 1; i++) {
		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft() + GetTileWidth() * i, GetMapTop());
		MoveToEx(_hdc, pairConvPoint.first, pairConvPoint.second, nullptr);

		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft() + GetTileWidth() * i, GetMapBottom());
		LineTo(_hdc, pairConvPoint.first, pairConvPoint.second);
	}
}

void CMapEditor::RenderZeroPoint(HDC & _hdc, CCamera2D * _pCamera)
{
	pair<float, float> pairScreenPoint = _pCamera->GetScreenPoint(0.f, 0.f);

	Ellipse(_hdc, 
		pairScreenPoint.first - (GetTileWidth() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.second - (GetTileHeight() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.first + (GetTileWidth() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.second + (GetTileHeight() >> 2) * _pCamera->GetZoomMultiple());
}

void CMapEditor::RenderDetectedTile(HDC & _hdc, CCamera2D * _pCamera)
{
	// ����� Ÿ���� ���ٸ� �׸��� �ʴ´�.
	if (m_stDetectedAtlasObj.iAtlasID < 0) return;

	// ����� Ÿ���� �ʺ�� ����
	_atlas_loader_info stAtlasInfo = m_stMapRenderInfo.vecAtlasLoaders[m_stDetectedAtlasObj.iAtlasID]->GetAtlasInfo();

	// ����� Ÿ�� ��� ��ǥ
	int fX = WINCX - GetTileWidth() - 30;
	int fY = WINCY - GetTileHeight() - 30;

	StretchBlt(_hdc,
		fX,			// ��� ������ǥ X
		fY,			// ��� ������ǥ Y
		m_stMapRenderInfo.stMapStructureInfo.iTileWidth,					
		m_stMapRenderInfo.stMapStructureInfo.iTileHeight,				
		m_stMapRenderInfo.vecAtlasLoaders[m_stDetectedAtlasObj.iAtlasID]->GetMemDC(),
		m_stDetectedAtlasObj.rcOutputArea.left,
		m_stDetectedAtlasObj.rcOutputArea.top,
		m_stDetectedAtlasObj.rcOutputArea.right - m_stDetectedAtlasObj.rcOutputArea.left,
		m_stDetectedAtlasObj.rcOutputArea.bottom - m_stDetectedAtlasObj.rcOutputArea.top,
		SRCCOPY);
}

void CMapEditor::RenderMode(HDC & _hdc, CCamera2D * _pCamera)
{
	const static TCHAR* szLayer[MAP_EDITOR::LAYER_END] = { TEXT("Draw Layer"), TEXT("Col Layer"), TEXT("Trig Layer"), TEXT("Door Layer") };
	const static TCHAR* szTool[MAP_EDITOR::TOOL_END] = { TEXT("Paint"), TEXT("Erase") };

	TCHAR szMode[256];
	swprintf_s(szMode, TEXT("[%s_%s Mode]"), szTool[m_eTool], szLayer[m_eLayerType]);
	TextOut(_hdc, 30, WINCY >> 1 , szMode, lstrlen(szMode));
	swprintf_s(szMode, TEXT("MapID : %d"), m_iMapID);
	TextOut(_hdc, 30, (WINCY >> 1) + 30, szMode, lstrlen(szMode));
	swprintf_s(szMode, TEXT("DrawLayerIdx : %d"), m_iDrawLayerIndex);
	TextOut(_hdc, 30, (WINCY >> 1) + 60, szMode, lstrlen(szMode));
	swprintf_s(szMode, TEXT("Group : %d"), m_iGroupID);
	TextOut(_hdc, 30, (WINCY >> 1) + 90, szMode, lstrlen(szMode));
	swprintf_s(szMode, TEXT("Door : %c"), (m_eDoorType == MAP_OBJ::LAYER_DOOR_HOR ? 'H' : 'V'));
	TextOut(_hdc, 30, (WINCY >> 1) + 120, szMode, lstrlen(szMode));
}

void CMapEditor::ClearObjs(void)
{
	for (int i = 0; i < ciMaxDrawLayerNum; i++) {
		DeleteListSafe(m_listAtlasObjs[i]);
	}
	DeleteListSafe(m_listColliders);
	DeleteListSafe(m_listTriggers);
	DeleteListSafe(m_listDoors);
}

void CMapEditor::ChangeMapWidth(void* _pDeltaWidth)
{
	int iDeltaWidth = GetMapWidth() + *static_cast<int*>(_pDeltaWidth);
	SetMapWidth(Clamp(iDeltaWidth, 1, 1000));
}

void CMapEditor::ChangeMapHeight(void* _pDeltaHeight)
{
	int iDeltaHeight = GetMapHeight() + *static_cast<int*>(_pDeltaHeight);
	SetMapHeight(Clamp(iDeltaHeight, 1, 1000));
}

void CMapEditor::MoveCameraToMapCenter(void *)
{
	TO_MAPTOOL(m_rGameWorld).GetCamera()->SetXY(GetMapMiddleX(), GetMapMiddleY());
}

void CMapEditor::ToggleAtlas(void * _pID)
{
	int iID = *static_cast<int*>(_pID);

	bool bIsVisible = m_stMapRenderInfo.vecAtlasLoaders[iID]->IsVisible();

	for (int i = 0; i < m_stMapRenderInfo.vecAtlasLoaders.size(); i++) {
		m_stMapRenderInfo.vecAtlasLoaders[i]->SetVisible(false);
	}
	
	if (bIsVisible) {
		m_stMapRenderInfo.vecAtlasLoaders[iID]->SetVisible(false);
		m_iVisibleAtlasID = -1;			// ���� �������� �ִ� ��Ʋ�󽺰� ����.
	}
	else {
		m_stMapRenderInfo.vecAtlasLoaders[iID]->SetVisible(true);
		m_iVisibleAtlasID = iID;		// ���� �������� �ִ� ��Ʋ�� ���̵� ���. => Ÿ�� ���⿡ ����.
	}
}

void CMapEditor::ChangeLayer(void * _pLayerType)
{
	m_eLayerType = *static_cast<MAP_EDITOR::E_LAYER*>(_pLayerType);
}

void CMapEditor::ChangeDrawLayerIndex(void * _pDrawLayerIndex)
{
	m_iDrawLayerIndex = *static_cast<int*>(_pDrawLayerIndex);
}

void CMapEditor::ChangeTool(void * _pTool)
{
	m_eTool = *static_cast<MAP_EDITOR::E_TOOL*>(_pTool);
}

void CMapEditor::ChangeGroupID(void * _pDeltaGroupID)
{
	m_iGroupID += *static_cast<int*>(_pDeltaGroupID);
	if (m_iGroupID < 0) {
		m_iGroupID = 0;
	}
}

void CMapEditor::ChangeMapID(void * _pMapID)
{
	m_iMapID = *static_cast<int*>(_pMapID);
}

void CMapEditor::SaveMap(void *)
{
	FILE* fpOut = nullptr;
	errno_t err = fopen_s(&fpOut, "../MapDatas/Maps/0/Editor_Map.txt", "wt");
	if (!err) {
		int iSize = 0;
		for (int i = 0; i < ciMaxDrawLayerNum; i++) {
			iSize = m_listAtlasObjs[i].size();
			fprintf_s(fpOut, "%d \n", iSize);
			for (auto& obj : m_listAtlasObjs[i]) {
				obj->SaveInfo(fpOut);
			}
		}
		iSize = m_listColliders.size();
		fprintf_s(fpOut, "%d \n", iSize);
		for (auto& obj : m_listColliders) {
			obj->SaveInfo(fpOut);
		}
		iSize = m_listTriggers.size();
		fprintf_s(fpOut, "%d \n", iSize);
		for (auto& obj : m_listTriggers) {
			obj->SaveInfo(fpOut);
		}
		iSize = m_listDoors.size();
		fprintf_s(fpOut, "%d \n", iSize);
		for (auto& obj : m_listDoors) {
			obj->SaveInfo(fpOut);
		}
	}
	if (fpOut) fclose(fpOut);
}

void CMapEditor::LoadMap(void *)
{
	ClearObjs();

	FILE* fpIn = nullptr;
	errno_t err = fopen_s(&fpIn, "../MapDatas/Maps/0/Editor_Map.txt", "rt");
	if (!err) {
		int iSize = 0;
		CEditor_Obj* pObj = nullptr;
		for (int i = 0; i < ciMaxDrawLayerNum; i++) {
			fscanf_s(fpIn, " %d", &iSize);
			for (int j = 0; j < iSize; j++) {
				pObj = new CEditor_AtlasObj(m_stMapRenderInfo);
				pObj->LoadInfo(fpIn);
				m_listAtlasObjs[i].emplace_back(pObj);
			}
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CEditor_Collider(m_stMapRenderInfo);
			pObj->LoadInfo(fpIn);
			m_listColliders.emplace_back(pObj);
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CEditor_Trigger(m_stMapRenderInfo);
			pObj->LoadInfo(fpIn);
			m_listTriggers.emplace_back(pObj);
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CEditor_Door(m_stMapRenderInfo);
			pObj->LoadInfo(fpIn);
			m_listDoors.emplace_back(pObj);
		}
	}
	if (fpIn) fclose(fpIn);
}

void CMapEditor::ChangeDoorType(void * _pDoorType)
{
	m_eDoorType = *static_cast<MAP_OBJ::E_TYPE*>(_pDoorType);
}

void CMapEditor::MakeMapData(void *)
{
	FILE* fpOut = nullptr;
	errno_t err = fopen_s(&fpOut, "../MapDatas/Maps/0/Game_Map.txt", "wt");
	if (!err) {
		int iSize = 0;
		for (int i = 0; i < ciMaxDrawLayerNum; i++) {
			iSize = m_listAtlasObjs[i].size();
			fprintf_s(fpOut, "%d \n", iSize);
			for (auto& obj : m_listAtlasObjs[i]) {
				obj->MakeMapData(fpOut);
			}
		}
		iSize = m_listColliders.size();
		fprintf_s(fpOut, "%d \n", iSize);
		for (auto& obj : m_listColliders) {
			obj->MakeMapData(fpOut);
		}
		iSize = m_listTriggers.size();
		fprintf_s(fpOut, "%d \n", iSize);
		for (auto& obj : m_listTriggers) {
			obj->MakeMapData(fpOut);
		}
		iSize = m_listDoors.size();
		fprintf_s(fpOut, "%d \n", iSize);
		for (auto& obj : m_listDoors) {
			obj->MakeMapData(fpOut);
		}
	}
	if (fpOut) fclose(fpOut);
}


