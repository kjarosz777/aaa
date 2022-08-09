#include "file_descriptor.hh"
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

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
