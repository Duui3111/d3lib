#include "../d3lib/ImgProcessing.h"
#include <iostream>

struct Region
{
    int x, y, w, h;
    Region(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
};

#define RANGE(x, a, b) ((a <= x) && (x <= b))

int main()
{
    std::vector<Region> region;

    d3lib::Image bmp("facerec.bmp");
    d3lib::Image Finalbmp("facerec.bmp");

    int X = 0, Y = 0, OFFX = 0, OFFY = 0, LastVal = 0;

    for (int i = 0; i < bmp.Width(); i++)
    {
        for (int j = 0; j < bmp.Height(); j++)
        {
            if (!(RANGE(bmp.ToHSV(i, j)[0], 0, 50) && RANGE(bmp.ToHSV(i, j)[1], 0.23, 0.68))) bmp.SetPixels(i, j, 0, 0, 0);
            else bmp.SetPixels(i, j, 255, 255, 255);

            if (bmp.ToRGB(i, j) != 0 && bmp.ToRGB(i - 4, j) == 0 || bmp.ToRGB(i, j) != 0 && bmp.ToRGB(i - 4, j - 4) == 0 ||
                bmp.ToRGB(i, j) != 0 && bmp.ToRGB(i - 2, j - 2) == 0) bmp.SetPixels(i, j, 0, 0, 1);

            if (bmp.ToRGB(i - 1, j) == bmp.WhatRGB(255, 255, 255)) bmp.SetPixels(i, j, 255, 0, 0), X = i;
            if (bmp.ToRGB(i, j - 1) == bmp.WhatRGB(255, 255, 255)) bmp.SetPixels(i, j, 0, 255, 0), Y = j;
            
            if (bmp.ToRGB(i, j) == bmp.WhatRGB(0, 0, 1)) OFFX = i, OFFY = j;

            if (bmp.ToRGB(i, j) == bmp.WhatRGB(255, 0, 0))
            {
                if (X == OFFX)
                {                        
                    if (bmp.ToRGB(X, Y) != bmp.WhatRGB(0, 255, 0))
                    {
                        region.push_back({ X, Y, 0, 0 });
                    }
                }
            }
        }
    }

    for (int i = 0; i < region.size(); i++)
    {
        if (bmp.ToRGB(region[i].x, region[i].y) != bmp.WhatRGB(0, 0, 1))
        {
            if (bmp.ToRGB(region[i].x, region[i].y) != bmp.WhatRGB(0, 0, 255))
            {
                if (region[i].y != region[(i+1) % region.size()].y)
                {
                    if (region[i].y != region[(i + 1) % region.size()].y + 1)
                    {
                        std::cout << "[" << (int)bmp.GetPixels(region[i].x, region[i].y)[0] << ", " << (int)bmp.GetPixels(region[i].x, region[i].y)[1] << ", "
                            << (int)bmp.GetPixels(region[i].x, region[i].y)[2] << "] [" << region[i].x << ", " << region[i].y << "]" << std::endl;

                        //Finalbmp.DrawCircle(region[i].x, region[i].y, 0, 0, 255, 5);
                        Finalbmp.DrawRectangle(region[i].x-7, region[i].y-10, region[i].x+5, region[i].y+5, 0, 0, 255);
                    }
                }
            }
        }
    }

    Finalbmp.save("facerec_output.bmp");
}