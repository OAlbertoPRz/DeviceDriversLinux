#include "header.h"


#define WR_DATA _IOW('a','a', int32_t*)
#define RD_DATA _IOR('a','b', int32_t*)

int main(){
	int fd;
	int32_t val;
	int num;
	printf("\nIOCTL based Character device driver operations from user space...\n");
	fd=open("/dev//My_device", O_RDWR);
	if(fd < 0){
		printf("Cannot open the device file\n");
		return 0;
	}
	printf("Enter de data to be sent\n");
	scanf("%d", &num);
	printf("Wrtiting value to the driver...\n");
	ioctl(fd, WR_DATA, (int32_t*)&num);
	
	printf("Reading the value from driver...\n");
	ioctl(fd, RD_DATA,(int32_t*)&val);
	printf("Value is: %d\n",val);
	printf("Closing driver\n");
	close(fd);

}

