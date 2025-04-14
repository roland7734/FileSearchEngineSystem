#include <gtest/gtest.h>
#include <memory>
#include "filters/content-filter.hpp"
#include "model/file.hpp"
#include "config/config.hpp"
#include "service/insert-service.hpp"
#include "service/search-service.hpp"

void insertTestFile(Database& db, InsertService& insertService,
                    const std::string& name, double score, int accessCount, int searchCount) {
    std::string path = "test_files/" + name;
    std::string text = "searchterm";

    File file(path, text.size(), std::time(nullptr));
    file.setTextContent(text);
    file.setScore(score);

    insertService.insertBatchToDatabase({file});

    pqxx::work txn(*db.getConnection());
    txn.exec("INSERT INTO file_usage_stats (file_id, access_count, search_count) "
             "SELECT id, " + std::to_string(accessCount) + ", " + std::to_string(searchCount) +
             " FROM files WHERE path = " + txn.quote(path) + ";");
    txn.commit();
}

TEST(RankingTests, FilesAreRankedByScoreAndUsageStats) {
    Database db;
    db.connect();

    pqxx::work txn(*db.getConnection());
    txn.exec("TRUNCATE file_usage_stats, file_metadata, files RESTART IDENTITY CASCADE;");
    txn.commit();

    InsertService insert(&db);
    SearchService search(&db);

    insertTestFile(db, insert, "low_score_high_usage.txt", 10.0, 50, 50);
    insertTestFile(db, insert, "high_score_low_usage.txt", 100.0, 0, 0);
    insertTestFile(db, insert, "low_all.txt", 5.0, 0, 0);

    std::vector<std::unique_ptr<IFilter>> filters;
    filters.push_back(std::make_unique<ContentFilter>("searchterm"));
    auto results = search.searchQuery(std::move(filters));

    ASSERT_EQ(results.size(), 3);
    EXPECT_TRUE(results[0].getPath().find("low_score_high_usage.txt") != std::string::npos);
    EXPECT_TRUE(results[1].getPath().find("high_score_low_usage.txt") != std::string::npos);
    EXPECT_TRUE(results[2].getPath().find("low_all.txt") != std::string::npos);
}

TEST(RankingTests, EqualScoresPreserveInsertionOrder) {
    Database db;
    db.connect();

    pqxx::work txn(*db.getConnection());
    txn.exec("TRUNCATE file_usage_stats, file_metadata, files RESTART IDENTITY CASCADE;");
    txn.commit();

    InsertService insert(&db);
    SearchService search(&db);

    insertTestFile(db, insert, "fileA.txt", 50.0, 1, 1);
    insertTestFile(db, insert, "fileB.txt", 40.0, 2, 2);
    insertTestFile(db, insert, "fileC.txt", 30.0, 3, 3);

    std::vector<std::unique_ptr<IFilter>> filters;
    filters.push_back(std::make_unique<ContentFilter>("searchterm"));
    auto results = search.searchQuery(std::move(filters));

    ASSERT_EQ(results.size(), 3);
    EXPECT_TRUE(results[0].getPath().find("fileC.txt") != std::string::npos);
    EXPECT_TRUE(results[1].getPath().find("fileB.txt") != std::string::npos);
    EXPECT_TRUE(results[2].getPath().find("fileA.txt") != std::string::npos);
}

TEST(RankingTests, OnlyTop5FilesReturned) {
    Database db;
    db.connect();

    pqxx::work txn(*db.getConnection());
    txn.exec("TRUNCATE file_usage_stats, file_metadata, files RESTART IDENTITY CASCADE;");
    txn.commit();

    InsertService insert(&db);
    SearchService search(&db);

    for (int i = 0; i < 10; ++i) {
        insertTestFile(db, insert, "file" + std::to_string(i) + ".txt", 10.0 * i, 0, 0);
    }

    std::vector<std::unique_ptr<IFilter>> filters;
    filters.push_back(std::make_unique<ContentFilter>("searchterm"));
    auto results = search.searchQuery(std::move(filters));

    ASSERT_EQ(results.size(), 5);
    EXPECT_TRUE(results[0].getPath().find("file9.txt") != std::string::npos);
    EXPECT_TRUE(results[1].getPath().find("file8.txt") != std::string::npos);
    EXPECT_TRUE(results[4].getPath().find("file5.txt") != std::string::npos);
}
