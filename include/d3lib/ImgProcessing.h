#ifndef __IMGPROCESSING_H__
#define __IMGPROCESSING_H__

#include <fstream>
#include <vector>
#include <string>
#include <cmath>

namespace d3lib
{
    class Image
    {
    private:
        int width = 0, height = 0;
        unsigned char* pixels = nullptr;

    public:
        Image() {}
        Image(std::string filename) { load(filename); }
        Image(int width, int height) : width(width), height(height) { this->pixels = new unsigned char[width * height * 3]; }
        ~Image() { delete[] this->pixels; }

        int Width() const { return this->width; }
        int Height() const { return this->height; }

        void load(std::string filename)
        {
            std::ifstream img(filename, std::ios::binary);

            if (filename.find(".bmp") != std::string::npos)
            {
                char header[54]{};
                img.read(header, sizeof(header));

                unsigned char bmp_pad[3] = { 0, 0, 0, };
                this->width = header[18], this->height = header[22];
                this->pixels = new unsigned char[this->width * this->height * 3];

                for (int i = 0; i < this->height; i++)
                {
                    img.read((char*)this->pixels + (this->width * i * 3), 3 * this->width);
                    img.read((char*)bmp_pad, (4 - (this->width * 3) % 4) % 4);
                }
            }

            img.close();
        }

        void Grayscale(const int x, const int y)
        {
            unsigned char gray = ((int)GetPixels(x, y)[0] + (int)GetPixels(x, y)[1] + (int)GetPixels(x, y)[2]) / 3;
            SetPixels(x, y, gray, gray, gray);
        }

        void Rezise(int w, int h)
        {
            this->width = w, this->height = h;
            this->pixels = new unsigned char[w * h * 3];
        }

        void SetPixels(const int x, const int y, const unsigned char R, const unsigned char G, const unsigned char B) 
        { 
            this->pixels[x * 3 + (this->height - y - 1) * this->width * 3 + 2] = R;
            this->pixels[x * 3 + (this->height - y - 1) * this->width * 3 + 1] = G;
            this->pixels[x * 3 + (this->height - y - 1) * this->width * 3 + 0] = B;
        }

        std::vector<unsigned char> GetPixels(const int x, const int y) const
        { 
            return 
            { 
                this->pixels[x * 3 + (this->height - y - 1) * this->width * 3 + 2], 
                this->pixels[x * 3 + (this->height - y - 1) * this->width * 3 + 1],
                this->pixels[x * 3 + (this->height - y - 1) * this->width * 3 + 0]
            };
        }

        std::vector<float> ToHSV(int x, int y)
        {
            int r = GetPixels(x, y)[0];
            int g = GetPixels(x, y)[1];
            int b = GetPixels(x, y)[2];

            float max = static_cast<float>(fmax(r, fmax(g, b)));
            float delta = static_cast<float>(max - fmin(r, fmin(g, b)));

            std::vector<float> hsvColor{ 0, (delta / max), max };

            if (hsvColor[0] < 0.0f) hsvColor[0] += 0.0f;
            if (hsvColor[1] < 0.0f) hsvColor[1] += 0.0f;

            if (r == max) hsvColor[0] = 60 * ((g - b) / delta);
            else if (g == max) hsvColor[0] = 60 * (2 + (b - r) / delta);
            else if (b == max) hsvColor[0] = 60 * (4 + (r - g) / delta);

            return hsvColor;
        }

        unsigned long ToRGB(int x, int y) const
        {
            return ((GetPixels(x, y)[0] << 0) | (GetPixels(x, y)[1] << 8) | (GetPixels(x, y)[2] << 16));
        }

        constexpr unsigned long WhatRGB(int R, int G, int B) const noexcept 
        { 
            return ((R << 0) | (G << 8) | (B << 16)); 
        }

        void DrawLine(int x1, int y1, int x2, int y2, int R, int G, int B)
        {
            float xdiff_abs = static_cast<float>(x2 - x1);
            if (xdiff_abs < 0) xdiff_abs *= -1;
            float ydiff_abs =  static_cast<float>(y2 - y1);
            if (ydiff_abs < 0) ydiff_abs *= -1;

            float steps;
            if (xdiff_abs >= ydiff_abs) steps = xdiff_abs;
            else steps = ydiff_abs;

            for (float i = 0; i <= (int)steps; i++)
            {
                float corrector = 0;
                if (i == 0 && steps == 0) corrector = 1;
                
                float pX = x1 + ((x2 - x1) / (steps + corrector) * i);
                float pY = y1 + ((y2 - y1) / (steps + corrector) * i);
                SetPixels((int)pX, (int)pY, R, G, B);
            }
        }

        void DrawRectangle(int x1, int y1, int x2, int y2, int R, int G, int B)
        {
            DrawLine(x1, y1, x1, y2, R, G, B);
            DrawLine(x1, y1, x2, y1, R, G, B);
            DrawLine(x2, y1, x2, y2, R, G, B);
            DrawLine(x2, y2, x1, y2, R, G, B);
        }

        void DrawCircle(int x, int y, int R, int G, int B, int radius = 1)
        {
            for (float rad = 0; rad < 3.14 * 2; rad += (float)(5 / (2 * 3.14 * radius)))
                SetPixels(x + (int)(radius * cos(rad)), y + (int)(radius * sin(rad)), R, G, B);    
        }

        std::vector<std::vector<double>> ToMatrix()
        {
            std::vector<int> image3;

            for(int _i = 0; _i < height; _i++)
                for(int _j = 0; _j < width; _j++)		
                    image3.push_back(ToRGB(_i, _j) == 0 ? 1 : -1);
                
            std::vector<std::vector<double>> a(height, std::vector<double>(width));
            
            for(int i = 0; i < 100; ++i) 
                a[i / height][i % width] = image3[i];

            for (size_t i = 0; i <  a.size(); i++) 
		        for (size_t j = i; j < a.size(); j++) 		
			        std::swap(a[i][j], a[j][i]);
            
            return a;
        }
        
        void save(std::string filename)
        {
            std::ofstream out(filename, std::ios::binary);

            if (filename.find(".bmp") != std::string::npos)
            {
                unsigned char bmp_pad[3] = { 0, 0, 0, };
                uint32_t head[13] = { (54 + (3 * this->width * this->height) + (((this->width * -3) & 3) * this->height)), 0, 54, 40, this->width, this->height, (24 << 16) | 1 };

                out.write("BM", 2);
                out.write((char*)head, 52);

                for (uint32_t i = 0; i < this->height; i++)
                {
                    out.write((char*)this->pixels + (this->width * i * 3), 3 * this->width);
                    out.write((char*)bmp_pad, ((4 - (this->width * 3) % 4) % 4));
                }
            }
    
            out.close();
        }
    };
}

#endif
