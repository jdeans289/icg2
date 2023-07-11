#include "Algorithm/DataTypeVisitor.hpp"
#include "Type/DataType.hpp"

bool DataTypeVisitor::go(const DataType * node) {
    return node->accept(this);
}
