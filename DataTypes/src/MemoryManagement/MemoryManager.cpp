#include <iostream>
#include <fstream>
#include <algorithm>

#include "MemoryManagement/MemoryManager.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "Utils/MutableDeclaration.hpp"

#include "CheckpointAgent/J_CheckpointAgent.hpp"
#include "DataTypeInator.hpp"



// Constructor
MemoryManager::MemoryManager() : MemoryManager(new DataTypeInator()) {}

MemoryManager::MemoryManager (DataTypeInator * dictionary) : dataTypeInator(dictionary) {
   debugLevel = 0;

   checkpointAgent = new J_CheckpointAgent(dataTypeInator);
}

void* MemoryManager::do_declare_var(const std::string& abstract_declarator, 
                             const std::string& variable_name,
                             void * supplied_allocation) {

    if ( var_exists( variable_name )) {
        std::cerr << "ERROR: Variable " << variable_name << " already declared." << std::endl;
        return ((void*)NULL);
    }

    AllocInfo* newAllocInfo;
    void* actualAllocation;
    try {
        // FIXME: MUTEX LOCK to protect AllocInfo static variables
        const DataType * type = dataTypeInator->resolve(abstract_declarator);
        if (type == NULL) {
            std::cerr << "ERROR: Unable to resolve type string \"" << abstract_declarator << "\" for variable \"" << variable_name << "\", cannot declare." << std::endl;
            return ((void*)NULL);
        }

        newAllocInfo = new AllocInfo(variable_name, type, supplied_allocation);

        // Set the address and insert in the maps
        actualAllocation = newAllocInfo->getStart();

        allocInfoByAddressMap[actualAllocation] = newAllocInfo;
        allocInfoByNameMap[variable_name] = newAllocInfo;

    } catch ( std::logic_error& e ) {
        // FIXME: MUTEX UNLOCK
        std::cerr << e.what();
        newAllocInfo = (AllocInfo*)NULL;
        actualAllocation = (void*)NULL;
    }
    if ( debugLevel ) {
        if ( newAllocInfo == (AllocInfo*)NULL ) {
            std::cout << "Allocation Failed." << std::endl;
        } else {
            std::cout << newAllocInfo->toString() << std::endl;
        }
    }
    return( actualAllocation );
}

// MEMBER FUNCTION
void* MemoryManager::declare_var( const std::string& typeSpecifier,
                           const std::string& variableName,
                           unsigned int       dimensionsCount,
                           int*               dimensions,
                           void*              suppliedAllocation ) {

    std::vector<int> dims(dimensions, dimensions + dimensionsCount);
    MutableDeclaration decl (typeSpecifier, dims);
    return do_declare_var(decl.getAbstractDeclarator(), variableName, suppliedAllocation);
}

// MEMBER FUNCTION
void* MemoryManager::declare_var( const std::string& declaration,
                           void*              suppliedAllocation ) {

    MutableDeclaration typeName(declaration);
    std::string abstractDeclarator = typeName.getAbstractDeclarator();
    std::string variableName = typeName.getVariableName();

    return do_declare_var( abstractDeclarator, variableName, suppliedAllocation );
}

// MEMBER FUNCTION
void* MemoryManager::declare_var( const std::string& typeSpecName,
                           const std::string& variableName,
                           int                n,
                           void*              suppliedAllocation) {

    MutableDeclaration decl =  MutableDeclaration( typeSpecName);
    decl.pushDimension(n);

    std::string abstractDeclarator = decl.getAbstractDeclarator();

    return do_declare_var( abstractDeclarator, variableName, suppliedAllocation );   
}

// MEMBER FUNCTION
void* MemoryManager::declare_var( const std::string& typeSpecName,
                           int                n,
                           void*              suppliedAllocation ) {

    MutableDeclaration decl =  MutableDeclaration( typeSpecName);
    decl.pushDimension(n);

    std::string abstractDeclarator = decl.getAbstractDeclarator();
    std::string variableName = decl.getVariableName();

    return do_declare_var( abstractDeclarator, variableName, suppliedAllocation );   
}

// // MEMBER FUNCTION
// void* MemoryManager::resize_var( void* address, size_t newElementCount) {
//     void* newAddress;
//     AllocInfo* allocInfo = getAllocInfoOf(address);
//     if (allocInfo != NULL) {
//         newAddress = allocInfo->resize( newElementCount );
//     } else {
//         std::cerr << __FUNCTION__ << " failed. Address (" << address << ") not in Trick managed memory." << std::endl;
//         newAddress = NULL;
//     }
//     return newAddress;
// }

// // MEMBER FUNCTION
// void* MemoryManager::resize_var( const std::string& name, size_t newElementCount) {
//    void* newAddress;
//    AllocInfo* allocInfo = getAllocInfoNamed(name);
//     if (allocInfo != NULL) {
//         newAddress = allocInfo->resize( newElementCount );
//     } else {
//         std::cerr << __FUNCTION__ << " failed. Name \"" << name << "\" not in Trick managed memory." << std::endl;
//         newAddress = NULL;
//     }
//     return newAddress;
// }

// MEMBER FUNCTION
//    void* resize_var( const char* name, int num);


// MEMBER FUNCTION
bool MemoryManager::var_exists( const std::string& variableName) {

    if ( !variableName.empty()) {
        AllocInfo* allocInfo = getAllocInfoNamed( variableName );
        if ( allocInfo != (AllocInfo*)NULL ) {
            return true;
        }
    }
    return false;
}

// MEMBER FUNCTION
void MemoryManager::clear_var( void* address) {

    AllocInfo* allocInfo;
    if ((allocInfo = getAllocInfoOf( address)) != NULL) {
        allocInfo->clear();
    } else {
        std::stringstream ss;
        ss << "Cannot clear the variable at address ";
        ss << address;
        ss << " because memory manager knows nothing about it.";
        ss << std::endl;
        std::cerr << ss.str();
    }
}

// MEMBER FUNCTION
void MemoryManager::clear_var( const std::string& variableName) {

    AllocInfo* allocInfo;
    if ((allocInfo = getAllocInfoNamed( variableName )) != NULL) {
        allocInfo->clear();
    } else {
        std::stringstream ss;
        ss << "Cannot clear the variable named ";
        ss << variableName;
        ss << " because memory manager knows nothing about it.";
        ss << std::endl;
        std::cerr << ss.str();
    }
}

// MEMBER FUNCTION
void MemoryManager::clear_all_vars() {
}

// MEMBER FUNCTION
void MemoryManager::write_checkpoint( std::ostream& out_s) {

    AllocInfo* allocInfo;
    std::vector<AllocInfo*> allocInfoList;
    std::map<void*, AllocInfo*>::iterator pos;

    pthread_mutex_lock(&allocInfoMapMutex);

    for ( pos = allocInfoByAddressMap.begin() ; pos != allocInfoByAddressMap.end() ; pos++ ) {
        allocInfo = pos->second;
        allocInfoList.push_back(allocInfo);
    }
    write_checkpoint( out_s, allocInfoList);

    pthread_mutex_unlock(&allocInfoMapMutex);
}

// MEMBER FUNCTION
void MemoryManager::write_checkpoint( const std::string& filename) {

    std::ofstream out_s( filename.c_str(), std::ios::out);
    if (out_s.is_open()) {
        write_checkpoint( out_s );
    } else {
        std::cerr << "ERROR: Couldn't open \""<< filename <<"\"." << std::endl;
        std::cerr.flush();
    }
}

// MEMBER FUNCTION
void MemoryManager::write_checkpoint( std::ostream& out_s, const std::string& variableName) {

    std::vector<AllocInfo*> dependencies;
    pthread_mutex_lock(&allocInfoMapMutex);
    AllocInfo * allocInfo = getAllocInfoNamed( variableName );
    // if ( allocInfo != NULL ) {
    //     allocInfo->appendDependencies( this, dependencies );
    // }
    pthread_mutex_unlock(&allocInfoMapMutex);
    write_checkpoint( out_s, dependencies);
}

// MEMBER FUNCTION
void MemoryManager::write_checkpoint(const std::string& filename, const std::string& variableName) {

    std::ofstream out_s( filename.c_str(), std::ios::out);
    if (out_s.is_open()) {
        write_checkpoint( out_s, variableName);
    } else {
        std::cerr << "ERROR: Couldn't open \""<< filename <<"\"." << std::endl;
        std::cerr.flush();
    }
}

// MEMBER FUNCTION
void MemoryManager::write_checkpoint( std::ostream& out_s, std::vector<const char*>& variableNameList) {

    std::vector<AllocInfo*> dependencies;
    const char* variableName;
    int nameCount;
    nameCount = variableNameList.size();
    for (int ii=0; ii< nameCount; ii++) {
        variableName = variableNameList[ii];
        pthread_mutex_lock(&allocInfoMapMutex);
        // AllocInfo* allocInfo = getAllocInfoNamed( variableName );
        // if ( allocInfo != NULL ) {
        //     allocInfo->appendDependencies( this, dependencies );
        // }
        pthread_mutex_unlock(&allocInfoMapMutex);
    }
    write_checkpoint( out_s, dependencies);
}

// MEMBER FUNCTION
void MemoryManager::write_checkpoint(const std::string& filename, std::vector<const char*>& variableNameList) {

    std::ofstream out_s( filename.c_str(), std::ios::out);
    if (out_s.is_open()) {
        write_checkpoint( out_s, variableNameList );
    } else {
        std::cerr << "ERROR: Couldn't open \""<< filename <<"\"." << std::endl;
        std::cerr.flush();
    }
}

// STATIC FUNCTION
static bool allocInfoCompare( AllocInfo* lhs, AllocInfo* rhs ) {
    bool result;
    StorageClass::e lhsStorageClass = lhs->getStorageClass();
    StorageClass::e rhsStorageClass = rhs->getStorageClass();
    unsigned int lhsSerialNumber = lhs->getSerialNumber();
    unsigned int rhsSerialNumber = rhs->getSerialNumber();

    if ( lhsStorageClass == rhsStorageClass ) {
        result = ( lhsSerialNumber < rhsSerialNumber );
    } else if ( lhsStorageClass == StorageClass:: EXTERN ) {
        result = true;
    } else {
        result = false;
    }
    return result;
}

// PRIVATE MEMBER FUNCTION
void MemoryManager::write_checkpoint(std::ostream& outStream, std::vector<AllocInfo*>& allocInfoList) {
    checkpointAgent->dump(outStream, allocInfoList);
}

void MemoryManager::restore_checkpoint(const std::string& filename) {
    std::ifstream in_s( filename.c_str(), std::ios::in);
    if (in_s.is_open()) {
        restore_checkpoint( in_s) ;
    } else {
        std::cerr << "ERROR: Couldn't open \""<< filename <<"\"." << std::endl;
        std::cerr.flush();
    }
}

void MemoryManager::restore_checkpoint( std::istream& in_s) {
    // TODO: We should be holding a lock here
    
    // Only pass external allocations into the agent
    std::vector<AllocInfo *> extern_alloc_list;
    for ( auto pos = allocInfoByAddressMap.begin() ; pos != allocInfoByAddressMap.end() ; pos++ ) {
        AllocInfo * allocInfo = pos->second;
        if (allocInfo->getStorageClass() == StorageClass::EXTERN) {
            extern_alloc_list.push_back(allocInfo);
        }
    }

// TODO *****IMPORTANT JACKIE YOU HAVE TO THINK ABOUT THIS AT SOME POINT
// WHAT HAPPENS TO THE OLD LOCAL ALLOCS?????


    // Load the checkpoint into AllocInfos
    auto restored_alloc_list = checkpointAgent->restore(in_s, extern_alloc_list);

    // Rebuild the maps
    allocInfoByAddressMap.clear();
    allocInfoByNameMap.clear();

    for (AllocInfo * alloc : restored_alloc_list) {
        void * addr = alloc->getStart();
        std::string varname = alloc->getName();

        allocInfoByAddressMap[addr] = alloc;
        allocInfoByNameMap[varname] = alloc;
    }

    // We did it!
}

CheckpointAgentBase * MemoryManager::get_CheckPointAgent() {
    return checkpointAgent;
}

void MemoryManager::set_CheckPointAgent( CheckpointAgentBase* agent) {
    delete checkpointAgent;
    checkpointAgent = agent;
}

// MEMBER FUNCTION
AllocInfo* MemoryManager::getAllocInfoOf( void* address ) {
    AllocInfo* allocInfo;
    std::map<void*, AllocInfo*>::iterator pos;
    for ( pos = allocInfoByAddressMap.begin() ; pos != allocInfoByAddressMap.end() ; pos++ ) {
        allocInfo = pos->second;
        if ( allocInfo->contains( address )) {
            return allocInfo;
        }
    }
    return NULL;
}

// MEMBER FUNCTION
AllocInfo* MemoryManager::getAllocInfoAt( void* address ) {
    std::map<void*, AllocInfo*>::iterator pos;
    pos = allocInfoByAddressMap.find( address );
    if ( pos == allocInfoByAddressMap.end()) {
        return NULL;
    } else {
        return pos->second;
    }
}

// MEMBER FUNCTION
AllocInfo* MemoryManager::getAllocInfoNamed( const std::string& name ) {
    std::map<std::string, AllocInfo*>::iterator pos;
    pos = allocInfoByNameMap.find(name);
    if ( pos == allocInfoByNameMap.end()) {
        return NULL;
    } else {
        return pos->second;
    }
}


// MEMBER FUNCTION
const DataType* MemoryManager::getDataType( std::string typeName ) {
    return dataTypeInator->resolve(typeName);
}

#if NEWSTUFF
void * MemoryManager::getVarAccessInfo( const std::string& variableName, VarAccessInfo& varAccessInfo ) {

    std::string varName = variableName;
    // Get identifier at beginnning of the string and the remiander.
    // Find the AllocInfoByName ( identifier );

    AllocInfo* allocInfo = getAllocInfoNamed (variableName);
    if (allocInfo != NULL) {
    }
    returnAddress = alloc->getVarAccessInfo( varName, varAccessInfo );

    return returnAddress;
}
#endif
