// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <ctime>
#include <string>
using namespace std;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Const.h"
#include "Extern.h"
#include "Function.h"
#include "KeyMgr.h"
#include "MainApp.h"
#include "CSceneMgr.h"
#include "CWOL_World.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <io.h>
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")
#include "SoundMgr.h"