#include "stdafx.h"
#include "CEditor_Obj.h"


CEditor_Obj::CEditor_Obj(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eObjType)
	:
	m_rGameWorld(_rGameWorld),
	m_eObjType(_eObjType),
	m_rMapRenderInfo(_rMapRenderInfo),
	m_stPivotPoint(_pivot_point{_iPivotRow, _iPivotCol})
{
}

CEditor_Obj::CEditor_Obj(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo)
	:
	m_rGameWorld(_rGameWorld),
	m_rMapRenderInfo(_rMapRenderInfo)
{
}

CEditor_Obj::~CEditor_Obj()
{
}

void CEditor_Obj::SaveInfo(FILE * _fpOut) {
	fprintf_s(_fpOut, "%d %d %d %d %d \n", m_eObjType, m_iGroupID, m_iDrawLayer, m_stPivotPoint.iRow, m_stPivotPoint.iCol);
}

void CEditor_Obj::LoadInfo(FILE * _fpIn)
{
	fscanf_s(_fpIn, " %d %d %d %d %d", &m_eObjType, &m_iGroupID, &m_iDrawLayer, &m_stPivotPoint.iRow, &m_stPivotPoint.iCol);
}

void CEditor_Obj::MakeMapData(FILE * _fpOut)
{
	fprintf_s(_fpOut, "%d %d \n", m_eObjType, m_iGroupID);
}
