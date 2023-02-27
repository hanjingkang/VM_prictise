#include <string>
using namespace std;

string outside_value="i am a string from outside";
string outside_value2="test";

class class_school
{
public:
    static string lib;
    static void add(string book){
        lib+=book;
    }
};

