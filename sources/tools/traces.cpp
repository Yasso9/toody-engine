#include "traces.hpp"

namespace Trace
{
    void GenerateTest ()
    {
        Trace::Error( "{}", "This is an Error Trace" );
        Trace::Warning( "{}", "This is a Warning Trace" );
        Trace::Success( "{}", "This is a Success Trace" );
        Trace::Info( "{}", "This is an Info Trace" );
        Trace::Debug( "{}", "This is a Debug Trace" );
        Trace::FileIssue( "path/to/file", "{}", "This is a File Issue Trace" );
        Trace::FileNotFound( "path/to/file", "{}",
                             "This is a File Not Found Trace" );
    }

}  // namespace Trace
