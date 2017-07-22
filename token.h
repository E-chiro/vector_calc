#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include "vec.h"


class Token{

public:
    Vector value = 0;
    std::string text = "...";
    char kind = '?';

    Token(){}
    Token(const char& kind): kind(kind) {}
    Token(const Vector& v, const char& kind = 'n'): value(v), kind(kind) {}
    Token(const string& id, const char& kind='$'): text(id), kind(kind) {}
    bool empty()const {
        return (kind == '?');
    }
};

struct TokenPair{
    Token first, last;
};

#endif // TOKEN_H
