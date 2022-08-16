#include "file_descriptor.hh"

#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <string.h>

#include <span>

void runWrite(const int& fd ) {
	int num_to_write = getpid();

  // write to pipe
  printf("******************\n");
  printf("Writing: %d\n", num_to_write);
  ssize_t ret = write(fd, &num_to_write, sizeof(int));
  if ( ret <= 0) {
    fprintf(stderr, "error writing ret=%ld errno=%d perror: ", ret, errno);
    perror("");
  } else {
    printf("Bytes written: %ld\n", ret);
  }
  printf("***************\n");
}

void runWriteMmap(const int& fd ) {
  long page_size = sysconf(_SC_PAGE_SIZE);

  auto memory = mmap(NULL, page_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if (memory == MAP_FAILED)
  {
    printf("Error runWriteMmap errno: %s\n", strerror(errno));
    return;
  }

  printf("runWriteMmap MMAP ptr: %p\n", memory);

  int* memoryValue = reinterpret_cast<int*>(memory);
  for (int i = 0; i < 32; ++i)
  {
      printf("%d: setting memoryValue: %d\n", i, 999);
      *(memoryValue+i) = 999;
  }

  munmap(memory, page_size);
}

void runReadMmap(const int& fd ) {
  long page_size = sysconf(_SC_PAGE_SIZE);

  auto memory = mmap(NULL, page_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if (memory == MAP_FAILED)
  {
    printf("Error runReadMmap errno: %s\n", strerror(errno));
    return;
  }

  printf("runReadMmap MMAP ptr: %p\n", memory);

  int* memoryValue = reinterpret_cast<int*>(memory);
  for (int i = 0; i < 32; ++i)
  {
      printf("%d: reading memoryValue: %d\n", i, *(memoryValue+i));
  }

  munmap(memory, page_size);
}

void runRead(const int& fd ) {
	int num_read;

  ssize_t ret = read(fd, &num_read, sizeof(int));
  if( ret > 0) {
    printf("*************************\n");
    printf("Number read from buffer device: %d\n",num_read);
    printf("Bytes read: %ld\n", ret);
    printf("*************************\n");
  } else {
    fprintf(stderr, "error reading ret=%ld errno=%d perror: ", ret, errno);
    perror("");
  }
}

int readOrWrite(int argc, const char *argv[]) {
	
  int fd;

	if ( (fd = open("/dev/aaa_kernel", O_RDWR)) < 0)
  {
		perror("");
    printf("error opening %s\n", argv[1]);
		// exit(1);
    return 1;
	}

  std::string variant(argv[1]);

  if ("r" == variant)
  {
    printf("Running runRead\n");
    runRead(fd);
  }
  else if ("w" == variant)
  {
    printf("Running runWrite\n");
    runWrite(fd);
  }
  else if ("wm" == variant)
  {
    printf("Running runWriteMmap\n");
    runWriteMmap(fd);
  }
  else if ("rm" == variant)
  {
    printf("Running runWriteMmap\n");
    runReadMmap(fd);
  }

	close(fd);

	return 0;
}

int main(int argc, const char* argv[])
{
  const std::span<const char*> arguments(argv, argc);

  if (2 != arguments.size())
  {
    printf("Wrong argument count (should be 2, provided %lu)\n", arguments.size());
    return -1;
  }

  std::vector<std::string> variants = {"r", "w", "rm", "wm"};

  if (std::find(variants.begin(), variants.end(), std::string(argv[1])) != variants.end())
  {
    return readOrWrite(argc, argv);
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
