#ifndef PARSER_H
#define PARSER_H
#include <sstream>
#include "lexer.h"
#include "symtab.h"
#include "func.h"

void assert(const char& expect, const char& got){
    if(expect != got){
        stringstream ss;
        ss << "[" << expect << "] expected instead of [" << got << "]";
        throw ss.str();
    }
}

typedef Vector vec;
TokenStream ts(cin);
Token glob_tok1, glob_tok2, glob_tok3;
SymbolTable sym;
string rule = "";
TokenPair tpair;
vec expression(TokenPair& );
vec statement(TokenPair&);

vec primary(TokenPair& tp, bool sign = false){
    Token t = ts.get(sign);
    tp.first = t;
    switch(t.kind){
        case 'n': return t.value;
        case '$': return sym.get(t.text);
        case '(':{
            auto expr = expression(tp);
            Token cp = ts.get();
            tp.last = cp;
            assert(')', cp.kind);
            return expr;
        }
        case '[':{
            vector<double>values;
            Token comma(',');
            while(comma.kind == ','){
                auto num = expression(tp);
                values.push_back(num);
                comma = ts.get(sign);
                tp.last = comma;
            }
            assert(']', comma.kind);
            return values;
        }
        case '+': {
            auto res = expression(tp);
            return res.plus();
        }
        case '-': {
            auto res = expression(tp);
            return res.minus();
        }
        default:
                throw string("Primary expected.");
        }
}

vec range(TokenPair& tp){
    vec lval = primary(tp);
    Token t = ts.get();
    tp.first = t;
    if(t.kind != ':'){
        ts.putback(t);
        return lval;
    }
    vec rval = primary(tp);
    Token t2 = ts.get();
    tp.last = t2;
    if(t2.kind == ':'){
        vec step = primary(tp, true);
        return Vector(Range(lval, rval, step));
    }
    ts.putback(t2);
    return Vector(Range(lval, rval,1));
}


vec term(TokenPair& tp){
    vec lval = range(tp);
   Token t = ts.get();
   tp.first = t;

   while(true)
        switch(t.kind){
        case '*':{
            Token t2 = ts.get();
            tp.last = t2;
            if(t2.kind == '*'){
                return lval.power(expression(tp));
            }
            else
                ts.putback(t2);
            lval *= range(tp);
            t = ts.get();
            tp.last = t;
            break;
        }
         case '/':
            lval /= range(tp);
            t = ts.get();
            tp.last = t;
            break;
        default:
            ts.putback(t);
            return lval;
        }
}

vec function(TokenPair& tp, const string& fn){

    if(fn == "sqrt"){
        auto ex = expression(tp);
        ex.apply(
                    [](double d) -> double {
                        return sqrt(d);
                    }
        );
        return ex;
    }

    else throw string("Unknown function");
}

vec expression(TokenPair& tp){
    Token t = ts.get();
    tp.first = t;
    if(t.kind == '$' && t.text == "sqrt")
        return function(tp, t.text);
    else ts.putback(t);

    vec lval = term(tp);
    t = ts.get();
    tp.last = t;
    while(true)
        switch(t.kind){
        case '+':
            lval += term(tp);
            t = ts.get();
            tp.last = t;
            break;
         case '-':
            lval -= term(tp);
            t = ts.get();
            tp.last = t;
            break;
        default: ts.putback(t);
            return lval;
        }
}


vec assignment(TokenPair& tp){
    rule = "=";
    vec result = 0;
    Token t = ts.get();
    tp.first = t;
    assert('$', t.kind);
    string varname = t.text;

    t = ts.get();
    tp.last = t;
    assert('=', t.kind);

    t = ts.get();
    tp.last = t;
    if(t.kind == '$'){
        ts.putback(t);
        result = statement(tp);
    }
    else{
        ts.putback(t);
        result = expression(tp);
    }

    sym.set(varname, result);
    return result;

}

void eol(TokenPair& tp){
    Token t = ts.get();
    if(!(t.kind == ';' || t.kind == '\n'))
        throw string("Syntax error: end of line expected.");
    tp.first = tp.last = t;
}



vec statement(TokenPair& tp){
    Token t = ts.get();
    tp.first = t;
    vec value;
    switch(t.kind){
        case '$':{
            Token t2 = ts.get();
            tp.last = t2;
            if(t2.kind == '='){
                ts.putback(t2);
                ts.putback(t);
                value = assignment(tp);
                if(tp.last.kind == '\n')
                    return value;
                eol(tp);
                return value;
            }
            else ts.putback(t2);
        }
        default:
            ts.putback(t);
            value =  expression(tp);
            t = ts.get();
            tp.last = t;
            if(t.kind == '['){
                vec ix = expression(tp);
                t = ts.get();
                tp.last = t;
                assert(']', t.kind);
                eol(tp);
                return value[ix];
            }
            else{
                ts.putback(t);
                return value;
            }
            eol(tp);
            return value;
    }
}

void interpret(){

    while(cin)
    try{
        cout << "> ";
        Token t = ts.get();
        while (t.kind == ';' || t.kind == '\n')
            t = ts.get();
        if(t.text == "quit" || t.text == "exit")
            exit(0);
        if(t.text == "ls"){
            cout << sym.var_list();
            interpret();
        }
        if(t.text == "del"){
            t = ts.get();
            assert('$', t.kind);
            sym.del(t.text);
            interpret();
        }

        ts.putback(t);
        auto result = statement(tpair);

        if(rule != "=")
            cout << result << endl;

     rule.clear();
    }
    catch(const string& s){
        cerr << s << endl;
        ts.cleanup();
    }

}


#endif // PARSER_H
