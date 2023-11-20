#include "Maths.h"

float B2(const int &i, const float &t)
{
    switch(i)
    {
    case 0: return (1 - t) * (1 - t);
    case 1: return 2 * t * (1 - t);
    case 2: return t * t;
    default: return 0;
    }
}

float B3(const int &i, const float &t)
{
    switch(i)
    {
        case 0: return (1 - t) * (1 - t) * (1 - t);
        case 1: return 3 * t * (1 - t) * (1 - t);
        case 2: return 3 * t * t * (1 - t);
        case 3: return t * t * t;
        default: return 0;
    }
}

float dB3(const int &i, const float &t)
{
    switch(i)
    {
        case 0: return - 3 * (1 - t) * (1 - t);
        case 1: return 3 * (1 - t) * (1 - 3 * t);
        case 2: return 3 * t * (2 - 3 * t);
        case 3: return 3 * t * t;
        default: return 0;
    }
}
