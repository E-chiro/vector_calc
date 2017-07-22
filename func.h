#ifndef FUNC_H
#define FUNC_H
#include <algorithm>
using namespace std;

template <typename T>
double mean(const vector<T>& v){
    auto sum = accumulate(v.begin(), v.end(), 0);
    return double(sum) / v.size();
}

template <typename T>
double gmean(const vector<T>& v){
    double acc = 1.;
    for(auto i: v)
        acc *= i;
    return pow(acc, 1./v.size());
}

template <typename T>
double hmean(const vector<T>& v){
    double acc = 0;
    for(auto i: v)
        acc += 1./i;
    return 1 / (acc/v.size());
}

template <typename T>
double median(const vector<T>& v){
    if(v.empty())
        return 0;
    auto tmp = v;
    auto mid = tmp.size()/2;
    sort(tmp.begin(), tmp.end());

    if(v.size()%2)
        return v[mid];
    else
        return (v[mid-1] + v[mid]) / 2.0;
}

template <typename T>
double disp(const vector<T>& v,
            bool general,
            double _MeanFunc(const vector<T>&) = mean){
    auto _mean = _MeanFunc(v);
    double acc = 0;
    for(auto i: v)
        acc += pow(i - _mean, 2);
    return (general)? acc / v.size() : acc / ( v.size()-1 ) ;
}

template <typename T>
double Sd(const vector<T>& v,
          double _MeanFunc(const vector<T>&) = mean){
    return sqrt( disp(v, true, _MeanFunc) );
}

template <typename T>
double sd(const vector<T>& v,
          double _MeanFunc(const vector<T>&) = mean){
    return sqrt( disp(v, false, _MeanFunc) );
}

template <typename T>
int interval_count(const vector<T>& v){
    return round( 1 + log10(v.size())/log10(2) );
}

template <typename T>
int interval_size(const vector<T>& v){
    auto dif = max(v.begin(), v.end()) - min(v.begin(), v.end());
    return roiund (double(dif) / interval_count(v));
}

template <typename T>
map<T, int> dist(const vector<T>& v){
    map<T, int> _dist;
    for(auto i: v)
        _dist[i]++;
    return _dist;
}



#endif // FUNC_H
