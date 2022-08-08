// #include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // printf("Hello World!\n");

  // pid_t pid = getpid();

  // printf("pid: %lun\n", pid);

  if (2 != argc)
  {
    puts("Wrong argument count (should be 2)\n");
    return -1;
  }

  const int fd = open(argv[1], O_RDONLY | O_NONBLOCK);
  if (0 > fd)
  {
    printf("error opening %s (%d)\n", argv[1], fd);
    return -1;
  }

  printf("opened %s (fd: %d)\n", argv[1], fd);

  // designated initializer (C99 / C++20)
  pollfd info = {
      .fd      = fd,
      .events  = POLLIN,
      .revents = POLLIN,
  };

  const int retval = poll(&info, 1, 100'000);

  printf("poll returned %d\n", retval);

  close(fd);

  return 0;
}
