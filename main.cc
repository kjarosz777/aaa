#include "file_descriptor.hh"
#include "fmt/core.h"

#include <fcntl.h>
#include <poll.h>
#include <stdio.h>

#include <span>

int main(int argc, const char* argv[])
{
  const std::span<const char*> arguments(argv, argc);

  if (2 != arguments.size())
  {
    fmt::print("Wrong argument count (should be 2, provided {})\n", arguments.size());
    return -1;
  }

  const FileDescriptor fd(open(arguments.back(), O_RDONLY | O_NONBLOCK));
  if (!fd.IsValid())
  {
    fmt::print("error opening {} ({})\n", arguments.back(), fd.Get());
    return -1;
  }

  fmt::print("opened {} (fd: {})\n", arguments.back(), fd.Get());

  const int retval = fd.Poll(POLLIN, POLLIN);

  fmt::print("poll returned {}\n", retval);

  return 0;
}
