#include <stdio.h>

class Matrix;

class Vector {
private:
  double a, b;
public:
  Vector(double a, double b): a(a), b(b) {}
  ~Vector() {}
  double getA() { return a; }
  double getB() { return b; }
  friend Vector& operator*(const Matrix& m, const Vector& v);
  friend Vector& operator*(const Vector& v, const Matrix& m);
};

class Matrix {
private:
  double a,b,c,d;
public:
  Matrix(double a, double b, double c, double d): a(a), b(b), c(c), d(d) {}
  ~Matrix() {}
  Matrix& operator+(const Matrix& m) {
    return *(new Matrix(a+m.a,b+m.b,c+m.c,d+m.d));
  }
  Matrix& operator-(const Matrix& m) {
    return *(new Matrix(a-m.a,b-m.b,c-m.c,d-m.d));
  }
  Matrix& operator*(const Matrix& m) {
    return *(new Matrix(a*m.a+b*m.c,
      a*m.b+b*m.d,
      c*m.a+d*m.c,
      c*m.b+d*m.d));
  }
  Matrix& operator/(const Matrix& m) {
    return *(new Matrix(a/m.a,b/m.b,c/m.c,d/m.d));
  }
  double getA() { return a; }
  double getB() { return b; }
  double getC() { return c; }
  double getD() { return d; }

  friend Vector& operator*(const Matrix& m, const Vector& v);
  friend Vector& operator*(const Vector& v, const Matrix& m);
};

Vector& operator*(const Matrix& m, const Vector& v) {
  return *(new Vector(m.a*v.a+m.b*v.b, m.c*v.a+m.d*v.b));
}

Vector& operator*(const Vector& v, const Matrix& m) {
  return *(new Vector(v.a*m.a + v.b*m.c, v.a*m.b + v.b*m.d));
}

int main(void) {
  Matrix a(1,2,3,4), b(5,6,7,8);
  Matrix c = a*b;
  printf("%f %f\n%f %f\n", c.getA(), c.getB(), c.getC(), c.getD());
  Vector v(5,10);
  Vector v2 = b*v;
  printf("%f %f\n", v2.getA(), v2.getB());
  Vector v3 = v*b;
  printf("%f %f\n", v3.getA(), v3.getB());
  return 0;
}
