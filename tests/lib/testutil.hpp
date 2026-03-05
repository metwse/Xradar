#ifndef TESTUTIL_HPP
#define TESTUTIL_HPP


#include <filesystem>  // IWYU pragma: export, used by TEST_COMPONENT_LOADER


#define TEST_COMPONENT_LOADER \
    std::filesystem::path lib_dir { argv[0] }; \
    component_loader cl { lib_dir.parent_path() };


#endif
