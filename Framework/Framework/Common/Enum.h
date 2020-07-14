#pragma once

namespace OBJ {
	enum E_DIRECTION { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_END };
	enum E_COLLIDED { COL_LEFT, COL_RIGHT, COL_TOP, COL_BOTTOM, COL_END };
}

namespace UI {
	enum E_GAUGE { GAUGE_VERTICAL, GAUGE_HORIZONTAL, GAUGE_END };
}

namespace TIMER {
	enum E_TYPE { TIMER_TYPE_TIME, TIMER_TYPE_TICKCOUNT, TIMER_TYPE_WINDOWS, TIMER_TYPE_END };
}

namespace KEY {
	enum E_KEY { KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LBUTTON, KEY_RBUTTON, KEY_Q, KEY_E, KEY_W, KEY_A, KEY_S, KEY_D, KEY_END };
}

namespace MAP_EDITOR {
	enum E_LAYER { LAYER_DRAW, LAYER_COLLISION, LAYER_TRIGGER, LAYER_DOOR, LAYER_END };
	enum E_TOOL { TOOL_PAINT, TOOL_ERASE, TOOL_END };
}

namespace MAP_OBJ {
	enum E_TYPE { TYPE_ATLAS_OBJ, TYPE_COLLIDER, TYPE_TRIGGER, LAYER_DOOR, TYPE_END };
}

namespace ATLAS_LOADER {
	// UNIFORM : Ÿ��ó�� ���� ���� ���̰� ������ ������Ʈ�� ���� ��Ʋ���� ���
	// NON_UNIFORM : Ÿ�� �̿� ���� ���� ���̰� �������� ���� ������Ʈ�� ���� ��Ʋ���� ���
	enum E_TYPE { TYPE_UNIFORM, TYPE_NON_UNIFORM, TYPE_END};
}

namespace COLLIDER {
	enum E_TYPE { TYPE_RECT, TYPE_CIRCLE, TYPE_END};
}