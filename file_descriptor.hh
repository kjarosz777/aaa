#pragma once

#include <cstdint>

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
