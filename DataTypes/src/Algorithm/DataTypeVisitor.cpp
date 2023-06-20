#include "Algorithm/DataTypeVisitor.hh"
#include "Type/DataType.hh"

bool DataTypeVisitor::go(const DataType * node) {
    return node->accept(this);
}
