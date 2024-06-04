# building the tests
option(FEATURE_TESTS "Enable the tests" OFF)

# building the docs
option(FEATURE_DOCS "Enable the docs" OFF)
option(ENABLE_OPENSSL "Enable OpenSSL support" OFF)
# fuzz tests
option(FEATURE_FUZZ_TESTS "Enable the fuzz tests" OFF)
option(ENABLE_BENCHMARKS "Enable benchmark tests" OFF)
option(ENABLE_CROSS_COMPILING "Detect cross compiler and setup toolchain" OFF)
option(ENABLE_SAMPLE_PROJECT_COMPILING "Enable sample projects" OFF)
option(BUILD_SHARED_LIBS "Build shared instead of static libraries." ON)
option(ENABLE_MANUAL_SETUP "Download some libs from git repo." ON)

if(FEATURE_DOCS)
  set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
endif()
