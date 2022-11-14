#include "printable.hpp"

std::ostream & operator<< (
    std::ostream & stream, Printable const & printableObject )
{
    printableObject.print_to_stream( stream );
    return stream;
}