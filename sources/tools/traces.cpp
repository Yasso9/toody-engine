#include "traces.hpp"

namespace Trace
{
    void GenerateTest ()
    {
        Trace::Error( "Traces - {}", "This is an Error Trace" );
        Trace::Warning( "Traces - {}", "This is a Warning Trace" );
        Trace::Success( "Traces - {}", "This is a Success Trace" );
        Trace::Info( "Traces - {}", "This is an Info Trace" );
        Trace::Debug( "Traces - {}", "This is a Debug Trace" );
        Trace::FileIssue( "path/to/file", "Traces - {}",
                          "This is a File Issue Trace" );
        Trace::FileNotFound( "path/to/file", "Traces - {}",
                             "This is a File Not Found Trace" );
    }

}  // namespace Trace
