#include "stdafx.h"
#include "CLayer.h"

CLayer::CLayer(const char* _szFileName)
{
	FILE* fp = nullptr;
	
	//TCHAR szDirectory[256] = _T("../SaveFile/");
	//_tcscat_s(szDirectory, sizeof(szDirectory), _szFileName);
	char szDirectory[256] = "../SaveFile/";
	strcat_s(szDirectory, sizeof(szDirectory), _szFileName);

	//errno_t err = _tfopen_s(&fp, szDirectory, _T("rt"));
	errno_t err = fopen_s(&fp, szDirectory, "rt");

	if (!err) {
		int iX, iY;

		vector<pair<INT, INT>> points;
		points.reserve(128);
		while (!feof(fp)) {
			//_ftscanf_s(fp, _T("%d %d"), &iX, &iY);
			fscanf_s(fp, "%d %d", &iX, &iY);
			if (iX == -1 || iY == -1) {
				m_vecDatas.push_back(points);
				points.clear();
				continue;
			}
			points.push_back(make_pair(iX, iY));
		}
		fclose(fp);
	}
}


CLayer::~CLayer()
{
}

void CLayer::Render(const HDC & _hdc)
{
	size_t iSize = m_vecDatas.size();

	for (int i = 0; i < iSize; i++) {
		MoveToEx(_hdc, m_vecDatas[i][0].first, m_vecDatas[i][0].second, nullptr);
		size_t iPointsNum = m_vecDatas[i].size();
		for (int j = 1; j < iPointsNum; j++) {
			LineTo(_hdc, m_vecDatas[i][j].first, m_vecDatas[i][j].second);
		}
	}
}
