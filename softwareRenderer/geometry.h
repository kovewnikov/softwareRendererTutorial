//
//  geometry.h
//  softwareRenderer
//
//  Created by kovewnikov on 14.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#ifndef softwareRenderer_geometry_h
#define softwareRenderer_geometry_h

#include <cmath>
#include <ostream>
#include <iostream>
#include <assert.h>
/*
 * Vec2
 *
 */
template <typename t> struct Vec2 {
    union {
        struct { t x, y; };
        struct { t u, v; };
        t raw[2];
    };
    
    Vec2() : u(0), v(0) {}
    Vec2(t iu, t iv) : u(iu), v(iv) {}
    
    static Vec2<float> lerp(Vec2<t> from, Vec2<t> to, float v) {
        Vec2<float> retVal((from.x+to.x)*v, (from.y+to.y)*v, (from.z+to.z)*v);
        return retVal;
    }
    
    void set(t newX, t newY) {
        x = newX;
        y = newY;
    }
    
    inline Vec2<t> operator+ (const Vec2<t> &vec) const {
        return Vec2<t>(u+vec.u, v+vec.v);
    }
    inline Vec2<t> operator- (const Vec2<t> &vec) const {
        return Vec2<t>(u-vec.u, v-vec.v);
    }
    inline Vec2<t> operator* (t s) const {
        return Vec2<t>(u*s,v*s);
    }
    
    template <typename > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};
template <typename t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}





/*
 * Vec3
 */

template<typename t> struct Vec3 {
    union {
        struct { t x, y, z; };
        struct { t ivert, iuv, vnorm; };
        t raw[3];
    };
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(t ix, t iy, t iz) : x(ix), y(iy), z(iz) {}
    
    static Vec3<float> lerp(Vec3<t> from, Vec3<t> to, float v) {
        Vec3<float> retVal((from.x+to.x)*v, (from.y+to.y)*v, (from.z+to.z)*v);
        return retVal;
    }
    
    void set(t newX, t newY, t newZ) {
        x = newX;
        y = newY;
        z = newZ;
    }
    
    inline Vec3<t> operator+(const Vec3<t> &vec) const{
        return Vec3<t>(x+vec.x, y+vec.y, z+vec.z);
    }
    inline Vec3<t> operator-(const Vec3<t> &vec) const{
        return Vec3<t>(x-vec.x, y-vec.y, z-vec.z);
    }
    inline t operator*(const Vec3<t> &vec) const {
        return x*vec.x + y*vec.y + z*vec.z;
    }
    inline Vec3<t> operator*(t s) const {
        return Vec3<t>(x*s,y*s,z*s);
    }
    inline Vec3<t> operator^(const Vec3<t> &vec) {
        return Vec3<t>(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y-y*vec.x);
    }
    
    inline operator Vec3<float>() {
        return Vec3<float>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
    }
    inline operator Vec3<int>() {
        return Vec3<float>(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
    }
    
    float length() const {
        return std::sqrt(x*x+y*y+z*z);
    }
    Vec3<float> normalized() const {
        float l = 1./length();
        return Vec3<float>(x*l,y*l,z*l);
    }
    template <typename > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};
template <typename t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}

template <typename t>
struct Vec4 {
    union {
        struct { t x, y, z, r; };
        t raw[4];
    };
    Vec4() : x(0), y(0), z(0), r(0) {}
    Vec4(t xi, t yi, t zi, t ri) : x(xi), y(yi), z(zi), r(ri) {}
    Vec4(Vec3<t> v) : x(v.x), y(v.y), z(v.z), r(1) {}
    void set(t newX, t newY, t newZ,  t newR) {
        x = newX;
        y = newY;
        z = newZ;
        r = newR;
    }
    Vec3<float>projectTo3D() {
        return Vec3<float>(x/r, y/r, z/r);
    }
    template <typename > friend std::ostream& operator<<(std::ostream& s, Vec4<t>& v);
};
template <typename t> std::ostream& operator<<(std::ostream& s, Vec4<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.r << ")\n";
    return s;
}





struct mat {
private:
    int _dimM;
    int _dimN;
    float *_elements;
public:
    mat(Vec2<float> v);
    mat(Vec3<float> v);
    mat(Vec4<float> v);
    mat(int m, int n);
    mat( const mat &other );
    ~mat();
    int mDimension() const;
    int nDimension() const;
    int size() const;
    bool isQuad();
    float* rawElements() const;
    void fill(float* source);
    void fillIdentity();
    void fillZero();
    
//    void set(int m, int n, float val);
//    void setByOffset(int offset, float val);
//    float get (int m, int n) const;
    float at(int offset) const;
    
    Vec2<float> toVec2();
    Vec3<float> toVec3();
    Vec4<float> toVec4();
    
    void operator=(const mat &m );
    
    mat operator*(const mat &m2) const;
    Vec2<float> operator*(const Vec2<float> v) const;
    Vec3<float> operator*(const Vec3<float> v) const;
    Vec4<float> operator*(const Vec4<float> v) const;
    mat operator+(const mat &m2) const;
    mat operator-(const mat &m2) const;
    float& operator [](size_t index);
    const float& operator [](size_t index) const;
    
    friend std::ostream& operator<<(std::ostream& s, mat &m);
};



typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;
typedef Vec4<int> Vec4i;
typedef Vec4<float> Vec4f;

#endif
