#include "Vector.h"

#include <math.h>

inline Direction GetDirection(int dX, int dY)
{
    if (!(dX || dY))
    {
        return none;
    }

    const float tan225 = 0.414f;
    if (abs(dY) <= fabsf(dX * tan225))
    {
        return right;
    }
    else if (abs(dX) <= fabsf(dY * tan225))
    {
        return up;
    }
    else if (dX * dY > 0)
    {
        return rightup;
    }
    else if (dX * dY < 0)
    {
        return leftup;
    }
    else
    {
        return none;
    }
}
