#include "Post.h"
#include "Config.h"

#ifdef DEBUG
#include <iostream>
#include <stdio.h>
#include <glm/ext.hpp>
#endif

#define K_SIZE 3

using namespace std;

static float sobelx[3][3] = {{-1., 0., 1.},
                             {-2., 0., 2.},
                             {-1., 0., 1.}};

static float sobely[3][3] = {{-1, -2, -1},
                             {0, 0, 0},
                             {1, 2, 1}};

#if (K_SIZE == 3)
static float g_blur[3][3] = {{1, 2, 1},
                             {2, 4, 2},
                             {1, 2, 1}};
#else
static float g_blur[5][5] = {{1, 4, 6, 4 ,1},
                             {4, 16, 24, 16, 4},
                             {6, 24, 36, 24, 6},
                             {4, 16, 24, 16, 4},
                             {1, 4, 6, 4 ,1}};

#endif
void mark_disk(vec3 pixel[SCREEN_WIDTH][SCREEN_HEIGHT],
               bool disc[SCREEN_WIDTH][SCREEN_HEIGHT]);

void post_processing(vec3 pixel[SCREEN_WIDTH][SCREEN_HEIGHT])
{
    bool disc[SCREEN_WIDTH][SCREEN_HEIGHT];
    mark_disk(pixel,disc);
    return;
}

float average(vec3 v3)
{
    return (v3[0] + v3[1] + v3[2]) / 3;
}

void mark_disk(vec3 pixel[SCREEN_WIDTH][SCREEN_HEIGHT],
               bool disc[SCREEN_WIDTH][SCREEN_HEIGHT])
{
    int xn, yn;
    vec3 magX, magY, magP;
    for (int i = 1; i < SCREEN_WIDTH-(K_SIZE - 1)/2; i++)
    {
        for (int j = 1; j < SCREEN_HEIGHT- (K_SIZE - 1)/2; j++)
        {
            // Convolve
            magX = vec3(0);
            magY = vec3(0);

            for (int a = 0; a < K_SIZE; a++)
            {
                for (int b=0; b < K_SIZE; b++)
                {
                    xn = i + a - 1;
                    yn = j + b - 1;

                    magX = magX + pixel[xn][yn] * sobelx[a][b];
                    magY = magY + pixel[xn][yn] * sobely[a][b];
                }
            }
            disc[i][j] = average(glm::abs(magX + magY))  > THRESHOLD ? true : false;
        }
    }

    for (int i = 1; i < SCREEN_WIDTH-1; i++)
    {
        for (int j = 1; j < SCREEN_HEIGHT-1; j++)
        {
            if (disc[i][j])
            {
                magP = vec3(0);
                for (int a = 0; a < 3; a++)
                {
                    for(int b = 0; b < 3; b++)
                    {
                        xn = i + a - 1;
                        yn = j + b - 1;
                        magP += pixel[xn][yn] * g_blur[a][b] * 1.f/16.f;
                    }
                }
                pixel[i][j] = magP;
            }
        }
    }
}
