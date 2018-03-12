#ifndef CONFIG_H
#define CONFIG_H

// Screen Setup
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define FULLSCREEN_MODE false

// Camera
#define ROTATION_SPEED 0.1 // In radians
#define CAM_FOCAL_LENGTH SCREEN_WIDTH

// Auxillary Variables
#define EPSILON 1e-3
#define DEBUG 1
#define RAY_DEPTH 10

// SCENE SETTINGS
#define WALL_REFLECTANCE 0.9
#define BOX_REFLECTANCE 0.5
#endif
