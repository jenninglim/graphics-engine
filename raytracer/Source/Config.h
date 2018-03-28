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
#define RAY_DEPTH 5

//// SCENE SETTINGS
// REFLECT
#define WALL_REFLECTANCE 1

#define BOX_REFLECTANCE 0.8
#define BOX_REFRACT 0.8f

#define SPHERE_REFLECTANCE 1.0f
#define SPHERE_REFRACT 0.3f
#define DEF_IOR 1000000.f

// Lighting
#define LIGHT_POWER

// AA
#define THRESHOLD 0.5
#endif

