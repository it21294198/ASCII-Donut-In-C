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
Step 2 -> draw a point
    - Need to understand:(0,0) top-left (x -> horizontal, y -> vertical)
    - Convert (x,y) into array index
    - Uses: index = x + y × WIDTH
    - Draw a single point : screen[40 + 10 * WIDTH] = '@';
Step 3 -> draw a circle
    - A circle is created by moving around angles. The magic formulas are: x = r * cos(θ), y = r * sin(θ)
    - As theta(θ) changes: 0 → 2π the point moves around a circle.
    - cos() and sin() return values between: -1 and +1
    - scale the circle by multiplying the radius (r) to the cos() and sin() values.
    - x = radius * cos(theta); y = radius * sin(theta);
    - shift the circle to screen center. Terminal coordinates: (0,0) start at TOP LEFT.
    - Formula for Circle coordinates : x = centerX + r*cos(θ), y = centerY +r*sin(θ)
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

    // circle parameters
    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;

    int radius = 8;

    // move around circle
    for (float theta = 0; theta < 2 * M_PI; theta += 0.05)
    {
        int x = centerX + radius * cos(theta);
        int y = centerY + radius * sin(theta);

        screen[x + y * WIDTH] = '@';
    }

    // print characters on screen
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