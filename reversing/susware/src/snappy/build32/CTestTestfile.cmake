# CMake generated Testfile for 
# Source directory: C:/Users/a/Downloads/snappy
# Build directory: C:/Users/a/Downloads/snappy/build32
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(snappy_unittest "C:/Users/a/Downloads/snappy/build32/snappy_unittest")
  set_tests_properties(snappy_unittest PROPERTIES  WORKING_DIRECTORY "C:/Users/a/Downloads/snappy" _BACKTRACE_TRIPLES "C:/Users/a/Downloads/snappy/CMakeLists.txt;320;add_test;C:/Users/a/Downloads/snappy/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(snappy_unittest "C:/Users/a/Downloads/snappy/build32/snappy_unittest")
  set_tests_properties(snappy_unittest PROPERTIES  WORKING_DIRECTORY "C:/Users/a/Downloads/snappy" _BACKTRACE_TRIPLES "C:/Users/a/Downloads/snappy/CMakeLists.txt;320;add_test;C:/Users/a/Downloads/snappy/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(snappy_unittest "C:/Users/a/Downloads/snappy/build32/snappy_unittest")
  set_tests_properties(snappy_unittest PROPERTIES  WORKING_DIRECTORY "C:/Users/a/Downloads/snappy" _BACKTRACE_TRIPLES "C:/Users/a/Downloads/snappy/CMakeLists.txt;320;add_test;C:/Users/a/Downloads/snappy/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(snappy_unittest "C:/Users/a/Downloads/snappy/build32/snappy_unittest")
  set_tests_properties(snappy_unittest PROPERTIES  WORKING_DIRECTORY "C:/Users/a/Downloads/snappy" _BACKTRACE_TRIPLES "C:/Users/a/Downloads/snappy/CMakeLists.txt;320;add_test;C:/Users/a/Downloads/snappy/CMakeLists.txt;0;")
else()
  add_test(snappy_unittest NOT_AVAILABLE)
endif()
subdirs("third_party/googletest")
subdirs("third_party/benchmark")
