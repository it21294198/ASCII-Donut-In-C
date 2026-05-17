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
    - Lighting and ASCII Shading
    - Real 3D graphics use: light direction and surface direction, to determine brightness.
    - If a surface faces the light: bright ; If it faces away: dark
    - ASCII brightness Trick: Instead of real pixels/colors, use characters.
    - Dark → Bright: ".,-~:;=!*#$@"
    - Surface normals : Every surface has a direction. Called: normal vector ; Think: which way surface is facing
    - Facing light ;LIGHT ---> [surface] : bright.
    - Facing away ;[surface] <--- LIGHT : dark.
    - Measuring brightness : use dot product ; Simple meaning: how aligned two directions are.
    - Dot product result
        +----------+--------------+
        |  Result  |    Meaning   |
        +----------+--------------+
        | positive | facing light |
        +----------+--------------+
        | zero     | sideways     |
        +----------+--------------+
        | negative | facing away  |
        +----------+--------------+
    - In donut.c , N = calculates brightness. Then: ".,-~:;=!*#$@"[N] , selects the character.
    - Brightness depends on theta, just to understand shading.
    - This works for now using : sin(theta)
        - returns:
            - -1 → +1
            - add 1 : 0 → 2
            - multiply by 5 : 0 → 10
            - now get indexes into: ".,-~:;=!*#$@"
    - Simple mental model :
        - Surface directly facing light -> brightness = high
        - Surface sideways -> brightness = medium
        - Surface facing away -> brightness = dark
Step 8 -> rotation
    - So every frame slightly rotate the donut.
    - Imagine top-down view for (x,z)
    - Rotate around Y-axis by angle A:
        - x' = x*cos(A) - z*sin(A)
        - z' = x*sin(A) + z*cos(A)
    - Just rotate does not move the donut.
    - Just changing coordinates to simulate rotation.
    - To animation uses infinite while(1) loop.
    - Need to clear the buffers every frame, otherwise old frames remain.
Step 9 -> animation with real lighting
    - Simple meaning of dot product
        - The dot product measures: how much two directions point the same way
    - Dot product formula : a⋅b = ax*bx + ay*by + az*bz
    +--------+-------------------+
    | Vector |      Meaning      |
    +--------+-------------------+
    | normal | surface direction |
    +--------+-------------------+
    | light  | light direction   |
    +--------+-------------------+
    - Surface normal on the donut
        - The donut surface points outward.
            - inside tube -> inward
            - outside tube -> outward
    - Define a light direction
        - Suppose light comes from: top-right-front
        - define as:
            float lightX = 0;
            float lightY = 1;
            float lightZ = -1;
    - Surface normal for the torus:
        normalX = cos(theta) * cos(phi);
        normalY = sin(theta);
        normalZ = cos(theta) * sin(phi);
    - These describe: which way surface faces
    - Calculate brightness using dot product
        - brightness = nx*lx + ny*ly + nz*lz
    - values usually: -1 → +1
    - Negative values mean, surface faces away : usually ignore those points.
Step 10 -> final touches
    - Add another rotation around Z-axis by angle B
    - Make values reusable variables, to avoid recalculating repeatedly.
*/

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 22

/*
    screen[]  -> stores ASCII characters
    zbuffer[] -> stores depth values (1/z)
*/
char screen[WIDTH * HEIGHT];
float zbuffer[WIDTH * HEIGHT];

int main()
{
    /*
        A -> rotation angle around X-axis
        B -> rotation angle around Z-axis
    */
    float A = 0;
    float B = 0;

    /*
        ASCII brightness map

        Dark -----------------> Bright
          .,-~:;=!*#$@
    */
    char shades[] = ".,-~:;=!*#$@";

    /*
        Clear terminal screen once
    */
    printf("\x1b[2J");

    while (1)
    {
        /*
            Clear frame buffers
        */
        memset(screen, ' ', WIDTH * HEIGHT);
        memset(zbuffer, 0, sizeof(zbuffer));

        /*
            Screen center
        */
        int centerX = WIDTH / 2;
        int centerY = HEIGHT / 2;

        /*
            Torus dimensions

            R1 -> tube radius
            R2 -> donut radius
        */
        float R1 = 1;
        float R2 = 2;

        /*
            K2 -> camera distance

            Moves donut away from camera
            so z never becomes 0
        */
        float K2 = 5;

        /*
            K1 -> projection scaling factor

            Controls donut size on screen
        */
        float K1 = WIDTH * K2 * 3 / (8 * (R1 + R2));

        /*
            theta -> angle around tube

            0 -> 2PI
        */
        for (float theta = 0; theta < 2 * M_PI; theta += 0.07)
        {
            /*
                phi -> angle around entire donut
            */
            for (float phi = 0; phi < 2 * M_PI; phi += 0.02)
            {
                /*
                    Precompute trig values

                    Faster than recalculating repeatedly
                */
                float cosTheta = cos(theta);
                float sinTheta = sin(theta);

                float cosPhi = cos(phi);
                float sinPhi = sin(phi);

                float cosA = cos(A);
                float sinA = sin(A);

                float cosB = cos(B);
                float sinB = sin(B);

                /*
                    Small circle coordinates

                    circleX -> horizontal tube position
                    circleY -> vertical tube position

                    x = R2 + R1*cos(theta)
                    y = R1*sin(theta)
                */
                float circleX = R2 + R1 * cosTheta;
                float circleY = R1 * sinTheta;

                /*
                    =================================================
                    3D ROTATION EQUATIONS
                    =================================================

                    Rotates torus using angles A and B

                    Final rotated coordinates:
                        x
                        y
                        z
                */

                float x =
                    circleX * (cosB * cosPhi + sinA * sinB * sinPhi) - circleY * cosA * sinB;

                float y =
                    circleX * (sinB * cosPhi - sinA * cosB * sinPhi) + circleY * cosA * cosB;

                float z =
                    K2 + cosA * circleX * sinPhi + circleY * sinA;

                /*
                    =================================================
                    PERSPECTIVE PROJECTION
                    =================================================

                    ooz -> One Over Z

                    Near object:
                        large ooz

                    Far object:
                        small ooz
                */
                float ooz = 1 / z;

                /*
                    =================================================
                    3D -> 2D SCREEN CONVERSION
                    =================================================

                    screenX = centerX + K1 * x / z
                    screenY = centerY - K1 * y / z

                    y uses 0.5 scaling because terminal
                    characters are taller than wide
                */
                int screenX =
                    (int)(centerX + K1 * ooz * x);

                int screenY =
                    (int)(centerY - K1 * ooz * y * 0.5);

                /*
                    Convert 2D coordinate to array index

                    index = x + y*WIDTH
                */
                int index = screenX + screenY * WIDTH;

                /*
                    =================================================
                    LIGHTING CALCULATION
                    =================================================

                    L -> luminance

                    Dot product between:
                        surface normal
                        light direction

                    Positive:
                        facing light

                    Negative:
                        facing away
                */
                float L =
                    cosPhi * cosTheta * sinB - cosA * cosTheta * sinPhi - sinA * sinTheta + cosB * (cosA * sinTheta - cosTheta * sinA * sinPhi);

                /*
                    Draw only illuminated surfaces
                */
                if (L > 0)
                {
                    /*
                        Bounds check
                    */
                    if (screenX >= 0 &&
                        screenX < WIDTH &&
                        screenY >= 0 &&
                        screenY < HEIGHT)
                    {
                        /*
                            =================================================
                            Z-BUFFER TEST
                            =================================================

                            Draw only if point is closer
                        */
                        if (ooz > zbuffer[index])
                        {
                            /*
                                Update depth
                            */
                            zbuffer[index] = ooz;

                            /*
                                Convert brightness to ASCII index
                            */
                            int luminanceIndex = L * 8;

                            /*
                                Draw ASCII character
                            */
                            screen[index] =
                                shades[luminanceIndex];
                        }
                    }
                }
            }
        }

        /*
            Move terminal cursor to top-left
        */
        printf("\x1b[H");

        /*
            Print frame
        */
        for (int i = 0; i < WIDTH * HEIGHT; i++)
        {
            putchar(i % WIDTH ? screen[i] : '\n');
        }

        /*
            Animate rotation
        */
        A += 0.04;
        B += 0.02;

        /*
            Frame delay
        */
        usleep(30000);
    }

    return 0;
}