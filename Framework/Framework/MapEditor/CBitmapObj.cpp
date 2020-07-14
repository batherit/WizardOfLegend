#include "stdafx.h"
#include "CBitmapObj.h"


CBitmapObj::CBitmapObj()
{
}


CBitmapObj::~CBitmapObj()
{
	Release();
}

void CBitmapObj::LoadBmp(const TCHAR * pFilePath)
{
	HDC hDC = GetDC(g_hWND);
	// ��ȭ�� �غ�. 
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWND, hDC);
	// ����� ���� �غ�. 
	m_hBitmap = (HBITMAP)LoadImage(nullptr, pFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// ���Ⱑ �׸��� �׸�. 
	m_OldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CBitmapObj::Release()
{
	// ���õǾ� �ִ� GDI ������Ʈ�� ����� �ִ� ����� ����. 
	// �׷��� ���� �ִ������� ���� ���� �ְ� ���� ����� ���� �����. 
	// �׷��� OldBitmap�̶�³���� �����. 
	SelectObject(m_hMemDC, m_OldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
