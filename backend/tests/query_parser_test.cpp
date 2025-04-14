#include <gtest/gtest.h>
#include "filters/query-parser.hpp"
#include "filters/content-filter.hpp"
#include "filters/path-name-filter.hpp"
#include "filters/access-time-filter.hpp"
#include "filters/size-filter.hpp"
#include "filters/mime-type-filter.hpp"

template<typename T>
bool isFilterOfType(const std::unique_ptr<IFilter>& filter) {
    return dynamic_cast<T*>(filter.get()) != nullptr;
}

namespace tests {
    TEST(QueryParserTest, ValidComplexQueryParsesCorrectly) {
        std::string query = R"(content:file size:>300000 size<500000 accesstime:>2022 accesstime<2026-12 path:"D:\Facultate\3nd Year Sem1\GP" path:"D:\Facultate\3nd Year Sem1")";
        auto filters = QueryParser::parse(query);

        ASSERT_EQ(filters.size(), 7);
        EXPECT_TRUE(isFilterOfType<ContentFilter>(filters[0]));
        EXPECT_TRUE(isFilterOfType<SizeFilter>(filters[1]));
        EXPECT_TRUE(isFilterOfType<SizeFilter>(filters[2]));
        EXPECT_TRUE(isFilterOfType<AccessTimeFilter>(filters[3]));
        EXPECT_TRUE(isFilterOfType<AccessTimeFilter>(filters[4]));
        EXPECT_TRUE(isFilterOfType<PathNameFilter>(filters[5]));
        EXPECT_TRUE(isFilterOfType<PathNameFilter>(filters[6]));

    }

    TEST(QueryParserTest, MissingSeparatorThrows) {
        std::string query = "contentfile";
        EXPECT_THROW(QueryParser::parse(query), std::invalid_argument);
    }

    TEST(QueryParserTest, EmptyKeyThrows) {
        std::string query = ":file";
        EXPECT_THROW(QueryParser::parse(query), std::invalid_argument);
    }

    TEST(QueryParserTest, EmptyValueThrows) {
        std::string query = "content:";
        EXPECT_THROW(QueryParser::parse(query), std::invalid_argument);
    }

    TEST(QueryParserTest, UnknownKeyThrows) {
        std::string query = "unknownkey:value";
        EXPECT_THROW(QueryParser::parse(query), std::invalid_argument);
    }

    TEST(QueryParserTest, UnmatchedQuoteThrows) {
        std::string query = R"(path:"D:\Unfinished)";
        EXPECT_THROW(QueryParser::parse(query), std::invalid_argument);
    }

    TEST(QueryParserTest, DoubleOperatorThrows) {
        std::string query = "size:>=300000";
        EXPECT_THROW(QueryParser::parse(query), std::invalid_argument);
    }
}