#pragma once

struct _atlas_info {
	char szAtlasFileDirectory[256] = "";
	int iAtlasWidth = 100;
	int iAtlasHeight = 100;
	float  fAtlasRatio = 1.f;
	int iTileWidth = 73;
	int iTileHeight = 73;
};