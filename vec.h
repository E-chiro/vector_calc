#ifndef VEC_H
#define VEC_H
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <iomanip>
#include "screen.h"
using namespace std;

struct Range{
    const double
        start = 0, stop = 0, step = 1;

    Range(double stop):
        stop(stop) { }
    Range(double start, double stop, double step):
        start(start), stop(stop), step(step) {
        if(step <= 0) throw string("Step must be a positive real number.");
    }
};

class Vector{
   std::vector<double> _vector;
    void check_dimension(const Vector& lval, const Vector& rval){
        if(lval.size() != rval.size())
            throw string("Dimentions of the vectors must be equal.");
    }
    int maxlen(){
        int maxln=0;
        for(auto i: _vector){
            stringstream ss;
            ss << i;
            int sz = ss.str().size();
            if(maxln < sz)
                maxln = sz;
        }
        return maxln;
    }

public:
    Vector() { }
    Vector(const double number): _vector(vector<double>(1, number)) { }
    Vector(const vector<double>& vec): _vector(vec) { }
    Vector(const Range& range){
        if(range.start == range.stop) { _vector.push_back(range.start); return; }
        if(range.start < range.stop)
        for(auto j = range.start; j <= range.stop; j += range.step)
            _vector.push_back(j);
        else if(range.start > range.stop)
            for(auto j = range.start; j >= range.stop; j -= range.step)
            _vector.push_back(j);
    }

    Vector apply(double f(double)){
        for(auto& i: _vector)
            i = f(i);
        return *this;
    }
    operator double() { return _vector.size()>0? _vector[0] : 0; }
    vector<double> data()const {return _vector;}
    size_t size()const { return _vector.size(); }
    double operator[](int ix)const {
        if(ix>=0 && ix<_vector.size())
            return _vector[ix];
        else throw string("Index error.");
    }
    friend ostream& operator<< (ostream& out, Vector& vec){
        int colwidth = vec.maxlen() + 1;
        int colcount = win_sz().first / colwidth;
        stringstream ss;
        for(size_t j=1; j<=vec._vector.size(); j++){
            out << setw(colwidth) <<  vec._vector[j-1];
            if(j>1 && j%colcount == 0)
                out << endl;
        }

       out << ss.str();
       return out;

    }
    friend istream& operator>> (istream& in, Vector& vec){
        double d; in >> d; vec = Vector(d);
        return in;
    }

    Vector operator+ (const Vector& vec){
        if(vec.size() == 0) return Vector(*this);
        auto result = *this;
        if(vec.size() == 1){
            for(auto& i: result._vector)
                i += vec[0];
            return result;
        }
        check_dimension(*this, vec);
        size_t ix = 0;
        for(auto& i: result._vector)
            i += vec[ix++];
        return result;
    }

    Vector operator- (const Vector& vec){
        if(vec.size() == 0) return Vector(*this);
        auto result = *this;
        if(vec.size() == 1){
            for(auto& i: result._vector)
                i -= vec[0];
            return result;
        }
        check_dimension(*this, vec);
        size_t ix = 0;
        for(auto& i: result._vector)
            i -= vec[ix++];
        return result;
    }

    Vector operator* (const Vector& vec){
        if(vec.size() == 0) return Vector(*this);
        auto result = *this;
        if(vec.size() == 1){
            for(auto& i: result._vector)
                i *= vec[0];
            return result;
        }
        check_dimension(*this, vec);
        size_t ix = 0;
        for(auto& i: result._vector)
            i *= vec[ix++];
        return result;
    }

    Vector operator/ (const Vector& vec){
        if(vec.size() == 0) return Vector(*this);
        auto result = *this;
        if(vec.size() == 1){
            for(auto& i: result._vector)
                i = (vec[0] == 0)? INFINITY : i / vec[0];
            return result;
        }
        check_dimension(*this, vec);
        size_t ix = 0;
        for(auto& i: result._vector){
            i = (vec[ix] == 0)? INFINITY : i / vec[ix];
            ix++;
        }
        return result;
    }

    Vector operator += (const Vector& vec){
        return *this = *this+ vec;
    }
    Vector operator -= (const Vector& vec){
        return *this = *this- vec;
    }
    Vector operator *= (const Vector& vec){
        return *this = *this* vec;
    }
    Vector operator /= (const Vector& vec){
        return *this = *this/ vec;
    }

    Vector plus(){
        for(auto& i: _vector) i = +i;
        return *this;
    }
    Vector minus(){
        for(auto& i: _vector) i = -i;
        return *this;
    }
    Vector power(const double& pw){
        for(auto& i: _vector) i = pow(i, pw);
        return *this;
    }

};

#endif // VEC_H
