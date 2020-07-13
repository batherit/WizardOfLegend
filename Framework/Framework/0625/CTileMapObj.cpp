#include "stdafx.h"
#include "CTileMapObj.h"


CTileMapObj::CTileMapObj(const _obj_render_info & _rObjRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eObjType)
	:
	m_eObjType(_eObjType),
	m_rObjRenderInfo(_rObjRenderInfo),
	m_stPivotPoint(_pivot_point{_iPivotRow, _iPivotCol})
{
}

CTileMapObj::~CTileMapObj()
{
}
