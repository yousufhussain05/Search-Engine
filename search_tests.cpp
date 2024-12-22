#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

#pragma region cleanTokenTests

TEST(CleanTokens, noPunctOrSpecialChars) {
  string text = "helloWorld123";
  string expected = "helloworld123";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, punctAtBegin) {
  string text = "!hello";
  string expected = "hello";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, punctAtEnd) {
  string text = "hello!";
  string expected = "hello";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, noLetters) {
  string text = "";
  string expected = "";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, withUpperAndPunct) {
  string text = "Hello,World!";
  string expected = "hello,world";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, punctAtBeginAndEnd) {
  string text = "!HelloWorld!";
  string expected = "helloworld";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, punctAtMiddleNotEndButStart) {
  string text = "!Hello...World";
  string expected = "hello...world";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}
TEST(CleanTokens, punctOnlyMiddle) {
  string text = "Hello!World";
  string expected = "hello!world";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}
TEST(CleanTokens, punctAtBeginAndEndAndMiddle) {
  string text = "!Hello,World!";
  string expected = "hello,world";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}
TEST(CleanTokens, punctAtMiddleAndEnd) {
  string text = "Hello.World!";
  string expected = "hello.world";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}
TEST(CleanTokens, numAtEnd) {
  string text = "helloworld1";
  string expected = "helloworld1";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, oneLetter) {
  string text = "h";
  string expected = "h";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, alotPunct) {
  string text = "......A......";
  string expected = "a";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

TEST(CleanTokens, startEndWithElse) {
  string text = "\"HI!\"";
  string expected = "hi";

  EXPECT_THAT(cleanToken(text), StrEq(expected));
}

#pragma endregion cleanTokenTests

#pragma region gatherTokensTests

TEST(GatherTokens, leadingSpaces) {
  string text = "  Hello World";
  set<string> expected = {"hello", "world"};

  EXPECT_THAT(gatherTokens(text), Eq(expected));
}

TEST(GatherTokens, trailingSpaces) {
  string text = "Hello World   ";
  set<string> expected = {"hello", "world"};

  EXPECT_THAT(gatherTokens(text), Eq(expected));
}

TEST(GatherTokens, multipleSpaces) {
  string text = "       Hello        World   ";
  set<string> expected = {"hello", "world"};

  EXPECT_THAT(gatherTokens(text), Eq(expected));
}

#pragma endregion gatherTokensTests

#pragma region buildIndexTests

TEST(BuildIndex, tinyTextFile) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedMap = {
      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},
      {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},
      {"one", {"www.dr.seuss.net"}},
      {"two", {"www.dr.seuss.net"}},
      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}}};
  map<string, set<string>> actualMap;
  int numProcesed = buildIndex(filename, actualMap);
  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(actualMap, ContainerEq(expectedMap)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(numProcesed, Eq(4)) << retTestFeedback;
}

TEST(BuildIndex, fileNotFound) {
  string filename = "data/missing.txt";
  map<string, set<string>> expectedMap;
  map<string, set<string>> actualMap;
  int numProcesed = buildIndex(filename, actualMap);
  EXPECT_THAT(buildIndex(filename, actualMap), Eq(0))
      << "text = \\file not found\\" << endl;
}

#pragma endregion buildIndexTests

#pragma region findQueryMatchesTests

TEST(FindQueryMatches, firstQueryTermNotInIndex) {
  string filename = "data/tinier.txt";
  set<string> expected;
  map<string, set<string>> INDEX = {{"hello", {"example.com", "uic.edu"}},
                                    {"world", {"example.com"}}};
  expected = {};
  
  EXPECT_EQ(findQueryMatches(INDEX, "there"), expected);
}

TEST(FindQueryMatches, LaterQueryTermNotInIndex) {
  set<string> expected;
  map<string, set<string>> INDEX = {{"hello", {"example.com", "uic.edu"}},
                                    {"world", {"example.com"}}
                                  };
  expected = {"example.com", "uic.edu"};

  EXPECT_EQ(findQueryMatches(INDEX, "hello there"), expected);
}

TEST(FindQueryMatches, LaterQueryTermWithPlusModifierNotInIndex) {
  set<string> expected;
  map<string, set<string>> INDEX = {{"hello", {"example.com", "uic.edu"}},
                                    {"world", {"example.com"}}
                                  };
  expected = {};

  EXPECT_EQ(findQueryMatches(INDEX, "hello +missing"), expected);

}
TEST(FindQueryMatches, LaterQueryTermWithMinusModifierNotInIndex) {
  set<string> expected;
  map<string, set<string>> INDEX = {{"hello", {"example.com", "uic.edu"}},
                                    {"world", {"example.com"}}
                                  };
  expected = {"example.com", "uic.edu"};

  expected = {"example.com"};
  EXPECT_EQ(findQueryMatches(INDEX, "world -missing"), expected);
}


#pragma endregion buildIndexTests
