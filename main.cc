#include "file_descriptor.hh"

#include <fcntl.h>
#include <poll.h>
#include <stdio.h>

#include <span>

int main(int argc, const char* argv[])
{
  const std::span<const char*> arguments(argv, argc);

  if (2 != arguments.size())
  {
    printf("Wrong argument count (should be 2, provided %lu)\n", arguments.size());
    return -1;
  }

  const FileDescriptor fd(open(arguments.back(), O_RDONLY | O_NONBLOCK));
  if (!fd.IsValid())
  {
    printf("error opening %s (%d)\n", arguments.back(), fd.Get());
    return -1;
  }

  printf("opened %s (fd: %d)\n", arguments.back(), fd.Get());

  const int retval = fd.Poll(POLLIN, POLLIN);

  printf("poll returned %d\n", retval);

  return 0;
}
