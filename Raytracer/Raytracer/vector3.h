#pragma once
#include <ostream>

class Vector3 {
   public:
    Vector3();
    Vector3(double x, double y, double z);
    ~Vector3();

    double GetX() const;
    double GetY() const;
    double GetZ() const;
    void Set(double x, double y, double z);
    void Set(const Vector3 &other);

    Vector3 operator+(const Vector3 &other) const;
    Vector3 &operator+=(const Vector3 &other);
    Vector3 &operator*=(const int v);
    Vector3 operator-(const Vector3 &other) const;
    Vector3 operator*(const double &f) const;
    friend Vector3 operator*(const double &f, const Vector3 &vec);
    friend std::ostream &operator<<(std::ostream &os, const Vector3 &vec);

    double Dot(const Vector3 &other) const;
    Vector3 Cross(const Vector3 &other) const;
    Vector3 ReflectAbout(const Vector3 &normal) const;

    Vector3 Normalize() const;
    double Magnitude() const;
    double SqrMagnitude() const;

   private:
    double x_;
    double y_;
    double z_;
};
