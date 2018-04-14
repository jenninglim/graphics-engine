#include "Scene.h"
#include "TestModelH.h"
#include "Post.h"
#include "Octree.h"

#define SCALE 0.3f
using namespace glm;
using glm::vec3;

static vec3 pixels[SCREEN_WIDTH][SCREEN_HEIGHT];

void drawPixels(screen * screen);

Scene::Scene()
{
    vec4 camPos(0,0,-3,1);
    this->cam = Camera(CAM_FOCAL_LENGTH, camPos);
    this-> light = Light(
            vec4(0, -0.5, -0.2, 1),
            14.f * vec3(1,1,1));
    LoadTestModel(this->objects);
    this->bvh = BVH(objects);
    this->octree = Octree(this->objects, bvh.bv, light, &this->bvh);
}

void Scene::Draw(screen* screen)
{
    vec3 color;
    vec4 rayFromOrigin, rayFromCam;
    vec3 d = vec3();
    Ray r;
    updateTextureOctree(&this->octree, light, &this->bvh);

    for(int y = 0; y < SCREEN_HEIGHT; y++){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            rayFromOrigin.x = x - SCREEN_WIDTH/2;
            rayFromOrigin.y = y - SCREEN_HEIGHT/2;
            rayFromOrigin.z = cam.focalLength;
            rayFromOrigin[3] = 1;

            color = vec3(0);

            rayFromCam = cam.R * rayFromOrigin;

            d = glm::normalize(vec3(rayFromCam[0],
                        rayFromCam[1],
                        rayFromCam[2]));

            r.initial = cam.position;
            r.direction = d;

            shootRay(r, color, bvh, light);
            shootRay(r, color, this->octree, this->bvh, light);
            pixels[x][y] = color;
        }
    }
    //post_processing(pixels);
    drawPixels(screen);
}

void drawPixels(screen * screen)
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        for (int j = 0; j < SCREEN_HEIGHT; j ++)
        {
            PutPixelSDL(screen, i, j, pixels[i][j]);
        }
    }
}

void Scene::Update(KeyStroke_t * keystate)
{
    if ( keystate[SDL_SCANCODE_UP] )
	{
        this->cam.forward();
	}
	if ( keystate[SDL_SCANCODE_DOWN] )
	{
		this->cam.backward();
	}
	if ( keystate[SDL_SCANCODE_LEFT] )
	{
	    this->cam.left();
	}
	if ( keystate[SDL_SCANCODE_RIGHT] )
	{
	    this->cam.right();
	}
    if ( keystate[SDL_SCANCODE_A] )
	{
	    this->light.position += SCALE * vec4(-1,0,0,0);
	}
    if ( keystate[SDL_SCANCODE_W] )
	{
        this->light.position += SCALE * vec4(0,0,-1,0);
	}
    if ( keystate[SDL_SCANCODE_D] )
	{
        this->light.position += SCALE * vec4(1,0,0,0);
	}
    if ( keystate[SDL_SCANCODE_S] )
	{
        this->light.position += SCALE * vec4(0,0,1,0);
	}
}


