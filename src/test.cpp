#include <iostream> 
#include <string>
#include <string.h>

using namespace std;

int main(){
    const char* name = "Mary";
    string blank;
    cout << name << endl;
    blank = name;
    cout << blank << endl;
    blank = blank + '/';
    cout << blank << endl;
    const char* last = "Bethany";
    blank = blank + last;
    cout << blank << endl;
    return 0;
}

