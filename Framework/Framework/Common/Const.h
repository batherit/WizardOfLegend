#pragma once

// DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_END
const char ccDirKey[OBJ::DIR_END] = { 'A', 'D', 'W', 'S' };
const float cfDeltaX[OBJ::DIR_END] = { -1.f, 1.f, 0.f, 0.f };
const float cfDeltaY[OBJ::DIR_END] = { 0.f, 0.f, -1.f, 1.f };

// �÷��̾� ����
const size_t ciPlayerSize = 50;
const float cfPlayerSpeed = 100.f;

// �Ѿ� ����
const size_t ciBulletSize = 8;
const float cfBulletSpeed = 5.f;
const float cfBulletDamage = 5.f;

// ���� ����
const size_t ciMonsterSize = 40;
const float cfMonsterSpeed = 1.f;
const float cfMonsterHp = 200.f;

// Ű���� ���� (const Ű���� ���� ������ ���װ� ���µ� �� �׷��� �𸣰ڴ�.)
const DWORD cdwKey[KEY::KEY_END] = { VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_LBUTTON, VK_RBUTTON, 'Q', 'E', 'W', 'A', 'S', 'D' };
const DWORD cdwKeyMask[KEY::KEY_END] = { KM_LEFT, KM_RIGHT, KM_UP, KM_DOWN, KM_LBUTTON, KM_RBUTTON, KM_Q, KM_E, KM_W, KM_A, KM_S, KM_D };

// �ʿ����� ����
const size_t ciMaxDrawLayerNum = 3;
const size_t ciMaxMapNum = 5;
const size_t ciMaxTriggerGroupRow = 3;
const size_t ciMaxTirggerGroupCol = 3;