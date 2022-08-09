// #include <iostream>
#include "fmt/core.h"
#include <fcntl.h>
#include <poll.h>
#include <span>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

int main(int argc, const char* argv[])
{
  const std::span<const char*> arguments(argv, argc);

  if (2 != arguments.size())
  {
    fmt::print("Wrong argument count (should be 2, provided {})\n", arguments.size());
    return -1;
  }

  const int fd = open(arguments.back(), O_RDONLY | O_NONBLOCK);
  if (0 > fd)
  {
    fmt::print("error opening {} ({})\n", arguments.back(), fd);
    return -1;
  }

  fmt::print("opened {} (fd: {})\n", arguments.back(), fd);

  pollfd info = {
      .fd      = fd,
      .events  = POLLIN,
      .revents = POLLIN,
  };

  const int retval = poll(&info, 1, 100'000);

  fmt::print("poll returned {}\n", retval);

  close(fd);

  return 0;
}
