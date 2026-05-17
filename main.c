#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Step 1 -> screen buffer
    - Suppose terminal size: 80x22
Step 2 -> draw a point
Step 3 -> draw a circle
Step 4 -> create donut points
Step 5 -> projection
Step 6 -> rotation
Step 7 -> depth buffer
Step 8 -> lighting
Step 9 -> animation
*/

#define WIDTH 80
#define HEIGHT 22

char screen[WIDTH * HEIGHT];

int main()
{
    // fill screen with spaces
    memset(screen, ' ', WIDTH * HEIGHT);

    // add newline at end of each row
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        putchar(screen[i]);

        if ((i + 1) % WIDTH == 0)
        {
            putchar('\n');
        }
    }
    return 0;
}