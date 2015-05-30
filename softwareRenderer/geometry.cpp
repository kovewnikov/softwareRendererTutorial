//
//  geometry.cpp
//  softwareRenderer
//
//  Created by kovewnikov on 24.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#include <stdio.h>
#import "geometry.h"

mat::mat(Vec2<float> v) : _dimM(2), _dimN(1) {
    _elements = new float[_dimM];
    fill(v.raw);
}
mat::mat(Vec3<float> v) : _dimM(3), _dimN(1) {
    _elements = new float[_dimM];
    fill(v.raw);
}
mat::mat(Vec4<float> v) : _dimM(4), _dimN(1) {
    _elements = new float[_dimM];
    fill(v.raw);
}
mat::mat(int m, int n) : _dimM(m), _dimN(n) {
    int s = _dimM*_dimN;
    _elements = new float[s];
    if(isQuad()) {
        fillIdentity();
    } else {
        fillZero();
    }
}
mat::mat( const mat &other ) : _dimM( other.mDimension() ), _dimN( other.nDimension() ) {
    _elements = new float[_dimM*_dimN];
    fill(other.rawElements());
}
mat::~mat() {
    delete [] _elements;
}
int mat::mDimension() const {
    return _dimM;
}
int mat::nDimension() const {
    return _dimN;
}
int mat::size() const {
    return _dimM*_dimN;
}
bool mat::isQuad() {
    return _dimM == _dimN;
}
float* mat::rawElements() const {
    return _elements;
}
void mat::fill(float* source) {
    int mSize = size();
    for(int i=0; i<mSize; i++) {
        _elements[i] = source[i];
    }
}
void mat::fillIdentity() {
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
void mat::fillZero() {
    int mSize = size();
    for(int i=0; i<mSize; i++) {
        _elements[i] = 0;
    }
}

void mat::set(int m, int n, float val) {
    _elements[m*_dimN+n] = val;
}
//void mat::setByOffset(int offset, float val) {
//    _elements[offset] = val;
//}
float mat::get (int m, int n) const {
    return _elements[m*_dimN+n];
}
float mat::at(int offset) const {
    return _elements[offset];
}



//алгебраическое дополнение для элемента:
mat mat::minorByRowAndCol(int targetM, int targetN) const {
    int minorSize = (this->mDimension()-1) * (this->nDimension()-1);
    float *source = new float[minorSize];
    int counter=0;
    for(int i=0; i<this->size(); i++) {
        bool itsOk =
        (i != floor(i/this->mDimension()) * this->nDimension() + targetN) &&
        (i != targetM * this->nDimension() + i%this->nDimension());
        
        if(itsOk) {
            source[counter] = this->at(i);
            counter++;
        }
    }
    mat minor(this->mDimension()-1, this->nDimension()-1);
    minor.fill(source);
    delete [] source;
    return minor;
}

float mat::det() const {
    return mat::detByMat(*this);
}
mat mat::transposed() const {
    mat transposed(this->mDimension(), this->nDimension());
    for(int i=0; i<this->mDimension(); i++) {
        for(int j=0; j<this->nDimension(); j++) {
            transposed.set(j, i, this->get(i, j));
        }
    }
    return transposed;
}
mat mat::inversed() const {
    //обратная матрица равняется 1/det(A) * A(+T), где А - исходная матрица, det(A) - ее определитель,
    //A(+T) - транспонированная матрица алгебраических дополнений
    //матрица алгебраических дополнений (A(+)) - матрица элементы которой составляет соответствующие каждому элементу алгебраические дополнения
    //алгебраическое дополнение - определитель минора  к некоторому элементу в матрице
//    float deter = std::abs(this->det());
    float deter = this->det();
    float oneDivDet = 1/deter;
    
    mat result(this->mDimension(), this->nDimension());
    for(int i=0; i<result.mDimension(); i++) {
        for(int j=0; j<result.nDimension(); j++) {
            result[i*result.nDimension() + j] = ((i+j)%2==0? 1: -1) * minorByRowAndCol(i, j).det();
        }
        
    }
    result = result.transposed();
    
    result*= oneDivDet;
    return result;
}



Vec2<float> mat::toVec2() {
    Vec2<float> v(_elements[0], _elements[1]);
    return v;
}
Vec3<float> mat::toVec3() {
    Vec3<float> v(_elements[0], _elements[1], _elements[2]);
    return v;
}
Vec4<float> mat::toVec4() {
    Vec4<float> v(_elements[0], _elements[1], _elements[2], _elements[3]);
    return v;
}

void mat::operator=(const mat &m ) {
    assert(size()==m.size());//Операция присваивания предусмотрена только для матриц одинаковой размерности
    _dimM = m.mDimension();
    _dimN = m.nDimension();
    int s = size();
    float* mEl = m.rawElements();
    for(int i=0; i<s; i++) {
        _elements[i] = mEl[i];
    }
}
mat mat::operator*(const mat &m2) const{
    mat result(mDimension(), m2.nDimension());
    if(nDimension()!=m2.mDimension()) {
        std::cout << "Ошибка! Операция умножения требует чтобы n-размерность первой матры была равна m-размерности второй матры" << "\n";
        return result;
    } else {
        //обнуляем матру-результат
        result.fillZero();
        
        int mDim = mDimension();
        int nDim = nDimension();
        int qDim = m2.nDimension();
        for(int mi=0; mi<mDim; mi++) {
            for(int qi=0; qi<qDim; qi++) {
                for(int ni=0; ni<nDim; ni++) {
                    float n1 = this->at(mi*mDim+ni);
                    float n2 = m2[ni*qDim+qi];
                    float accumVal = result[mi*qDim + qi];
                    result[mi*qDim + qi] = accumVal+n1 * n2;
                }
            }
        }
    }
    return result;
}
mat mat::operator*(float x) const {
    mat resultMat(this->mDimension(), this->nDimension());
    for(int i=0; i<resultMat.size(); i++) {
        resultMat[i] = this->at(i)*x;
    }
    
    return resultMat;
}
void mat::operator*=(float x) {
    for(int i=0; i<this->size(); i++) {
        _elements[i] = _elements[i]*x;
    }
    
}
Vec2<float> mat::operator*(const Vec2<float> v) const {
    mat m2(v);
    mat resultMat = *this * m2;;
    
    Vec2f result;
    result.set(resultMat[0], resultMat[1]);
    return result;
}
Vec3<float> mat::operator*(const Vec3<float> v) const {
    mat m2(v);
    mat resultMat = *this * m2;;
    
    Vec3f result;
    result.set(resultMat[0], resultMat[1], resultMat[2]);
    return result;
}
Vec4<float> mat::operator*(const Vec4<float> v) const {
    mat m2(v);
    mat resultMat = *this * m2;;
    
    Vec4f result;
    result.set(resultMat[0], resultMat[1], resultMat[2], resultMat[3]);
    return result;
}
mat mat::operator+(const mat &m2) const {
    mat result(mDimension(), nDimension());
    if(mDimension()!=m2.mDimension() ||
       nDimension()!=m2.nDimension()) {
        std::cout << "Ошибка! Операция сложения требует, чтобы матрицы-слагаемые имели одинаковую размерность" << "\n";
    } else {
        int mSize = size();
        mat result(mDimension(), nDimension());
        for(int i=0; i<mSize; i++) {
            result[i] = this->at(i) + m2.at(i);
        }
    }
    return result;
}
mat mat::operator-(const mat &m2) const {
    mat result(mDimension(), nDimension());
    if(mDimension()!=m2.mDimension() ||
       nDimension()!=m2.nDimension()) {
        std::cout << "Ошибка! Операция разности требует, чтобы матрицы имели одинаковую размерность" << "\n";
    } else {
        int mSize = size();
        for(int i=0; i<mSize; i++) {
            result[i] = this->at(i) - m2.at(i);
        }
    }
    return result;
}

float& mat::operator [](size_t index) {
    assert(index<size());
    return _elements[index];
}
const float& mat::operator [](size_t index) const {
    return _elements[index];
    
}



float mat::detByMat(const mat &m) {
    assert(m.mDimension() == m.nDimension());
    float accumDet=0;
    if(m.mDimension() > 2) {
        for(int i=0; i<m.nDimension(); i++) {
            accumDet += m.get(0, i) * (i%2==0? 1 : -1) * mat::detByMat(m.minorByRowAndCol(0, i));
        }
    } else {
        accumDet = m[0]*m[3] - m[1]*m[2];
    }
    return accumDet;
}


std::ostream& operator<<(std::ostream& s, mat&m) {
    for(int i=0; i<m.mDimension(); i++) {
        for(int j=0; j<m.nDimension(); j++) {
            s << m[i*m.mDimension() + j] << ", ";
        }
        s << "\n";
    }
    return s;
}