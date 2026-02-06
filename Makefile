AR=ar
CC=gcc
CXX=g++

INC_DIR = ./include
SRC_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = ./obj
LIB_DIR = ./lib
TESTSRC_DIR = ./testsrc
TESTOBJ_DIR = ./testobj
TESTBIN_DIR = ./testbin
TESTCOVER_DIR = ./htmlconv

PKGS = expat
DEFINES =
INCLUDES += -I $(INC_DIR)
CFLAGS += `pkg_config -cflags $(PKGS)`
CPPFLAGS += -std=c++20
LDFLAGS = `pkg_config -libs $(PKGS)`

TEST_CFLAGS = $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS = $(CPPFLAGS) -fno-inline
TEST_LDFLAGS = $(LDFLAGS) -lgtest_main -lgtest  -lpthread -lexpat


# test object files
TEST_OBJ_FILES_STRUTILS = $(TESTOBJ_DIR)/StringUtilsTest.o $(TESTOBJ_DIR)/StringUtils.o
TEST_OBJ_FILES_STRDTASRC = $(TESTOBJ_DIR)/StringDataSourceTest.o $(TESTOBJ_DIR)/StringDataSource.o
TEST_OBJ_FILES_STRDTASINK = $(TESTOBJ_DIR)/StringDataSinkTest.o $(TESTOBJ_DIR)/StringDataSink.o
TEST_OBJ_FILES_DSV = $(TESTOBJ_DIR)/DSVTest.o $(TESTOBJ_DIR)/DSVReader.o $(TESTOBJ_DIR)/DSVWriter.o $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/StringDataSink.o
TEST_OBJ_FILES_XML = $(TESTOBJ_DIR)/XMLTest.o $(TESTOBJ_DIR)/XMLReader.o $(TESTOBJ_DIR)/XMLWriter.o $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/StringDataSink.o

# test executables
TEST_TARGET_STRUTILS = $(TESTBIN_DIR)/teststrutils
TEST_TARGET_STRDTASRC = $(TESTBIN_DIR)/teststrdatasource
TEST_TARGET_STRDTASINK = $(TESTBIN_DIR)/teststrdatasink
TEST_TARGET_DSV = $(TESTBIN_DIR)/testdsv
TEST_TARGET_XML = $(TESTBIN_DIR)/testxml

TEST_TARGETS = $(TEST_TARGET_STRUTILS) $(TEST_TARGET_STRDTASRC) $(TEST_TARGET_STRDTASINK) $(TEST_TARGET_DSV) $(TEST_TARGET_XML)

all: directories runtests

runtests: $(TEST_TARGETS)
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test..."; \
		$$test; \
	done

	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR)

# link
$(TEST_TARGET_STRUTILS): $(TEST_OBJ_FILES_STRUTILS)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES_STRUTILS) $(TEST_LDFLAGS) -o $(TEST_TARGET_STRUTILS)

$(TEST_TARGET_STRDTASRC): $(TEST_OBJ_FILES_STRDTASRC)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES_STRDTASRC) $(TEST_LDFLAGS) -o $(TEST_TARGET_STRDTASRC)

$(TEST_TARGET_STRDTASINK): $(TEST_OBJ_FILES_STRDTASINK)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES_STRDTASINK) $(TEST_LDFLAGS) -o $(TEST_TARGET_STRDTASINK)

$(TEST_TARGET_DSV): $(TEST_OBJ_FILES_DSV)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES_DSV) $(TEST_LDFLAGS) -o $(TEST_TARGET_DSV)

$(TEST_TARGET_XML): $(TEST_OBJ_FILES_XML)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES_XML) $(TEST_LDFLAGS) -o $(TEST_TARGET_XML)


# compile
#
# stringutils
$(TESTOBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(SRC_DIR)/StringUtils.cpp -o $(TESTOBJ_DIR)/StringUtils.o

$(TESTOBJ_DIR)/StringUtilsTest.o: $(TESTSRC_DIR)/StringUtilsTest.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(TESTSRC_DIR)/StringUtilsTest.cpp -o $(TESTOBJ_DIR)/StringUtilsTest.o


# stringdatasource
$(TESTOBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(SRC_DIR)/StringDataSource.cpp -o $(TESTOBJ_DIR)/StringDataSource.o

$(TESTOBJ_DIR)/StringDataSourceTest.o: $(TESTSRC_DIR)/StringDataSourceTest.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(TESTSRC_DIR)/StringDataSourceTest.cpp -o $(TESTOBJ_DIR)/StringDataSourceTest.o


# stringdatasink
$(TESTOBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(SRC_DIR)/StringDataSink.cpp -o $(TESTOBJ_DIR)/StringDataSink.o

$(TESTOBJ_DIR)/StringDataSinkTest.o: $(TESTSRC_DIR)/StringDataSinkTest.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(TESTSRC_DIR)/StringDataSinkTest.cpp -o $(TESTOBJ_DIR)/StringDataSinkTest.o


# DSV
$(TESTOBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(SRC_DIR)/DSVReader.cpp -o $(TESTOBJ_DIR)/DSVReader.o

$(TESTOBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(SRC_DIR)/DSVWriter.cpp -o $(TESTOBJ_DIR)/DSVWriter.o

$(TESTOBJ_DIR)/DSVTest.o: $(TESTSRC_DIR)/DSVTest.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(TESTSRC_DIR)/DSVTest.cpp -o $(TESTOBJ_DIR)/DSVTest.o


# XML
$(TESTOBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(SRC_DIR)/XMLReader.cpp -o $(TESTOBJ_DIR)/XMLReader.o

$(TESTOBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(SRC_DIR)/XMLWriter.cpp -o $(TESTOBJ_DIR)/XMLWriter.o

$(TESTOBJ_DIR)/XMLTest.o: $(TESTSRC_DIR)/XMLTest.cpp | directories
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDES) -c $(TESTSRC_DIR)/XMLTest.cpp -o $(TESTOBJ_DIR)/XMLTest.o


.PHONY: directories
directories: 
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(TESTBIN_DIR)
	mkdir -p $(TESTOBJ_DIR)
	mkdir -p $(TESTCOVER_DIR)

clean::
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(TESTBIN_DIR)
	rm -rf $(TESTOBJ_DIR)
	rm -rf $(TESTCOVER_DIR)

.PHONY: clean
