#include <iostream>

int main()
{
#if defined( __clang__ )
    std::cout << "Hello Clang" << std::endl;
#elif defined( __GNUC__ )
    std::cout << "Hello GCC" << std::endl;
#elif defined( _MSC_VER )
    std::cout << "Hello MSVC" << std::endl;
#endif

    return EXIT_SUCCESS;
}
