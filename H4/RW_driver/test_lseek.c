#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    int n = 5;
    int write = open("/dev/RW_driver", O_RDWR);

    lseek(write, n, SEEK_SET);

    return 0;
}