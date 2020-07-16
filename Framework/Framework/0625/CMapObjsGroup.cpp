#include "stdafx.h"
#include "CMapObjsGroup.h"
#include "CCamera2D.h"


CMapObjsGroup::CMapObjsGroup(CGameWorld & _rGameWorld, int _iGroupID)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0, 0)
{
	m_iGroupID = _iGroupID;
}

CMapObjsGroup::~CMapObjsGroup()
{
	Release();
}

void CMapObjsGroup::Ready(void)
{
	for (auto& pObj : m_vecMapObjs) { pObj->Ready(); }
}

int CMapObjsGroup::Update(float _fDeltaTime)
{
	for (auto& pObj : m_vecMapObjs) { pObj->Update(_fDeltaTime); }
	return 0;
}

void CMapObjsGroup::LateUpdate(void)
{
	for (auto& pObj : m_vecMapObjs) { pObj->LateUpdate(); }
}

void CMapObjsGroup::Render(HDC & _hdc, CCamera2D * _pCamera)
{	
	for (auto& pObj : m_vecMapObjs) { pObj->Render(_hdc, _pCamera); }
	RenderGroupArea(_hdc, _pCamera);
}

void CMapObjsGroup::Release(void)
{
	DeleteVectorSafe(m_vecMapObjs);
}

void CMapObjsGroup::RenderGroupArea(HDC & _hdc, CCamera2D * _pCamera)
{
	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);
	// 내부가 빈
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

	// 노란색 선을 지닌
	HPEN hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 0));
	HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

	// 직사각형을 그린다.
	Rectangle(_hdc, pairLeftTop.first, pairLeftTop.second, pairRightBottom.first + 1, pairRightBottom.second + 1);

	SelectObject(_hdc, hOldBrush);
	SelectObject(_hdc, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

bool CMapObjsGroup::RegisterMapObj(CObj * _pObj)
{
	if (_pObj->GetGroupID() != m_iGroupID) return false;
	m_vecMapObjs.emplace_back(_pObj);
	ReadjustPosAndSize();
	return true;
}

void CMapObjsGroup::ReadjustPosAndSize(void)
{
	if (m_vecMapObjs.empty()) return;

	RECT rcGroupRect = m_vecMapObjs[0]->GetRect();

	int iSize = m_vecMapObjs.size();
	RECT rcObj;
	for (int i = 1; i < iSize; i++) {
		rcObj = m_vecMapObjs[i]->GetRect();
		rcGroupRect.left = min(rcGroupRect.left, rcObj.left);
		rcGroupRect.top = min(rcGroupRect.top, rcObj.top);
		rcGroupRect.right = max(rcGroupRect.right, rcObj.right);
		rcGroupRect.bottom = max(rcGroupRect.bottom, rcObj.bottom);
	}

	SetX((rcGroupRect.right + rcGroupRect.left) >> 1);
	SetY((rcGroupRect.bottom + rcGroupRect.top) >> 1);
	SetWidth(rcGroupRect.right - rcGroupRect.left);
	SetHeight(rcGroupRect.bottom - rcGroupRect.top);
}
