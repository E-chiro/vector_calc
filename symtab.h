#ifndef SYMTAB_H
#define SYMTAB_H

#include <map>
#include <string>
using namespace std;


class SymbolTable{
    std::map<string, Vector> vectors;

public:
    SymbolTable(){
        vectors["pi"] = 3.141592653589793;
        vectors["e"] = 2.718281828459045;
    }

    void set(const string& id, const Vector& value){
        vectors[id] = value;
    }

    Vector get(const string& id){
        if(vectors.find(id) != vectors.end())
            return vectors[id];
        throw string("Variable '" + id + "' is not defined.");
    }

    void del(const string& id){
        auto it = vectors.find(id);
        if(it != vectors.end()){
            vectors.erase(it);
            return;
        }
        throw string("Variable '" + id + "' is not defined.");
    }

    string var_list(){
        stringstream ss;
        for(auto i: vectors)
            ss << '\t' << i.first << ": " << i.second << endl;
        return ss.str();
    }
};



#endif // SYMTAB_H
