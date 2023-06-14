#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 1024
static char msgBuffer[BUFFER_LENGTH];

int main()
{
    int ret,fd;
    char send[BUFFER_LENGTH] = {0};

   //打开mknod /dev/char_device c 220 0 设备
    fd = open("/dev/char_device", O_RDWR);
    if(fd < 0)
    {
        perror("fail to open device!\n");
        return errno;
    }

    printf("please type a char\n");
    scanf("%s",send);
    printf("writing to device\n");
    ret = write(fd,send,strlen(send));

    if(ret < 0)
    {
        perror("fail to write to device!\n");
        return errno;
    }

    printf("Press Enter to read back from device\n");
    getchar();

    printf("reading from device\n");
    ret = read(fd,msgBuffer, BUFFER_LENGTH); 
    if(ret < 0)
    {
        perror("fail to read from device!\n");
        return errno;
    }
    printf("the msg from device is:%s\n",msgBuffer);
    return 0;
}
