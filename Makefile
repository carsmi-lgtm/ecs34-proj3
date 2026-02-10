# tools
AR=ar
CC=gcc
CXX=g++

# directories
INC_DIR = ./include
SRC_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = ./obj
LIB_DIR = ./lib
TESTSRC_DIR = ./testsrc
TESTOBJ_DIR = ./testobj
TESTBIN_DIR = ./testbin
TESTCOVER_DIR = ./htmlconv

#flags
PKGS = expat
DEFINES =
INCLUDES += -I $(INC_DIR)
CFLAGS += `pkg-config -cflags $(PKGS)`
CPPFLAGS += -std=c++20
LDFLAGS = `pkg-config -libs $(PKGS)`

TEST_CFLAGS = $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS = $(CPPFLAGS) -fno-inline -Iinclude
TEST_LDFLAGS = $(LDFLAGS) -lgtest_main -lgtest  -lpthread -lexpat


# test object files
TEST_OBJ_FILES_STRUTILS = $(TESTOBJ_DIR)/StringUtilsTest.o $(TESTOBJ_DIR)/StringUtils.o
TEST_OBJ_FILES_STRDTASRC = $(TESTOBJ_DIR)/StringDataSourceTest.o $(TESTOBJ_DIR)/StringDataSource.o
TEST_OBJ_FILES_STRDTASINK = $(TESTOBJ_DIR)/StringDataSinkTest.o $(TESTOBJ_DIR)/StringDataSink.o
TEST_OBJ_FILES_DSV = $(TESTOBJ_DIR)/DSVTest.o $(TESTOBJ_DIR)/DSVReader.o $(TESTOBJ_DIR)/DSVWriter.o $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/StringDataSink.o
TEST_OBJ_FILES_XML = $(TESTOBJ_DIR)/XMLTest.o $(TESTOBJ_DIR)/XMLReader.o $(TESTOBJ_DIR)/XMLWriter.o $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/StringDataSink.o
TEST_OBJ_FILES_CSVBS = $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/DSVReader.o $(TESTOBJ_DIR)/CSVBusSystem.o $(TESTOBJ_DIR)/CSVBusSystemTest.o

# test executables
TEST_TARGET_STRUTILS = $(TESTBIN_DIR)/teststrutils
TEST_TARGET_STRDTASRC = $(TESTBIN_DIR)/teststrdatasource
TEST_TARGET_STRDTASINK = $(TESTBIN_DIR)/teststrdatasink
TEST_TARGET_DSV = $(TESTBIN_DIR)/testdsv
TEST_TARGET_XML = $(TESTBIN_DIR)/testxml
TEST_TARGET_CSVBS = $(TESTBIN_DIR)/testcsvbs


all: directories run_strtest run_strsrctest run_strsinktest run_dsvtest run_xmltest run_csvbstest gencoverage

run_strtest: $(TEST_TARGET_STRUTILS)
	$(TEST_TARGET_STRUTILS)

run_strsrctest: $(TEST_TARGET_STRDTASRC)
	$(TEST_TARGET_STRDTASRC)

run_strsinktest: $(TEST_TARGET_STRDTASINK)
	$(TEST_TARGET_STRDTASINK)

run_dsvtest: $(TEST_TARGET_DSV)
	$(TEST_TARGET_DSV)

run_xmltest: $(TEST_TARGET_XML)
	$(TEST_TARGET_XML)

run_csvbstest: $(TEST_TARGET_CSVBS)
	$(TEST_TARGET_CSVBS)

gencoverage:
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

$(TEST_TARGET_CSVBS): $(TEST_OBJ_FILES_CSVBS)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES_CSVBS) $(TEST_LDFLAGS) -o $(TEST_TARGET_CSVBS)


# compile
$(TESTOBJ_DIR)/%.o: $(TESTSRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@


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
