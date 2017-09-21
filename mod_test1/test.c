#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  int file = open("/dev/mydriver", O_RDWR);
  
  if(file < 0)
    {
      perror("open");
      exit(errno);
    }
  
  write(file, "hello", 6);
  
  close(file);
  
  return 0;
}
