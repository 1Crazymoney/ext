
#include <ext/process>
#include <gtest/gtest.h>

TEST(process_test, run_inalid_cmd) {
  ext::process process("_invalid_");
#ifdef _WIN32
  EXPECT_EQ(process.last_error(), ERROR_FILE_NOT_FOUND);
  EXPECT_FALSE(process.joinable());
#else
  if (process.joinable())
    process.join();
#endif
}

TEST(process_test, run_list_working_directory_cmd) {
#ifdef _WIN32
  ext::process process("cmd", {"/c", "dir", "."});
#else
  ext::process process("ls", {"-al", "."});
#endif
  EXPECT_TRUE(process.joinable());
  process.join();
  EXPECT_EQ(process.exit_code(), EXIT_SUCCESS);
}

TEST(process_test,
     run_list_working_directory_cmd_with_working_directory_set_to_root) {
#ifdef _WIN32
  std::string systemDrive;
  size_t requiredCount = sizeof("C:");
  systemDrive.resize(requiredCount);
  errno_t err =
      getenv_s(&requiredCount, &systemDrive[0], requiredCount, "SystemDrive");
  if (err == ERANGE) {
    puts(std::to_string(requiredCount).c_str());
    systemDrive.resize(requiredCount);
    err =
        getenv_s(&requiredCount, &systemDrive[0], requiredCount, "SystemDrive");
  }
  EXPECT_EQ(err, 0);
  ext::process process("cmd", {"/c", "dir", "."}, systemDrive.c_str());
#else
  ext::process process("ls", {"-al", "."}, "/");
#endif
  EXPECT_TRUE(process.joinable());
  process.join();
  EXPECT_EQ(process.exit_code(), EXIT_SUCCESS);
}