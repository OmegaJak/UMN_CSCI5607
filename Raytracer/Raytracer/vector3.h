#pragma once
class Vector3 {
   public:
    Vector3();
    Vector3(double x, double y, double z) : x_(x), y_(y), z_(z) {}
    ~Vector3();

    double GetX();
    double GetY();
    double GetZ();

    void Set(double x, double y, double z);

   private:
    double x_;
    double y_;
    double z_;
};
