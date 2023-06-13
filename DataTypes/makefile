
RM = rm -rf
CC = cc
CPP = c++


CFLAGS = -g -Wall
INCLUDE_DIRS = -Iinclude
OBJDIR = obj
OBJ_SUBDIRS = $(addprefix ${OBJDIR}/,Value Type Algorithm MemoryManagement Utils )
LIBDIR = lib
LIBNAME = libDecl.a
LIBOBJS = \
	${OBJDIR}/Value/CompositeValue.o \
	${OBJDIR}/Value/IntegerValue.o \
	${OBJDIR}/Value/FloatingPointValue.o \
	${OBJDIR}/Value/PointerValue.o \
	${OBJDIR}/Value/StringValue.o \
	${OBJDIR}/Value/ArrayValue.o \
	${OBJDIR}/Type/StructMember.o \
	${OBJDIR}/Type/TypedStructMember.o \
	${OBJDIR}/Type/NormalStructMember.o \
	${OBJDIR}/Type/StaticStructMember.o \
	${OBJDIR}/Type/BitfieldStructMember.o \
	${OBJDIR}/Type/CompositeDataType.o \
	${OBJDIR}/Type/EnumDataType.o \
	${OBJDIR}/Type/ArrayDataType.o \
	${OBJDIR}/Type/PointerDataType.o \
	${OBJDIR}/Type/PrimitiveDataType.o \
	${OBJDIR}/Type/DataType.o \
	${OBJDIR}/MemoryManagement/AllocInfo.o \
	${OBJDIR}/MemoryManagement/MemMgr.o \
	${OBJDIR}/Utils/LexicalAnalyzer.o \
	${OBJDIR}/Utils/MutableDeclaration.o \
	${OBJDIR}/Algorithm/CheckpointVisitor.o \
	${OBJDIR}/Algorithm/LookupAddressVisitor.o \
	${OBJDIR}/Enumerator.o \
	${OBJDIR}/TypeDictionary.o \
	${OBJDIR}/EnumDictionary.o \
	${OBJDIR}/DataTypeInator.o \
	${OBJDIR}/ClassicChkPtAgent.o \






#############################################################################
##                            MODEL TARGETS                                ##
#############################################################################

all: ${LIBDIR}/${LIBNAME}

$(LIBOBJS): $(OBJDIR)/%.o : src/%.cpp | $(OBJDIR)
	$(CPP) $(CFLAGS) ${INCLUDE_DIRS} -c $< -o $@

${LIBDIR}/${LIBNAME}: ${LIBOBJS} | ${LIBDIR}
	ar crs $@ ${LIBOBJS}

${OBJDIR}:
	mkdir -p ${OBJDIR} ${OBJ_SUBDIRS}

${LIBDIR}:
	mkdir -p ${LIBDIR}

clean:
	${RM} *~
	${RM} ${OBJDIR} ${OBJ_SUBDIRS}

spotless: clean
	${RM} ${LIBDIR}/${LIBNAME}


