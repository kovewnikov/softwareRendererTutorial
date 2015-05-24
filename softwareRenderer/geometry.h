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
struct mat {
private:
    int _dimM;
    int _dimN;
    t *_elements;
public:
    mat(Vec2<t> v) {
        _dimM = 2;
        _dimN = 1;
        _elements = new t[_dimM];
        fill(v.raw);
    }
    mat(Vec3<t> v) {
        _dimM = 3;
        _dimN = 1;
        _elements = new t[_dimM];
        fill(v.raw);
    }
    mat(int m, int n) {
        _dimM = m;
        _dimN = n;
        int s = _dimM*_dimN;
        _elements = new t[s];
        if(isQuad()) {
            fillIdentity();
        } else {
            fillZero();
        }
    }
    ~mat() {
        delete [] _elements;
    }
    int mDimension() {
        return _dimM;
    }
    int nDimension() {
        return _dimN;
    }
    int size() {
        return _dimM*_dimN;
    }
    bool isQuad() {
        return _dimM == _dimN;
    }
    void fill(t* source) {
        int mSize = size();
        for(int i=0; i<mSize; i++) {
            _elements[i] = source[i];
        }
    }
    void fillIdentity() {
        if(!isQuad()) {
            std::cout << "Ошибка! Матрица может быть единичной только в случае, если она является квадратной" << "\n";
            return;
        }
        for(int i=0; i<mDimension(); i++) {
            for(int j=0; j<nDimension(); j++) {
                _elements[i*nDimension() + j] = i==j ? 1 : 0;
            }
        }
    }
    void fillZero() {
        int mSize = size();
        for(int i=0; i<mSize; i++) {
            _elements[i] = 0;
        }
    }
    
    void set(int m, int n, t val) {
        _elements[m*_dimN+n] = val;
    }
    void setByOffset(int offset, t val) {
        _elements[offset] = val;
    }
    t get (int m, int n) {
        return _elements[m*_dimN+n];
    }
    t getByOffset(int offset) {
        return _elements[offset];
    }
    
    void toVec2() {
        Vec2<t> v(_elements[0], _elements[1]);
        return v;
    }
    void toVec3() {
        Vec3<t> v(_elements[0], _elements[1], _elements[2]);
        return v;
    }

    static void multiplyMatrices(mat<t> *m1, mat<t> *m2, mat<t> *result) {
        if(m1->nDimension()!=m2->mDimension()) {
            std::cout << "Ошибка! Операция умножения требует чтобы n-размерность первой матры была равна m-размерности второй матры" << "\n";
            return;
        } else if(m1->mDimension() != result->mDimension() ||
                  m2->nDimension() != result->nDimension()) {
            std::cout << "Ошибка! результирующая матрица должна иметь размерность " << m1->mDimension() << "x" << m2->nDimension() << "\n";
            return;
        } else {
            //обнуляем матру-результат
            result->fillZero();
            
            int mDim = m1->mDimension();
            int nDim = m1->nDimension();
            int qDim = m2->nDimension();
            for(int mi=0; mi<mDim; mi++) {
                for(int qi=0; qi<qDim; qi++) {
                    for(int ni=0; ni<nDim; ni++) {
                        t n1 = m1->getByOffset(mi*mDim+ni);
                        t n2 = m2->getByOffset(ni*qDim+qi);
                        t accumVal = result->getByOffset(mi*qDim + qi);
                        result->setByOffset(mi*qDim + qi, accumVal+n1 * n2);
                    }
                }
            }
        }
    }
    
    static void multiplyMatrixAndVec2(mat<t> *m, Vec2<t> *v, Vec2<t> *result) {
        mat m2(*v);
        mat resultMat(2,1);
        multiplyMatrices(m, *m2, *resultMat);
        result->set(resultMat.get(0, 0), resultMat.get(1, 0));
        
    }
    static void  multiplyMatrixAndVec3(mat<t> *m, Vec3<t> *v, Vec3<t> *result) {
        mat m2(*v);
        mat resultMat(3,1);
        multiplyMatrices(m, *m2, *resultMat);
        result->set(resultMat.get(0, 0), resultMat.get(1, 0), resultMat.get(2, 0));
    }
    
    static void addMatrices(mat<t> *m1, mat<t> *m2, mat *result) {
        if(m1->mDimension()!=m2->mDimension() ||
           m2->nDimension()!=m2->nDimension()) {
            std::cout << "Ошибка! Операция сложения требует, чтобы матрицы-слагаемые имели одинаковую размерность" << "\n";
        } else {
            int mSize = m1->size();
            for(int i=0; i<mSize; i++) {
                result[i] = m1[i]+m2[i];
            }
        }
    }
    static void subtractMatrices(mat<t> *m1, mat<t> *m2, mat *result) {
        if(m1->mDimension()!=m2->mDimension() ||
           m2->nDimension()!=m2->nDimension()) {
            std::cout << "Ошибка! Операция разности требует, чтобы матрицы имели одинаковую размерность" << "\n";
        } else {
            int mSize = m1->size();
            for(int i=0; i<mSize; i++) {
                result[i] = m1[i]+m2[i];
            }
        }
    }
    
    template <typename > friend std::ostream& operator<<(std::ostream& s, mat<t>& m);
};
template <typename t> std::ostream& operator<<(std::ostream& s, mat<t>& m) {
    for(int i=0; i<m.mDimension(); i++) {
        for(int j=0; j<m.nDimension(); j++) {
            s << m.get(i, j) << ", ";
        }
        s << "\n";
    }
    return s;
}



typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

typedef mat<int> Mati;
#endif
