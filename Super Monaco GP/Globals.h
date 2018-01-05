#ifndef _GLOBALS_
#define _GLOBALS_

// Editable

#define WINDOW_TITLE "Super Monaco GP"
#define WINDOW_SCALE 1.75f

#define CAMERA_Y 5.0f
#define CAMERA_FOV 90.0f

#define SEGMENT_LENGTH 0.5f
#define DRAW_DISTANCE 200.0f

#define ROAD_WIDTH 30.0f

#define ROAD_RUMBLE_WIDTH 2.5f
#define ROAD_RUMBLE_HEIGHT 3.0f
#define ROAD_RUMBLE_WIDTH_PART_2 1.0f
#define ROAD_RUMBLE_WIDTH_PART_4 1.0f
#define ROAD_RUMBLE_OFFSET_X_PARTS_1_2 1.0f

#define ROAD_CURVE_HOLD_PERCENT 0.333f

#define ROAD_HILL_HOLD_PERCENT 0.0f

#define SPRITE_SIZE_RATIO 0.02f

#define BACKGROUND_REGION_X 256
#define BACKGROUND_REGION_Y 72
#define BACKGROUND_VELOCITY_OFFSET_X 20.0f
#define BACKGROUND_VELOCITY_OFFSET_Y 1.0f

#define PI 3.14159265358979323846f

#define N_COURSES 2
#define N_CARS 16
#define N_LAPS 3

static const char* COURSE_NAMES[N_COURSES]{ "ITALY", "PORTUGAL" };
static const float COURSE_LENGHTS[N_COURSES]{ 3808.0f, 3296.0f };

#define CAR_MAX_VELOCITY 60.0f
#define CAR_ACCELERATION_ROAD 16.0f
#define CAR_DEACCELERATION_ROAD 12.0f
#define CAR_DEACCELERATION_FRICTION_ROAD 4.0f
#define CAR_DEACCELERATION_FRICTION_GRASS 6.0f
#define CAR_DEACCELERATION_FRICTION_EXTRA_ROAD 0.0f
#define CAR_DEACCELERATION_FRICTION_EXTRA_GRASS 10.0f

// Computed

#define WINDOW_WIDTH (int)(640 * WINDOW_SCALE)
#define WINDOW_HEIGHT (int)(448 * WINDOW_SCALE)

#define N_SEGMENTS_DRAW (uint)(DRAW_DISTANCE / SEGMENT_LENGTH)

#define ROAD_MIN_X (-0.5f * ROAD_WIDTH)
#define ROAD_MAX_X (ROAD_MIN_X + ROAD_WIDTH)

#define ROAD_MIN_LENGTH DRAW_DISTANCE

#define ROAD_CURVE_ENTER_PERCENT (0.5f * (1.0f - ROAD_CURVE_HOLD_PERCENT))
#define ROAD_CURVE_LEAVE_PERCENT ROAD_CURVE_ENTER_PERCENT

#define ROAD_HILL_ENTER_PERCENT (0.5f * (1.0f - ROAD_HILL_HOLD_PERCENT))
#define ROAD_HILL_LEAVE_PERCENT ROAD_HILL_ENTER_PERCENT

#define CAR_MIN_X (ROAD_MIN_X - 2.0f)
#define CAR_MAX_X (ROAD_MAX_X + 2.0f)

#define CAR_ACCELERATION_GRASS (0.5f * CAR_ACCELERATION_ROAD)
#define CAR_DEACCELERATION_GRASS (1.5f * CAR_DEACCELERATION_ROAD)
#define CAR_DEACCELERATION_FRICTION_LIMIT_VELOCITY (0.333f * CAR_MAX_VELOCITY)

#endif