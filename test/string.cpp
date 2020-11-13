﻿#include <ext/string>
#include <gtest/gtest.h>

TEST(string_test, trim_test) {
  std::string str = " test ";  
  EXPECT_STREQ(ext::trim(str).c_str(), "test");

  std::wstring str_w = L" test ";  
  EXPECT_STREQ(ext::trim(str_w).c_str(), L"test");

  str = " test ";
  EXPECT_STREQ(ext::ltrim(str).c_str(), "test ");

  str_w = L" test ";  
  EXPECT_STREQ(ext::ltrim(str_w).c_str(), L"test ");

  str = " test ";
  EXPECT_STREQ(ext::rtrim(str).c_str(), " test");

  str_w = L" test ";
  EXPECT_STREQ(ext::rtrim(str_w).c_str(), L" test");

  EXPECT_STREQ(ext::trim(std::string(" test ")).c_str(), "test");
  EXPECT_STREQ(ext::trim(std::wstring(L" test ")).c_str(), L"test");
  EXPECT_STREQ(ext::ltrim(std::string(" test ")).c_str(), "test ");
  EXPECT_STREQ(ext::ltrim(std::wstring(L" test ")).c_str(), L"test ");
  EXPECT_STREQ(ext::rtrim(std::string(" test ")).c_str(), " test");
  EXPECT_STREQ(ext::rtrim(std::wstring(L" test ")).c_str(), L" test");

  EXPECT_STREQ(ext::trim(" test ").c_str(), "test");
  EXPECT_STREQ(ext::trim(L" test ").c_str(), L"test");
  EXPECT_STREQ(ext::ltrim(" test ").c_str(), "test ");
  EXPECT_STREQ(ext::ltrim(L" test ").c_str(), L"test ");
  EXPECT_STREQ(ext::rtrim(" test ").c_str(), " test");
  EXPECT_STREQ(ext::rtrim(L" test ").c_str(), L" test");
}

TEST(string_test, search_test) {
  EXPECT_TRUE(ext::search("hello, world :-)", "world"));
  EXPECT_TRUE(ext::search("hello, world :-)", "World"));

  EXPECT_TRUE(ext::search("hello, world :-)", "world", true));
  EXPECT_FALSE(ext::search("hello, world :-)", "World", true));
}

TEST(string_test, equal_test) {
  EXPECT_TRUE(ext::equal("hello, world :-)", "hello, world :-)"));
  EXPECT_TRUE(ext::equal("hello, world :-)", "hello, World :-)"));

  EXPECT_TRUE(ext::equal("hello, world :-)", "hello, world :-)", true));
  EXPECT_FALSE(ext::equal("hello, world :-)", "hello, World :-)", true));
}

TEST(string_test, replace_all_test) {
  std::string str = "hello, world :-)";
  EXPECT_STREQ(ext::replace_all(str, ":-", ";-").c_str(), "hello, world ;-)");

  std::wstring str_w = L"hello, world :-)";
  EXPECT_STREQ(ext::replace_all(str_w, L":-", L";-").c_str(),
               L"hello, world ;-)");
}
