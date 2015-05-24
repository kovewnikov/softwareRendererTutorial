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
    fill((float*)v.raw);
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
void mat::setByOffset(int offset, float val) {
    _elements[offset] = val;
}
float mat::get (int m, int n) {
    return _elements[m*_dimN+n];
}
float mat::getByOffset(int offset) {
    return _elements[offset];
}

Vec2<float> mat::toVec2() {
    Vec2<float> v(_elements[0], _elements[1]);
    return v;
}
Vec3<float> mat::toVec3() {
    Vec3<float> v(_elements[0], _elements[1], _elements[2]);
    return v;
}


void mat::operator=(const mat &m ) {
    if(size()!=m.size()) {
        std::cout<<"Операция присваивания предусмотрена только для матриц одинаковой размерности\n";
        return;
    }
    _dimM = m.mDimension();
    _dimN = m.nDimension();
    int s = size();
    float* mEl = m.rawElements();
    for(int i=0; i<s; i++) {
        _elements[i] = mEl[i];
    }
}

void mat::multiplyMatrices(mat *m1, mat *m2, mat *result) {
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
                    float n1 = m1->getByOffset(mi*mDim+ni);
                    float n2 = m2->getByOffset(ni*qDim+qi);
                    float accumVal = result->getByOffset(mi*qDim + qi);
                    result->setByOffset(mi*qDim + qi, accumVal+n1 * n2);
                }
            }
        }
    }
}

void mat::multiplyMatrixAndVec2(mat *m, Vec2<float> v, Vec2<float> *result) {
    mat m2(v);
    mat resultMat(2,1);
    
    multiplyMatrices(m, &m2, &resultMat);
    result->set(resultMat.get(0, 0), resultMat.get(1, 0));
    
}
void mat::multiplyMatrixAndVec3(mat *m, Vec3<float> v, Vec3<float> *result) {
    mat m2(v);
    mat resultMat(3,1);
    
    
    multiplyMatrices(m, &m2, &resultMat);
    result->set(resultMat.get(0, 0), resultMat.get(1, 0), resultMat.get(2, 0));
}

void mat::addMatrices(mat *m1, mat *m2, mat *result) {
    if(m1->mDimension()!=m2->mDimension() ||
       m2->nDimension()!=m2->nDimension()) {
        std::cout << "Ошибка! Операция сложения требует, чтобы матрицы-слагаемые имели одинаковую размерность" << "\n";
    } else {
        int mSize = m1->size();
        for(int i=0; i<mSize; i++) {
            result->setByOffset(i, m1->getByOffset(i) + m2->getByOffset(i));
        }
    }
}
void mat::subtractMatrices(mat *m1, mat *m2, mat *result) {
    if(m1->mDimension()!=m2->mDimension() ||
       m2->nDimension()!=m2->nDimension()) {
        std::cout << "Ошибка! Операция разности требует, чтобы матрицы имели одинаковую размерность" << "\n";
    } else {
        int mSize = m1->size();
        for(int i=0; i<mSize; i++) {
            result->setByOffset(i, m1->getByOffset(i) - m2->getByOffset(i));
        }
    }
}

std::ostream& operator<<(std::ostream& s, mat&m) {
    for(int i=0; i<m.mDimension(); i++) {
        for(int j=0; j<m.nDimension(); j++) {
            s << m.get(i, j) << ", ";
        }
        s << "\n";
    }
    return s;
}