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
    - Formula for Circle coordinates :
    - x = centerX + r*cos(θ)
    - y = centerY +r*sin(θ)
Step 4 -> create donut points
    - Small circle : This creates the tube thickness.
    - Large circle : The small circle travels around this path
    - theta(θ) : around tube , phi(ϕ) : around whole donut
    - Rotate around center : x=(R2+R1*cos(θ))cos(ϕ) ; y=(R2+R1*cos(θ))sin(ϕ)
    - Before drawing, ALWAYS check bounds.
Step 5 -> projection (3D point → 2D screen point)
    - Real-life perspective : Things far away appear smaller.
    - The simplest projection formula:
    - screenX = z/x
    - screenY = z/y
    - If: z = 0 ;program crashes. So move object away from camera.
    - Move donut forward -> add: z += 20; Now donut is always in front of camera.
    - K1 = zoom factor
    - screenX = centerX + K1(z/x)
    - screenY = centerY + K1(z/y)
    - ooz = one over z = 1/z
Step 6 -> depth buffer
    - Currently: far points can overwrite near points
    - A z-buffer stores : which point is closest to the camera for every screen pixel.
    - Current problem : Right now, screen[index] = '@', always overwrites. So draw order matters.
    - Need hidden surface removal; Is this point closer than previous point? If YES: draw it; If NO: ignore it
    - Create depth buffer : float zbuffer[WIDTH * HEIGHT];
    - For every screen pixel:
        - screen[]  -> character
        - zbuffer[] -> depth
    - ooz = 1 / z -> Closer objects: large ooz ; Far objects: small ooz ; So: BIGGER ooz = closer
    - (Depth test) Z-buffer rule -> If: ooz > zbuffer[pixel] ? then: new point is closer, So update the pixel.
Step 7 -> lighting
Step 8 -> rotation
Step 9 -> animation
*/

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 22

char screen[WIDTH * HEIGHT];
float zbuffer[WIDTH * HEIGHT];

int main()
{
    memset(screen, ' ', WIDTH * HEIGHT);
    memset(zbuffer, 0, sizeof(zbuffer));

    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;

    float R1 = 4;  // tube radius
    float R2 = 10; // donut radius

    float K1 = 30;

    for (float theta = 0; theta < 2 * M_PI; theta += 0.1)
    {
        for (float phi = 0; phi < 2 * M_PI; phi += 0.1)
        {
            // donut point in 3D
            float x = (R2 + R1 * cos(theta)) * cos(phi);
            float y = R1 * sin(theta);
            float z = (R2 + R1 * cos(theta)) * sin(phi);

            // move donut away from camera
            z += 20;

            // perspective projection
            float ooz = 1 / z;

            // screenX = centerX + K1(z/x)
            int screenX = centerX + K1 * ooz * x;
            int screenY = centerY + K1 * ooz * y;

            // bounds check
            if (screenX >= 0 && screenX < WIDTH &&
                screenY >= 0 && screenY < HEIGHT)
            {
                // screen[screenX + screenY * WIDTH] = '@';
                int index = screenX + screenY * WIDTH;

                // depth test
                if (ooz > zbuffer[index])
                {
                    zbuffer[index] = ooz;
                    screen[index] = '@';
                }
            }
        }
    }

    // print screen
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