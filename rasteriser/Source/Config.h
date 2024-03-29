#ifndef CONFIG_H
#define CONFIG_H

#define L 555

// Screen Setup
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define FULLSCREEN_MODE false

// Camera
#define ROTATION_SPEED 0.05 // In radians
#define CAM_FOCAL_LENGTH  300

// Lighting
#define SPECULAR_INTENSITY 14
#define DIFFUSE_INTENSITY 2
#define AMBIENT_INTENSITY 0.3

//Clipping
#define W_CLIPPING_PLANE 0.00001
#define NEARPLANE 0.5
#define FARPLANE 20
#define ANGLEOFVIEW 90

// AA
#define THRESHOLD 0.5

//Shadows on/off
#define SHADOWS false
#define BUNNYSHADOWS false



#endif
