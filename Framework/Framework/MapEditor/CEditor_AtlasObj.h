#pragma once
#include "CEditor_Obj.h"

class CEditor_AtlasObj :
	public CEditor_Obj
{
public:
	CEditor_AtlasObj(const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, const _atlas_obj_info& _rAtlasObjInfo); // �ܺ� ������
	CEditor_AtlasObj(const _map_render_info & _rMapRenderInfo);		// ���� �ε��
	~CEditor_AtlasObj();

public:
	// CTileMapObj��(��) ���� ��ӵ�
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);
	virtual void MakeMapData(FILE* _fpOut);

private:
	_atlas_obj_info m_stAtlasObjInfo;
};

