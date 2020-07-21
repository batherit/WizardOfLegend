#pragma once

// DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_END
const char ccDirKey[OBJ::DIR_END] = { 'A', 'D', 'W', 'S' };
const float cfDeltaX[OBJ::DIR_END] = { -1.f, 1.f, 0.f, 0.f };
const float cfDeltaY[OBJ::DIR_END] = { 0.f, 0.f, -1.f, 1.f };

// 플레이어 변수
const size_t ciPlayerSize = 50;
const float cfPlayerRunSpeed = 610.f; // 1.55초에 13개의 타일(73*73)을 지나가는 것에 근거. 
const float cfPlayerDashSpeed = 2350.f;	// 0.25초에 6개의 타일을 지나가는 것에 근거
const float cfPlayerAttackSpeed = 300.f;
const float cfPlayerKnockbackSpeed = 250.f;
const float cfPlayerMaxHp = 500.f;
const float cfPlayerMaxMana = 100.f;
const float cfPlayerNormalAttackDist = 120.f;

// 스워드맨 변수
const float cfSwordManRunSpeed = 300.f;
const float cfSwordManMaxHp = 80.f;
const float cfSwordManAttackableRange = 80.f;
const float cfSwordManNormalAttackDist = 120.f;
const float cfSwordManKnockbackSpeed = 250.f;

// 아쳐 변수
// => 매크로로 이제 하려고 한다. +ㅅ+!

// 총알 변수
const size_t ciBulletSize = 8;
const float cfBulletSpeed = 5.f;
const float cfBulletDamage = 5.f;

// 몬스터 변수
const size_t ciMonsterSize = 40;
const float cfMonsterSpeed = 1.f;
const float cfMonsterHp = 200.f;

// 키보드 변수 (const 키워드 빼면 재정의 버그가 나는데 왜 그런지 모르겠다.)
const DWORD cdwKey[KEY::KEY_END] = { VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_LBUTTON, VK_RBUTTON, 'Q', 'E', 'W', 'A', 'S', 'D', VK_LSHIFT, 'P', 'G', VK_SPACE };
const DWORD cdwKeyMask[KEY::KEY_END] = { KM_LEFT, KM_RIGHT, KM_UP, KM_DOWN, KM_LBUTTON, KM_RBUTTON, KM_Q, KM_E, KM_W, KM_A, KM_S, KM_D, KM_LSHIFT, KM_P, KM_G, KM_SPACE };

// 맵에디터 변수
const size_t ciMaxDrawLayerNum = 3;
const size_t ciMaxMapNum = 5;
const size_t ciMaxTriggerGroupRow = 3;
const size_t ciMaxTirggerGroupCol = 3;
