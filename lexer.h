#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <iomanip>
using namespace std;

class TokenStream{
    istream& in;
    vector<Token> buffer;

    public:
        TokenStream(istream& src): in(src) {}

        void putback(const Token& token){
            buffer.push_back(token);
        }

        Token get(bool sign_sens=false){
            if(buffer.size() > 0){
                Token t = buffer.back();
                buffer.pop_back();
                return t;
            }
            char ch;
            while(in.get(ch) && isspace(ch) && ch!='\n') {
                //
            }
            if(     (isdigit(ch) || ch=='.')
                ||  (sign_sens && (ch == '+' || ch == '-')) ){
                double d;
                if(ch == '0'){
                    char ch2;
                    in.get(ch2);
                    if(ch2 == 'x'){
                        in.putback(ch2);
                        in.putback(ch);
                        in >> hex >> d;
                        return Token(Vector(d));
                    }else if(isdigit(ch2)){
                        in.putback(ch2);
                        in.putback(ch);
                        in >> oct >> d;
                        return Token(Vector(d));
                    }
                    else in.putback(ch2);
                }

                in.putback(ch);
                in >> d;
                return Token(Vector(d));
            }
            else if(isalpha(ch) || ch=='_'){
                string s;
                s += ch;
                while(in.get(ch) && (isalnum(ch) || ch == '_'))
                    s += ch;
                in.putback(ch);
                return Token(s);
            }
            else switch(ch){
                case '+': case '-':
                case '*': case '/': case '%':
                case '(': case ')':
                case '[': case ']':
                case ',':
                case ';': case '\n':
                case ':': case '=':
                case '~':
                    return Token(ch);
                default:
                    throw string("Unknown lexeme.");
            }
        }

        void cleanup(){
            if(buffer.size() > 0){
                while(!(buffer.back().kind == ';' || buffer.back().kind == '\n'))
                    buffer.pop_back();
            }
            if(buffer.empty()){
                char ch;
                while(in.get(ch) && !(ch == '\n' || ch == ';')) ;
            }
            else return;
        }
};

#endif // LEXER_H
