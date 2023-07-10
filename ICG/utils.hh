#include <clang-c/Index.h>  // This is libclang.
#include <iostream>
#include <string>


enum StorageClass {LOCAL, STATIC};
enum AccessLevel  {INVALID, PUBLIC, PROTECTED, PRIVATE, NONE};


std::string toStdString(const CXString& str) {
    const char * c_str =  clang_getCString(str);
    if (c_str != NULL) {
        std::string my_std_string = clang_getCString(str);
        clang_disposeString(str);
        return my_std_string;
    }

    return std::string("");
}


std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

std::string getCursorSpelling (CXCursor c) {
    return toStdString(clang_getCursorSpelling(c));
}

std::string getTypeSpelling (CXCursor c) {
    return toStdString(clang_getTypeSpelling(clang_getCursorType(c)));
}

std::string getKindSpelling (CXCursor c) {
    return toStdString(clang_getCursorKindSpelling(clang_getCursorKind(c)));
}

enum AccessLevel getAccessLevel (CXCursor c) {
    auto clang_access = clang_getCXXAccessSpecifier(c);

    enum AccessLevel level;
    switch (clang_access) {
        case CX_CXXPublic:
            level = AccessLevel::PUBLIC;
            break;
        case CX_CXXProtected:
            level = AccessLevel::PROTECTED;
            break;
        case CX_CXXPrivate:
            level = AccessLevel::PRIVATE;
            break;
        case CX_CXXInvalidAccessSpecifier:
            level = AccessLevel::INVALID;
            break;
        default:
            level = AccessLevel::NONE;
            break;
    }

    return level;
}

std::string makeIOHeaderName (std::string header_name) {
    return "io_" + header_name;
}