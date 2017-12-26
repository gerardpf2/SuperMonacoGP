#ifndef _MODULE_WORLD_UI_
#define _MODULE_WORLD_UI_

#define TEXTURE_RECT_ROAD_X 0
#define TEXTURE_RECT_ROAD_Y 0
#define TEXTURE_RECT_ROAD_W WINDOW_WIDTH
#define TEXTURE_RECT_ROAD_H WINDOW_HEIGHT
#define TEXTURE_RECT_ROAD SDL_Rect{ TEXTURE_RECT_ROAD_X, TEXTURE_RECT_ROAD_Y, TEXTURE_RECT_ROAD_W, TEXTURE_RECT_ROAD_H }

#define VIEWPORT_RECT_ROAD_X 0
#define VIEWPORT_RECT_ROAD_Y 0
#define VIEWPORT_RECT_ROAD_W WINDOW_WIDTH
#define VIEWPORT_RECT_ROAD_H WINDOW_HEIGHT
#define VIEWPORT_RECT_ROAD SDL_Rect{ VIEWPORT_RECT_ROAD_X, VIEWPORT_RECT_ROAD_Y, VIEWPORT_RECT_ROAD_W, VIEWPORT_RECT_ROAD_H }

#define TEXTURE_RECT_ROAD_MIRROR_X 0
#define TEXTURE_RECT_ROAD_MIRROR_Y (int)(0.76f * WINDOW_HEIGHT)
#define TEXTURE_RECT_ROAD_MIRROR_W WINDOW_WIDTH
#define TEXTURE_RECT_ROAD_MIRROR_H (int)(0.24f * WINDOW_HEIGHT)
#define TEXTURE_RECT_ROAD_MIRROR SDL_Rect{ TEXTURE_RECT_ROAD_MIRROR_X, TEXTURE_RECT_ROAD_MIRROR_Y, TEXTURE_RECT_ROAD_MIRROR_W, TEXTURE_RECT_ROAD_MIRROR_H }

#define VIEWPORT_RECT_ROAD_MIRROR_X (int)(0.15f * WINDOW_WIDTH)
#define VIEWPORT_RECT_ROAD_MIRROR_Y (int)(0.12f * WINDOW_HEIGHT)
#define VIEWPORT_RECT_ROAD_MIRROR_W (int)(0.7f * WINDOW_WIDTH)
#define VIEWPORT_RECT_ROAD_MIRROR_H (int)(0.17f * WINDOW_HEIGHT)
#define VIEWPORT_RECT_ROAD_MIRROR SDL_Rect{ VIEWPORT_RECT_ROAD_MIRROR_X, VIEWPORT_RECT_ROAD_MIRROR_Y, VIEWPORT_RECT_ROAD_MIRROR_W, VIEWPORT_RECT_ROAD_MIRROR_H }

#define PAUSE_POSITION_SCALE (1.5f * WINDOW_SCALE)
#define PAUSE_POSITION_X (int)(0.5f * WINDOW_WIDTH)
#define PAUSE_POSITION_Y (int)(0.55f * WINDOW_HEIGHT)
#define PAUSE_POSITION WindowPosition{ PAUSE_POSITION_X, PAUSE_POSITION_Y }

#define GO_MENU_POSITION_SCALE (1.25f * WINDOW_SCALE)
#define GO_MENU_POSITION_X (int)(0.5f * WINDOW_WIDTH)
#define GO_MENU_POSITION_Y (int)(0.825f * WINDOW_HEIGHT)
#define GO_MENU_POSITION WindowPosition{ GO_MENU_POSITION_X, GO_MENU_POSITION_Y }

#define BEST_LAP_POSITION_SCALE WINDOW_SCALE
#define BEST_LAP_POSITION_X (int)(0.01f * WINDOW_WIDTH)
#define BEST_LAP_POSITION_Y (int)(0.0225f * WINDOW_HEIGHT)
#define BEST_LAP_POSITION WindowPosition{ BEST_LAP_POSITION_X, BEST_LAP_POSITION_Y }

#define BEST_LAP_VALUE_POSITION_SCALE WINDOW_SCALE
#define BEST_LAP_VALUE_POSITION_X (int)(0.25f * WINDOW_WIDTH)
#define BEST_LAP_VALUE_POSITION_Y (int)(0.0225f * WINDOW_HEIGHT)
#define BEST_LAP_VALUE_POSITION WindowPosition{ BEST_LAP_VALUE_POSITION_X, BEST_LAP_VALUE_POSITION_Y }

#define CURRENT_VELOCITY_POSITION_SCALE (1.5f * WINDOW_SCALE)
#define CURRENT_VELOCITY_POSITION_X (int)(0.89f * WINDOW_WIDTH)
#define CURRENT_VELOCITY_POSITION_Y (int)(0.9f * WINDOW_HEIGHT)
#define CURRENT_VELOCITY_POSITION WindowPosition{ CURRENT_VELOCITY_POSITION_X, CURRENT_VELOCITY_POSITION_Y }

#define KMH_RECT_X (int)(0.895f * WINDOW_WIDTH)
#define KMH_RECT_Y (int)(0.825f * WINDOW_HEIGHT)
#define KMH_RECT_W (int)(1.5f * 40 * WINDOW_SCALE)
#define KMH_RECT_H (int)(1.5f * 28 * WINDOW_SCALE)
#define KMH_RECT SDL_Rect{ KMH_RECT_X, KMH_RECT_Y, KMH_RECT_W, KMH_RECT_H }

#define CURRENT_LAP_TIME_POSITION_SCALE (1.25f * WINDOW_SCALE)
#define CURRENT_LAP_TIME_POSITION_X (int)(0.5f * WINDOW_WIDTH)
#define CURRENT_LAP_TIME_POSITION_Y (int)(0.35f * WINDOW_HEIGHT)
#define CURRENT_LAP_TIME_POSITION WindowPosition{ CURRENT_LAP_TIME_POSITION_X, CURRENT_LAP_TIME_POSITION_Y }

#define CURRENT_LAP_TIME_NOTIFICATION_POSITION_SCALE (1.0f * WINDOW_SCALE)
#define CURRENT_LAP_TIME_NOTIFICATION_POSITION_X (int)(0.4875f * WINDOW_WIDTH)
#define CURRENT_LAP_TIME_NOTIFICATION_POSITION_Y (int)(0.675f * WINDOW_HEIGHT)
#define CURRENT_LAP_TIME_NOTIFICATION_POSITION WindowPosition{ CURRENT_LAP_TIME_NOTIFICATION_POSITION_X, CURRENT_LAP_TIME_NOTIFICATION_POSITION_Y }

#define CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION_SCALE (1.0f * WINDOW_SCALE)
#define CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION_X (int)(0.5125f * WINDOW_WIDTH)
#define CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION_Y (int)(0.675f * WINDOW_HEIGHT)
#define CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION WindowPosition{ CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION_X, CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION_Y }

#define MIRROR_BORDER_TRAPEZOID_OFFSET (int)(5 * WINDOW_SCALE)
#define MIRROR_BORDER_TRAPEZOID_BL_0_X VIEWPORT_RECT_ROAD_MIRROR_X
#define MIRROR_BORDER_TRAPEZOID_BL_0_Y (VIEWPORT_RECT_ROAD_MIRROR_Y + VIEWPORT_RECT_ROAD_MIRROR_H)
#define MIRROR_BORDER_TRAPEZOID_BR_0_X (VIEWPORT_RECT_ROAD_MIRROR_X + VIEWPORT_RECT_ROAD_MIRROR_W)
#define MIRROR_BORDER_TRAPEZOID_BR_0_Y MIRROR_BORDER_TRAPEZOID_BL_0_Y
#define MIRROR_BORDER_TRAPEZOID_TR_0_X MIRROR_BORDER_TRAPEZOID_BR_0_X
#define MIRROR_BORDER_TRAPEZOID_TR_0_Y VIEWPORT_RECT_ROAD_MIRROR_Y
#define MIRROR_BORDER_TRAPEZOID_TL_0_X MIRROR_BORDER_TRAPEZOID_BL_0_X
#define MIRROR_BORDER_TRAPEZOID_TL_0_Y MIRROR_BORDER_TRAPEZOID_TR_0_Y
#define MIRROR_BORDER_TRAPEZOID_BL_1_X (MIRROR_BORDER_TRAPEZOID_BL_0_X - MIRROR_BORDER_TRAPEZOID_OFFSET)
#define MIRROR_BORDER_TRAPEZOID_BL_1_Y (MIRROR_BORDER_TRAPEZOID_BL_0_Y + MIRROR_BORDER_TRAPEZOID_OFFSET)
#define MIRROR_BORDER_TRAPEZOID_BR_1_X (MIRROR_BORDER_TRAPEZOID_BR_0_X + MIRROR_BORDER_TRAPEZOID_OFFSET)
#define MIRROR_BORDER_TRAPEZOID_BR_1_Y MIRROR_BORDER_TRAPEZOID_BL_1_Y
#define MIRROR_BORDER_TRAPEZOID_TR_1_X MIRROR_BORDER_TRAPEZOID_BR_1_X
#define MIRROR_BORDER_TRAPEZOID_TR_1_Y (MIRROR_BORDER_TRAPEZOID_TR_0_Y - MIRROR_BORDER_TRAPEZOID_OFFSET)
#define MIRROR_BORDER_TRAPEZOID_TL_1_X MIRROR_BORDER_TRAPEZOID_BL_1_X
#define MIRROR_BORDER_TRAPEZOID_TL_1_Y MIRROR_BORDER_TRAPEZOID_TR_1_Y
#define MIRROR_BORDER_TRAPEZOID_BL_0 WindowPosition{ MIRROR_BORDER_TRAPEZOID_BL_0_X, MIRROR_BORDER_TRAPEZOID_BL_0_Y }
#define MIRROR_BORDER_TRAPEZOID_BR_0 WindowPosition{ MIRROR_BORDER_TRAPEZOID_BR_0_X, MIRROR_BORDER_TRAPEZOID_BR_0_Y }
#define MIRROR_BORDER_TRAPEZOID_TR_0 WindowPosition{ MIRROR_BORDER_TRAPEZOID_TR_0_X, MIRROR_BORDER_TRAPEZOID_TR_0_Y }
#define MIRROR_BORDER_TRAPEZOID_TL_0 WindowPosition{ MIRROR_BORDER_TRAPEZOID_TL_0_X, MIRROR_BORDER_TRAPEZOID_TL_0_Y }
#define MIRROR_BORDER_TRAPEZOID_BL_1 WindowPosition{ MIRROR_BORDER_TRAPEZOID_BL_1_X, MIRROR_BORDER_TRAPEZOID_BL_1_Y }
#define MIRROR_BORDER_TRAPEZOID_BR_1 WindowPosition{ MIRROR_BORDER_TRAPEZOID_BR_1_X, MIRROR_BORDER_TRAPEZOID_BR_1_Y }
#define MIRROR_BORDER_TRAPEZOID_TR_1 WindowPosition{ MIRROR_BORDER_TRAPEZOID_TR_1_X, MIRROR_BORDER_TRAPEZOID_TR_1_Y }
#define MIRROR_BORDER_TRAPEZOID_TL_1 WindowPosition{ MIRROR_BORDER_TRAPEZOID_TL_1_X, MIRROR_BORDER_TRAPEZOID_TL_1_Y }
#define MIRROR_BORDER_TRAPEZOID_TOP WindowTrapezoid{ MIRROR_BORDER_TRAPEZOID_TL_0, MIRROR_BORDER_TRAPEZOID_TR_0, MIRROR_BORDER_TRAPEZOID_TR_1, MIRROR_BORDER_TRAPEZOID_TL_1 }
#define MIRROR_BORDER_TRAPEZOID_LEFT WindowTrapezoid{ MIRROR_BORDER_TRAPEZOID_BL_0, MIRROR_BORDER_TRAPEZOID_TL_0, MIRROR_BORDER_TRAPEZOID_TL_1, MIRROR_BORDER_TRAPEZOID_BL_1 }
#define MIRROR_BORDER_TRAPEZOID_RIGHT WindowTrapezoid{ MIRROR_BORDER_TRAPEZOID_BR_1, MIRROR_BORDER_TRAPEZOID_TR_1, MIRROR_BORDER_TRAPEZOID_TR_0, MIRROR_BORDER_TRAPEZOID_BR_0 }
#define MIRROR_BORDER_TRAPEZOID_BOTTOM WindowTrapezoid{ MIRROR_BORDER_TRAPEZOID_BL_1, MIRROR_BORDER_TRAPEZOID_BR_1, MIRROR_BORDER_TRAPEZOID_BR_0, MIRROR_BORDER_TRAPEZOID_BL_0 }

#define LIGHT_RECT_X (int)(0.0175f * WINDOW_WIDTH)
#define LIGHT_RECT_Y (int)(0.3275f * WINDOW_HEIGHT)
#define LIGHT_RECT_W (int)(1.2f * 192 * WINDOW_SCALE)
#define LIGHT_RECT_H (int)(1.2f * 80 * WINDOW_SCALE)
#define LIGHT_RECT SDL_Rect{ LIGHT_RECT_X, LIGHT_RECT_Y, LIGHT_RECT_W, LIGHT_RECT_H }

#endif