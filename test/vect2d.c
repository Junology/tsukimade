#include "vect2d.h"

#include <math.h>

Vector2D add_vecs(Vector2D u, Vector2D v)
{
    u.x += v.x;
    u.y += v.y;

    return u;
}

Vector2D diff_vecs(Vector2D u, Vector2D v)
{
    u.x -= v.x;
    u.y -= v.y;

    return u;
}

Vector2D scalar_prod(Vector2D v, double r)
{
    v.x *= r;
    v.y *= r;

    return v;
}

double norm(Vector2D v)
{
    return sqrt(v.x*v.x+v.y*v.y);
}

Vector2D gradient(double d, PotentialFunc f, Vector2D p)
{
    Vector2D result, dxp, dyp;
    double base = f(p);

    dxp.x = p.x + d;
    dxp.y = p.y;
    dyp.x = p.x;
    dyp.y = p.y + d;

    result.x = (f(dxp) - base) / d;
    result.y = (f(dyp) - base) / d;

    return result;
}
