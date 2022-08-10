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

  printf("Writing: %d", num_to_write);
  // write to pipe
  printf("******************\n");
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

  size_t sizeOfMemory = 32 * sizeof(int);

  auto memory = mmap(NULL, sizeOfMemory, PROT_WRITE, MAP_SHARED, fd, 0);
  if (memory == MAP_FAILED)
  {
    // fprintf(stderr, "error runWriteMmap errno=%d perror: ", errno);
    // perror("");
    printf("Error runWriteMmap errno: %s\n", strerror(errno));
    return;
  }

  int* memoryInt = reinterpret_cast<int*>(memory);

  std::fill(memoryInt, memoryInt + 32, 99);

  munmap(memory, sizeOfMemory);
}

void runRead(const int& fd ) {
	int num_read;

  ssize_t ret = read(fd, &num_read, sizeof(int));
  if( ret > 0) {
    //printf("Number read: %d ", ret);
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
