
#ifndef __CLANG_PARSE_HH__
#define __CLANG_PARSE_HH__


#include <string>
#include <vector>
#include <map>

#include "utils.hpp"
#include "icg_template_engine/icg_template_engine.hpp"

/**
 * @brief Hold information about a field declaration
 * 
 */
class FieldInfo final : public recursable {
    public:
    FieldInfo ();
    FieldInfo (std::string n, std::string t, AccessLevel a);

    // Rule of 5 is default

    std::string type;
    std::string name;
    AccessLevel access_info;
    StorageClass storage_class;
    
    std::map<std::string, std::string> toDictionary() const;
    std::vector<const recursable *> nextLevel() const;

    friend std::ostream& operator<< (std::ostream& stream, const FieldInfo& str);
};

/**
 * @brief Hold information about a class declaration
 * 
 */
class ClassInfo final : public recursable {
public:
    ClassInfo();
    ClassInfo(std::string n);

    // Rule of 5 is default

    std::string name;
    std::vector<FieldInfo> fields;

    std::map<std::string, std::string> toDictionary() const;
    std::vector<const recursable *> nextLevel() const;


    friend std::ostream& operator<< (std::ostream& stream, const ClassInfo& str);
};

// class TemplatedClassInfo : public recursable {
// public:
//     TemplatedClassInfo(std::string n) {}

//     std::vector<std::string> templateNames;
// };

#endif