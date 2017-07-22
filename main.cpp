#include <iostream>
#include "parser.h"
using namespace std;

int main()
{
    try{



        interpret();

    }catch(...){
        cerr << "Unknown error...\n";
    }

    return 0;
}
