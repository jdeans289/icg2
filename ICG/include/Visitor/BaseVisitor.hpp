#pragma once

#include <clang-c/Index.h>  // libclang


/**
 * @brief A base class for the visitors to traverse the Clang AST
 * @note A correct design will usually go something like:
 *      - go is the entry point of a visitor
 *      - go should pull any useful information out of the current pointer and then call some version of clang_visitChildren
 *      - clang_visitChildren will call traverse with all of the children it finds
 *      - traverse should have a switch statement or something similar to handle any relevant types of nodes found
 *      - Within this switch statement, a Visitor for the associated type should be created with relevant context, and then call visitor.go, and then pull out the result
 *    Calls to clang_visitChildren use the method forwarding_traverse to pass to the correct visitor. Calls should look something like this:
 *          clang_visitChildren(c, forwarding_traverse, &classVisitor);
 */
class BaseVisitor {
    public:
 
        /**
         * @brief This is the hook that will be called from clang_visitChildren via forwarding_traverse
         * @param cursor Current cursor to investigate
         * @param parent Parent cursor
         * @return CXChildVisitResult
         */
        virtual CXChildVisitResult traverse(CXCursor cursor, CXCursor parent) = 0;

        /**
         * @brief The entry point for Clang AST visitors
         * 
         * @param cursor Clang cursor to start visiting from
         */
        virtual void go (CXCursor cursor) = 0;
};


/**
 * @brief Decide if we should recurse into this branch of the tree or exclude it.
 * 
 * @param c Cursor to investigate
 * @return true if cursor is in a system library, false otherwise
 * @todo Implement other excludes here
 */
bool should_exclude (CXCursor c);

/**
 * @brief A free function that should be passed into LibClang's clang_visitChildren method. 
 * 
 * @param c Cursor to visit
 * @param parent parent of c
 * @param client_data A pointer to an instance of the correct visitor class. This is casted to a BaseVisitor and used to call traverse in the correct context.
 * @return CXChildVisitResult 
 */
CXChildVisitResult forwarding_traverse(CXCursor c, CXCursor parent, CXClientData client_data);