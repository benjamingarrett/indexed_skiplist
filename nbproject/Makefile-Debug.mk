#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/indexed_skiplist.o \
	${OBJECTDIR}/indexed_skiplist_long_int.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f3

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/skiplist_test_index_of.o \
	${TESTDIR}/tests/skiplist_test_key_comparisons.o \
	${TESTDIR}/tests/skiplist_test_rand_level.o \
	${TESTDIR}/tests/skiplist_test_read_write_delete.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${TESTDIR}/TestFiles/f5

${TESTDIR}/TestFiles/f5: ${OBJECTFILES}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f5 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/indexed_skiplist.o: indexed_skiplist.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/indexed_skiplist.o indexed_skiplist.c

${OBJECTDIR}/indexed_skiplist_long_int.o: indexed_skiplist_long_int.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/indexed_skiplist_long_int.o indexed_skiplist_long_int.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/skiplist_test_index_of.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/skiplist_test_key_comparisons.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/skiplist_test_rand_level.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/skiplist_test_read_write_delete.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/skiplist_test_index_of.o: tests/skiplist_test_index_of.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/skiplist_test_index_of.o tests/skiplist_test_index_of.c


${TESTDIR}/tests/skiplist_test_key_comparisons.o: tests/skiplist_test_key_comparisons.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -I. -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/skiplist_test_key_comparisons.o tests/skiplist_test_key_comparisons.c


${TESTDIR}/tests/skiplist_test_rand_level.o: tests/skiplist_test_rand_level.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/skiplist_test_rand_level.o tests/skiplist_test_rand_level.c


${TESTDIR}/tests/skiplist_test_read_write_delete.o: tests/skiplist_test_read_write_delete.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/skiplist_test_read_write_delete.o tests/skiplist_test_read_write_delete.c


${OBJECTDIR}/indexed_skiplist_nomain.o: ${OBJECTDIR}/indexed_skiplist.o indexed_skiplist.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/indexed_skiplist.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/indexed_skiplist_nomain.o indexed_skiplist.c;\
	else  \
	    ${CP} ${OBJECTDIR}/indexed_skiplist.o ${OBJECTDIR}/indexed_skiplist_nomain.o;\
	fi

${OBJECTDIR}/indexed_skiplist_long_int_nomain.o: ${OBJECTDIR}/indexed_skiplist_long_int.o indexed_skiplist_long_int.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/indexed_skiplist_long_int.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/indexed_skiplist_long_int_nomain.o indexed_skiplist_long_int.c;\
	else  \
	    ${CP} ${OBJECTDIR}/indexed_skiplist_long_int.o ${OBJECTDIR}/indexed_skiplist_long_int_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${TESTDIR}/TestFiles/f5

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
