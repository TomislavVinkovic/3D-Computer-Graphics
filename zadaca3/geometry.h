#pragma once
#include<vector>
#include<algorithm>
#include<iostream>

using namespace std;

template <typename T>
class Vec2
{
public:
    Vec2() : x(0), y(0) {}
    Vec2(T xx) : x(xx), y(xx) {}
    Vec2(T xx, T yy) : x(xx), y(yy) {}
    Vec2 operator+(const Vec2 &v) const
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator/(const T &r) const
    {
        return Vec2(x / r, y / r);
    }
    Vec2 operator*(const T &r) const
    {
        return Vec2(x * r, y * r);
    }
    Vec2 &operator/=(const T &r)
    {
        x /= r, y /= r;
        return *this;
    }
    Vec2 &operator*=(const T &r)
    {
        x *= r, y *= r;
        return *this;
    }
    friend std::ostream &operator<<(std::ostream &s, const Vec2<T> &v)
    {
        return s << '[' << v.x << ' ' << v.y << ']';
    }
    friend Vec2 operator*(const T &r, const Vec2<T> &v)
    {
        return Vec2(v.x * r, v.y * r);
    }
    T x, y;
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

template <typename T>
class Vec3
{
public:
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T xx) : x(xx), y(xx), z(xx) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
    Vec3 operator+(const Vec3 &v) const
    {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator-(const Vec3 &v) const
    {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }
    Vec3 operator*(const T &r) const
    {
        return Vec3(x * r, y * r, z * r);
    }
    Vec3 operator*(const Vec3 &v) const
    {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    T dotProduct(const Vec3<T> &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vec3 &operator/=(const T &r)
    {
        x /= r, y /= r, z /= r;
        return *this;
    }
    Vec3 &operator*=(const T &r)
    {
        x *= r, y *= r, z *= r;
        return *this;
    }
    Vec3 crossProduct(const Vec3<T> &v) const
    {
        return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    T norm() const
    {
        return x * x + y * y + z * z;
    }
    T length() const
    {
        return sqrt(norm());
    }

    const T &operator[](uint8_t i) const { return (&x)[i]; }
    T &operator[](uint8_t i) { return (&x)[i]; }
    Vec3 &normalize()
    {
        T n = norm();
        if (n > 0)
        {
            T factor = 1 / sqrt(n);
            x *= factor, y *= factor, z *= factor;
        }

        return *this;
    }

    friend Vec3 operator*(const T &r, const Vec3 &v)
    {
        return Vec3<T>(v.x * r, v.y * r, v.z * r);
    }
    friend Vec3 operator/(const T &r, const Vec3 &v)
    {
        return Vec3<T>(r / v.x, r / v.y, r / v.z);
    }

    friend std::ostream &operator<<(std::ostream &s, const Vec3<T> &v)
    {
        return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
    }

    T x, y, z;
};


typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template <typename T>
class Matrix4x4
{
private:
    T matrix[4][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

public:
    Matrix4x4(vector<T> &&r1, vector<T> &&r2, vector<T> &&r3, vector<T> &&r4)
    {
        for (int i = 0; i < 4; i++)
        {
            matrix[0][i] = r1[i];
        }
        for (int i = 0; i < 4; i++)
        {
            matrix[1][i] = r2[i];
        }
        for (int i = 0; i < 4; i++)
        {
            matrix[2][i] = r3[i];
        }
        for (int i = 0; i < 4; i++)
        {
            matrix[3][i] = r4[i];
        }
    }

    template <typename S>
    void multVecMatrix(const Vec3<S> &src, Vec3<S> &dst) const
    {
        S a, b, c, w;

        a = src[0] * matrix[0][0] + src[1] * matrix[1][0] + src[2] * matrix[2][0] + matrix[3][0];
        b = src[0] * matrix[0][1] + src[1] * matrix[1][1] + src[2] * matrix[2][1] + matrix[3][1];
        c = src[0] * matrix[0][2] + src[1] * matrix[1][2] + src[2] * matrix[2][2] + matrix[3][2];
        w = src[0] * matrix[0][3] + src[1] * matrix[1][3] + src[2] * matrix[2][3] + matrix[3][3];

        dst.x = a / w;
        dst.y = b / w;
        dst.z = c / w;
    }

    void print() const {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                cout << matrix[i][j] << " ";
            }
        cout << endl;
        }
    }
};

typedef Matrix4x4<float> Matrix4x4f;