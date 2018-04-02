#ifndef CONFIG_H
#define CONFIG_H
//Intersection Tests
#define MAX_DIST 5
#define CONE_SIZE 0.01/ 2.f
#define VOL_APPROX_INT 0.01

// Screen Setup
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define FULLSCREEN_MODE false

// Camera
#define ROTATION_SPEED 0.1 // In radians
#define CAM_FOCAL_LENGTH SCREEN_WIDTH

// Auxillary Variables
#define EPSILON 1e-3
#define DEBUG 1
#define RAY_DEPTH 3

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

