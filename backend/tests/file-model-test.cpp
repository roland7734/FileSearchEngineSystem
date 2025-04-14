#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <ctime>

#include "model/file.hpp"
#include "test_utils.hpp"



TEST(FileTest, ConstructorInitializesFieldsCorrectly) {
    std::string tempPath = "test_files/test1.txt";
    std::string content = "This is a test file.";
    createTempFile(tempPath, content);

    std::time_t now = std::time(nullptr);
    File f(tempPath, content.size(), now);

    EXPECT_EQ(f.getPath(), tempPath);
    EXPECT_EQ(f.getSize(), content.size());
    EXPECT_EQ(f.getCreatedAt(), now);
    EXPECT_TRUE(f.getMimeType().find("text") != std::string::npos);
    EXPECT_EQ(f.getTextContent(), content);
}

TEST(FileTest, ConstructorWithTextOnlySetsContentCorrectly) {
    std::string path = "test_files/manual.txt";
    std::string text = "Sample manual content.";
    File f(path, text);

    EXPECT_EQ(f.getPath(), path);
    EXPECT_EQ(f.getTextContent(), text);
}

TEST(FileTest, ScoreIsNeverNegative) {
    std::string tempPath = "test_files/old.txt";
    std::string content = "Old content";
    createTempFile(tempPath, content);

    std::time_t oldTime = std::time(nullptr) - 10 * 365 * 24 * 60 * 60;
    File f(tempPath, content.size(), oldTime);

    EXPECT_GE(f.getScore(), 0.0);
}

TEST(FileTest, NameAndExtensionAreExtractedCorrectly) {
    std::string tempPath = "test_files/myfile.report.txt";
    std::string content = "hello";
    createTempFile(tempPath, content);

    File f(tempPath, content.size(), std::time(nullptr));

    EXPECT_EQ(f.getName(), "myfile.report");
    EXPECT_EQ(f.getExtension(), ".txt");
}

TEST(FileTest, NonTextMimeTypeSkipsContentExtraction) {
    std::string binaryFile = "test_files/image.jpg";
    createTempFile(binaryFile, std::string("\xFF\xD8\xFF\xE0", 4));

    File f(binaryFile, 4, std::time(nullptr));

    if (f.getMimeType().find("text") == std::string::npos) {
        EXPECT_TRUE(f.getTextContent().empty());
    } else {
        SUCCEED();
    }
}