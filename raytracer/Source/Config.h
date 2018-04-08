#ifndef CONFIG_H
#define CONFIG_H
//Intersection Tests
#define MAX_DIST 5.f
#define CONE_SIZE 0.01/ 2.f
#define VOL_APPROX_INT 0.01

// Screen Setup
#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200
#define FULLSCREEN_MODE true

// Camera
#define ROTATION_SPEED 0.1 // In radians
#define CAM_FOCAL_LENGTH SCREEN_WIDTH

// Auxillary Variables
#define EPSILON 1e-3f
#define DEBUG 1
#define RAY_DEPTH 1

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

// OCTREE
#define OCT_DEPTH 5

// CONE ANGLES
#define SHADOW_ANG 0.5
#endif
