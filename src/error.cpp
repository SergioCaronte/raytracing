#include "error.h"
#include <cstdarg>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstdlib>

void fatalErrorFunc(const char *file, int line, const char *format, ...) 
{
    va_list args;

    // Concat the format string.
    std::stringstream ss;
    ss << file << ":" << line << ": " << format << "\n";

    va_start(args, format);
    vfprintf(stderr, ss.str().c_str(), args);
    va_end(args);

    exit(1);
}
