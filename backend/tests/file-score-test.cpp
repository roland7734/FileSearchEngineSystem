#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <ctime>

#include "model/file.hpp"
#include "config/config.hpp"
#include "test_utils.hpp"


TEST(FileScoreTest, DirectoryBonusAppliedCorrectly) {
    std::string path = "test_files/project/src/main.cpp";
    std::string content = "int main() { return 0; }";
    createTempFile(path, content);

    File f(path, content.size(), std::time(nullptr));
    EXPECT_GT(f.getScore(), 0.0);
}

TEST(FileScoreTest, SmallFileGetsBonus) {
    std::string path = "test_files/readme.md";
    std::string content = "Short file.";
    createTempFile(path, content);

    File f(path, content.size(), std::time(nullptr));
    EXPECT_GE(f.getScore(), Config::SMALL_FILE_BONUS);
}

TEST(FileScoreTest, PathDepthAffectsScore) {
    std::string shallowPath = "test_files/a.cpp";
    std::string deepPath = "test_files/a/b/c/d/e/f/g/h/deep.cpp";

    createTempFile(shallowPath, "a");
    createTempFile(deepPath, "a");

    File shallow(shallowPath, 1, std::time(nullptr));
    File deep(deepPath, 1, std::time(nullptr));

    EXPECT_GT(shallow.getScore(), deep.getScore());
}

TEST(FileScoreTest, TextExtensionIncreasesScore) {
    std::string txtPath = "test_files/data.txt";
    std::string nonTextPath = "test_files/data.bin";

    createTempFile(txtPath, "text");
    createTempFile(nonTextPath, "binary");

    File textFile(txtPath, 4, std::time(nullptr));
    File binaryFile(nonTextPath, 4, std::time(nullptr));

    EXPECT_GT(textFile.getScore(), binaryFile.getScore());
}

TEST(FileScoreTest, AgeDecreasesScore) {
    std::string path = "test_files/oldfile.cpp";
    createTempFile(path, "int x = 0;");

    std::time_t oldTime = std::time(nullptr) - (30 * 86400);
    File oldFile(path, 11, oldTime);
    File newFile(path, 11, std::time(nullptr));

    EXPECT_LT(oldFile.getScore(), newFile.getScore());
}

