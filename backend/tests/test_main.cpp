#include <gtest/gtest.h>
#include <filesystem>
#include "config/config.hpp"

int main(int argc, char** argv) {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path config_path = current_path / "config_test.json";
    Config::loadConfig(config_path.string());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
