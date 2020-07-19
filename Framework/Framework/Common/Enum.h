#pragma once

namespace OBJ {
	enum E_DIRECTION { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_END };
	enum E_COLLIDED { COL_LEFT, COL_RIGHT, COL_TOP, COL_BOTTOM, COL_END };
}

namespace SWORDMAN {
	enum E_DIRECTION { DIR_LEFT, DIR_RIGHT, DIR_END };
	enum E_STATE{ STATE_IDLE, STATE_RUN, STATE_ATTACK, STATE_DAMAGE, STATE_DEATH, STATE_END };
}

namespace UI {
	enum E_GAUGE { GAUGE_VERTICAL, GAUGE_HORIZONTAL, GAUGE_END };
}

namespace TIMER {
	enum E_TYPE { TIMER_TYPE_TIME, TIMER_TYPE_TICKCOUNT, TIMER_TYPE_WINDOWS, TIMER_TYPE_END };
}

namespace KEY {
	enum E_KEY { KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LBUTTON, KEY_RBUTTON, KEY_Q, KEY_E, KEY_W, KEY_A, KEY_S, KEY_D, KEY_LSHIFT, KEY_P, KEY_G, KEY_SPACE, KEY_END };
}

namespace MAP_EDITOR {
	enum E_LAYER { LAYER_DRAW, LAYER_COLLISION, LAYER_TRIGGER, LAYER_DOOR, LAYER_SPAWN_POINT, LAYER_END };
	enum E_TOOL { TOOL_PAINT, TOOL_ERASE, TOOL_END };
}

namespace MAP_OBJ {
	enum E_TYPE { TYPE_ATLAS_OBJ, TYPE_COLLIDER, TYPE_TRIGGER, TYPE_DOOR_HOR, TYPE_DOOR_VER, TYPE_SPAWN_POINT, TYPE_END };
}

namespace ATLAS_LOADER {
	// UNIFORM : 타일처럼 가로 세로 길이가 일정한 오브젝트를 지닌 아틀라스의 경우
	// NON_UNIFORM : 타일 이외 가로 세로 길이가 일정하지 않은 오브젝트를 지닌 아틀라스의 경우
	enum E_TYPE { TYPE_UNIFORM, TYPE_NON_UNIFORM, TYPE_END};
}

namespace COLLIDER {
	enum E_TYPE { TYPE_RECT, TYPE_CIRCLE, TYPE_END};
}

namespace PLAYER {
	enum E_STATE { STATE_IDLE, STATE_RUN, STATE_DASH, STATE_ATTACK1, STATE_ATTACK2, STATE_DAMAGE, STATE_DEATH, STATE_END };
}

namespace EFFECT_SPAWN {
	enum E_TYPE { EFFECT_SPAWN_PLAYER, EFFECT_SPAWN_ARCHER, EFFECT_SPAWN_SWORDMAN, EFFECT_SPAWN_WIZARD, EFFECT_SPAWN_WIZARDBALL, EFFECT_SPAWN_END };
}