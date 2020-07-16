#pragma once
#include "CObj.h"

class CGameWorld;

class CMapObjsGroup :
	public CObj
{
public:
	CMapObjsGroup(CGameWorld& _rGameWorld, int _iGroupID);
	virtual ~CMapObjsGroup();

public:
	virtual void Ready(void);
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera = nullptr);
	virtual void Release(void);

public:
	void RenderGroupArea(HDC& _hdc, CCamera2D* _pCamera = nullptr);
	bool RegisterMapObj(CObj* _pObj);
	const vector<CObj*>& GetMapObjs(void) const { return m_vecMapObjs; }
	
private:
	void ReadjustPosAndSize(void);

private:
	vector<CObj*> m_vecMapObjs;
};

