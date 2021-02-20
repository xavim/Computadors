#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>


class person {
     std::string name, surname1, surname2;
     std::string passport_no;
     std::string national_id;
     unsigned short age;

	public:

     person() : name(""),surname1(""),surname2(""),passport_no(""),national_id(""),age(0) 
	  { /*std::cout << "constructor" << std::endl; */ }
};


void show_maps(const char * header)
{
   int res;
   std::string str = "cat /proc/";
   std::stringstream command;
   command << str << getpid() << "/maps";
   std::cout << header << std::endl;
   res = system(command.str().c_str());
   std::cout << "end of " << header << std::endl;
}


int main(int argc, char * argv [])
{

    show_maps("initial mappings");

    person * p = nullptr;

    try {

        p = new person[50000];
	 
    } catch(std::exception & e) {
	    std::cerr << "Exception catched: " << e.what() << std::endl;
	    perror("new");
            std::cout << "pointer " << static_cast<void*>(p) << std::endl;
	    std::exit(2);
    }

    std::cout << "pointer " << static_cast<void*>(p) << std::endl;

    show_maps("final mappings");

    return 0;
}
