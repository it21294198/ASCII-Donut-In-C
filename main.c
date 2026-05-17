#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Step 1 -> screen buffer
    - Need memory that represents the terminal screen.
    - Think of it like pixels.
    - But instead of pixels `@,#,.,space` stored characters.
    - Suppose terminal size: 80x22
    - Need to understand:(0,0) top-left (x -> horizontal, y -> vertical)
    - Convert (x,y) into array index
    - Uses: index = x + y × WIDTH
    - Draw a single point : screen[40 + 10 * WIDTH] = '@';
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
    // fill screen with spaces, so terminal becomes blank.
    memset(screen, ' ', WIDTH * HEIGHT);

    // Draw a single point
    screen[40 + 10 * WIDTH] = '@';

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