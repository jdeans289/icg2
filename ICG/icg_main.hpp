
#ifndef __CLANG_PARSE_HH__
#define __CLANG_PARSE_HH__


#include <string>
#include <vector>
#include <map>

#include "utils.hpp"
#include "icg_template_engine/icg_template_engine.hpp"


class FieldInfo final : public recursable {
    public:
    FieldInfo (std::string n, std::string t, AccessLevel a);

    std::string type;
    std::string name;
    AccessLevel access_info;
    StorageClass storage_class;
    
    std::map<std::string, std::string> toDictionary() const;
    std::vector<const recursable *> nextLevel() const;


    friend std::ostream& operator<< (std::ostream& stream, const FieldInfo& str);
};

class ClassInfo final : public recursable {
public:
    ClassInfo(std::string n);
    std::string name;
    std::vector<FieldInfo> fields;

    std::map<std::string, std::string> toDictionary() const;
    std::vector<const recursable *> nextLevel() const;


    friend std::ostream& operator<< (std::ostream& stream, const ClassInfo& str);
};

// class TemplatedClassInfo : public ClassInfo {
// public:
//     TemplatedClassInfo(std::string n) : ClassInfo(n) {}

//     std::vector<std::string> templateNames;
// };

#endif