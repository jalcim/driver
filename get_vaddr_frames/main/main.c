#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  int file = open("/dev/devalloc", O_RDWR);

  if(file < 0)
    {
      perror("open");
      exit(errno);
    }

  read(file, "hello", 6);
  write(file, "hello", 6);

  close(file);

  return 0;
}
