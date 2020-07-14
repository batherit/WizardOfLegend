#include "stdafx.h"
#include "CTileMapObj.h"


CTileMapObj::CTileMapObj(const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eObjType)
	:
	m_eObjType(_eObjType),
	m_rMapRenderInfo(_rMapRenderInfo),
	m_stPivotPoint(_pivot_point{_iPivotRow, _iPivotCol})
{
}

CTileMapObj::~CTileMapObj()
{
}
