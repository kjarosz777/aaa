// #include <iostream>
#include "fmt/core.h"
#include <fcntl.h>
#include <poll.h>
#include <span>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class FileDescriptor
{
public:
  explicit FileDescriptor(int fd);
  ~FileDescriptor();

  bool IsValid() const;
  int  Get() const;
  int  Poll(int16_t events, int16_t revents) const;

private:
  int m_FD;
};

FileDescriptor::FileDescriptor(int fd)
    : m_FD(fd)
{
}

FileDescriptor::~FileDescriptor()
{
  if (IsValid())
  {
    close(m_FD);
  }
}

bool FileDescriptor::IsValid() const
{
  return 0 <= m_FD;
}

int FileDescriptor::Get() const
{
  return m_FD;
}

int FileDescriptor::Poll(int16_t events, int16_t revents) const
{
  pollfd info = {
      .fd      = m_FD,
      .events  = events,
      .revents = revents,
  };

  return poll(&info, 1, -1);
}

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
