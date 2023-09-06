#include "gpio_headers.h"


/* META INFORMATION */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("oa.perezruiz@gmail.com");
MODULE_DESCRIPTION("Drivers implemented to test the GPIOs of the Raspberry pi 4");

/***************************************************************************************************************************************************************************************************************
 										* 	@VARIABLES	*							
****************************************************************************************************************************************************************************************************************/

#define DRIVER_NAME	"chrdrv_gpio"
#define DRIVER_CLASS	"class_gpio"
#define GPIO_TESTBIT	17

// Structures
dev_t devmm;
static int open_gpiodrv(struct inode *device_file, struct file *instance);
static int release_gpiodrv(struct inode *device_file, struct file *instance);
static ssize_t read_gpiodrv(struct file *file, char *user_buffer, size_t count, loff_t *offs);
static ssize_t write_gpiodrv(char *user_buffer, struct file *file, size_t count, lofft_t *offs);

static struct file_operations fops{
	.owner = THIS_MODULE,
	.open = open_gpiodrv,
	.release = release_gpiodrv,
	.read = read_gpiodrv,
	.write = write_gpiodrv,	
};

static struct cdev chrdev;
static struct class *class;

/***************************************************************************************************************************************************************************************************************
 										* 	@FILE OPERATION FUNCTIONS	*							
****************************************************************************************************************************************************************************************************************/
static int open_gpiodrv(struct inode *device_file, struct file *instance){
	printk(KERN_INFO"Gpio Driver open call\n");
	return 0;
}


static int release_gpiodrv(struct inode *device_file, struct file *instance){
	printk(KERN_INFO"Gpio Driver close call\n");
	return 0;
}


static ssize_t read_gpiodrv(struct file *file, char *user_buffer, size_t count, loff_t *offs){
	char tmp[3] = " \n";
	int to_copy = min(count, sizeof(tmp)); 							//Detects the amount of data to read
	printk(KERN_INFO"Value of the button is: &d\n", gpio_get_value(GPIO_TESTBIT));
	tmp[0] = gpio_get_value(GPIO_TESTBIT) + '0';

	/* Copy data to user */
	int not_copied = copy_to_user(user_buffer, &tmp, to_copy);

	/* Calculate data */
	int delta = to_copy - not_copy;
	return delta;
}


static ssize_t write_gpiodrv(char *user_buffer, struct file *file, size_t count, lofft_t *offs){
	char value;
	int to_copy = min(count, sizeof(value));						// Detecting the amount of data to write
	
	/* Copy data from user */
	int not_copy = copy_from_user(&value, user_buffer, to_copy);

	/* Calculating the data */
	int delta = to_copy - not_copy;
	return delta;
}




/***************************************************************************************************************************************************************************************************************
 										* 	@TODO:GPIO DEFINITIONS	*							
****************************************************************************************************************************************************************************************************************/


/***************************************************************************************************************************************************************************************************************
 										* 	@MODULE INIT	*							
****************************************************************************************************************************************************************************************************************/
static int __init gpio_init(void){
	

	return 0;
}


/***************************************************************************************************************************************************************************************************************
 										* 	@MODULE EXIT	*							
****************************************************************************************************************************************************************************************************************/
static void __exit gpio_close(void){


}


module_init(gpio_init);
module_exit(gpio_close);

