#ifndef ERROR_H
#define ERROR_H

/**
 * Exits with fatal error.
 * You should call the macro fatalError for easier usage of this function.
 * @param file The file of the error.
 * @param line The line of the error.
 * @param format The error message. A \n will already appear at the end and
 * the line and file of the error also. You can use the printf() syntax
 * with this message.
 * @param ... printf() parameters.
 **/
void fatalErrorFunc(const char *file, int line, const char *format, ...);

/**
 * This function serves as a wrapper for fatalErrorFunc to allow easy insertion
 * of __FILE__ and __LINE__ macros to ease the debugging process.
 * @param format The error message. A \n will already be appended at the end
 * and the line and file of the error also. You can use the printf() syntax
 * with this message.
 * @param .. printf() parameters.
 **/
#define fatalError(...) fatalErrorFunc(__FILE__, __LINE__, __VA_ARGS__)

#endif // !ERROR_H
