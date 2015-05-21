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

template <typename t>
struct Vec2 {
    union {
        struct { t x, y; };
        struct { t u, v; };
        t raw[2];
    };
    
    Vec2() : u(0), v(0) {}
    Vec2(t iu, t iv) : u(iu), v(iv) {}
    
    inline Vec2<t> operator+ (const Vec2<t> &vec) const {
        return Vec2<t>(u+vec.u, v+vec.v);
    }
    
    inline Vec2<t> operator-(const Vec2<t> &vec) const {
        return Vec2<t>(u-vec.u, v-vec.v);
    }
    inline Vec2<t> operator*(t s) const {
        return Vec2<t>(u*s,v*s);
    }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template<typename t> struct Vec3 {
    union {
        struct { t x, y, z; };
        struct { t ivert, iuv, vnorm; };
        t raw[3];
    };
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(t ix, t iy, t iz) : x(ix), y(iy), z(iz) {}
    
    inline Vec3<t> operator+(const Vec3<t> &vec) const {
        return Vec3<t>(x+vec.x, y+vec.y, z+vec.z);
    }
    inline Vec3<t> operator-(const Vec3<t> &vec) const {
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
    float length() const {
        return std::sqrt(x*x+y*y+z*z);
    }
    void normalize() {
        float l = 1./length();
        x*=l;y*=l;z*=l;
    }
    Vec3<float> normalized() const {
        float l = 1./length();
        return Vec3<float>(x*l,y*l,z*l);
    }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

#endif
