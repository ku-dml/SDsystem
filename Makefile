# compiler
ifeq ($(shell uname),Linux)
	CXX = g++-10
else
	CXX = g++
endif
# CXXFLAGS = -O2 -Wall -std=gnu++20
CXXFLAGS = -O2 -Wall -std=gnu++20 -g -IOGDF/include

# src directories
DIR_BIN = ./src/bin
DIR_LIB = ./src/lib
DIR_TEST = ./test

# target directories
TARGET = target
TARGET_DEPS = ${TARGET}/deps

# src file
ifeq ($(shell uname),Linux)
	SRC_BIN := ${shell find ${DIR_BIN} -mindepth 1 -maxdepth 1 -name "*.cpp"}
	SRC_TEST := ${shell find ${DIR_TEST} -mindepth 1 -maxdepth 1 -name "*.cpp"}
	SRC_LIB := ${shell find ${DIR_LIB} -mindepth 1 -maxdepth 1 -name "*.cpp"}
else
	SRC_BIN := ${shell find ${DIR_BIN} -depth 1 -name "*.cpp"}
	SRC_TEST := ${shell find ${DIR_TEST} -depth 1 -name "*.cpp"}
	SRC_LIB := ${shell find ${DIR_LIB} -depth 1 -name "*.cpp"}
endif

SRCS = ${SRC_BIN} ${SRC_TEST} ${SRC_LIB}
# SRCS_LIB =

# objects files
OBJS = ${foreach src,${SRCS},${patsubst %.cpp,${TARGET_DEPS}/%.o,${notdir ${src}}}}
OBJS_LIB = ${foreach src,${SRC_LIB},${patsubst %.cpp,${TARGET_DEPS}/%.o,${notdir ${src}}}}
OBJS_TEST = ${foreach src,${SRC_TEST},${patsubst %.cpp,${TARGET_DEPS}/%.o,${notdir ${src}}}}
OBJS_BIN = ${foreach src,${SRC_BIN},${patsubst %.cpp,${TARGET_DEPS}/%.o,${notdir ${src}}}}

# OGDF_LIB

.PHONY: clean test run

run: ${TARGET}/enumSDSystem
	./$<

build: ${TARGET}/enumSDSystem

# run test
test: ${TARGET}/test
	./$<

${TARGET}/enumSDSystem: ${OBJS_BIN} ${OBJS_LIB} ./OGDF/libOGDF.a
	${CXX} ${CXXFLAGS} -o $@ $^

# make ELF for test
${TARGET}/test: ${OBJS_LIB} ${OBJS_TEST}
	${CXX} ${CXXFLAGS} -o $@ $^

# compile each src file to object file when file updated
define template
${TARGET_DEPS}/${patsubst %.cpp,%.o,${notdir $1}}: $1 ${TARGET_DEPS}
	if [ ! -e ${TARGET_DEPS}/${patsubst %.cpp,%.o,${notdir $1}} ] \
	|| [ ${TARGET_DEPS}/${patsubst %.cpp,%.o,${notdir $1}} -ot ${1} ]; then \
		${CXX} -c ${CXXFLAGS} -o ${TARGET_DEPS}/${patsubst %.cpp,%.o,${notdir $1}} ${1}; \
	fi
endef

${foreach src,${SRCS},${eval ${call template,${src}}}}

# remove binaries
clean:
	rm -rf ${TARGET}

# make target directory
${TARGET_DEPS}:
	mkdir -p $@

./OGDF/libOGDF.a:
	echo "\n1. Download OGDF (https://ogdf.uos.de/2023/09/14/elderberry/)\n2. and build it (https://github.com/ogdf/ogdf/blob/master/doc/build.md)\n"

${TARGET}/generateGraph: ./analyze/main.cpp ${TARGET_DEPS}
	${CXX} -O2 -Wall -std=gnu++20 -o target/generateGraph ./analyze/main.cpp