#ifndef TEST_LIB_H
#define TEST_LIB_H

//! 2-dimensional vectors.
typedef struct {
    double x, y;
} Vector2D;

//! potential functions
typedef double (*PotentialFunc)(Vector2D);

//! Addition of two vectors.
Vector2D add_vecs(Vector2D u, Vector2D v);

//! Difference of two vectors.
Vector2D diff_vecs(Vector2D u, Vector2D v);

//! Scalar product.
Vector2D scalar_prod(Vector2D v, double r);

//! Norm.
double norm(Vector2D v);

//! Compute the gradient of a function.
Vector2D gradient(double d, PotentialFunc f, Vector2D p);

#endif
