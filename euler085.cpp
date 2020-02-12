#include <iostream>
#include <cmath>

int calc(int width, int height)
{
    int result = 0;
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            result += (width - w) * (height - h);
        }
    }
    return result;
}

int main()
{
    constexpr int TARGET = 2000000;
    int result = TARGET;
    int resW = 0, resH = 0;

    for (int w = 1; w < 100; w++)
    {
        for (int h = 1; h < 100; h++)
        {
            int num = std::abs(calc(w, h) - TARGET);
            if (result > num)
            {
                result = num;
                resW = w;
                resH = h;
            }
        }
    }

    std::cout << resW << " * " << resH << " = " << (resW * resH) << " : " << calc(resW, resH) << std::endl;
}
