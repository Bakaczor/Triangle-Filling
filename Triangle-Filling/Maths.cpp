
float B(const int &i, const float &t)
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

float dB(const int &i, const float &t)
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
