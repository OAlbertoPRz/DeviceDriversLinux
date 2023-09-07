#include "gpio_headers.h"


/* META INFORMATION */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("oa.perezruiz@gmail.com");
MODULE_DESCRIPTION("Drivers implemented to test the GPIOs of the Raspberry pi 4");

/***************************************************************************************************************************************************************************************************************
 										* 	@VARIABLES	*							
****************************************************************************************************************************************************************************************************************/

#define DRIVER_NAME		"chrdrv_gpio"
#define DRIVER_CLASS	"class_gpio"
#define GPIO_TESTBIT	17
#define MEM_SIZE		1024


// Structures
dev_t devmm;
static int open_gpiodrv(struct inode *device_file, struct file *instance);
static int release_gpiodrv(struct inode *device_file, struct file *instance);
static ssize_t read_gpiodrv(struct file *filp, char __user *user_buffer, size_t size, loff_t *off);
static ssize_t write_gpiodrv(struct file *filp, const char __user *user_buffer, size_t size, loff_t *off);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open_gpiodrv,
	.release = release_gpiodrv,
	.read = read_gpiodrv,
	.write = write_gpiodrv,	
};

static struct cdev 	chrdev;
static struct class *gpio_class;

// buffer
uint8_t *kernel_buffer;

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


static ssize_t read_gpiodrv(struct file *filp, char __user *user_buffer, size_t size, loff_t *off){
	if(copy_to_user(user_buffer,kernel_buffer,MEM_SIZE)) 
		printk(KERN_INFO"Couldn't read properly...\n");
	printk(KERN_INFO "Data read: DONE...\n");
	return MEM_SIZE;
}


static ssize_t write_gpiodrv(struct file *filp, const char __user *user_buffer, size_t size, loff_t *off){
	if(copy_from_user(kernel_buffer, user_buffer, size))
		printk(KERN_INFO"Couldn't write properly...\n");
	printk(KERN_INFO "Data was written succesfully...\n");
	return size;
}




/***************************************************************************************************************************************************************************************************************
 										* 	@TODO:GPIO DEFINITIONS	*							
****************************************************************************************************************************************************************************************************************/


/***************************************************************************************************************************************************************************************************************
 										* 	@MODULE INIT	*							
****************************************************************************************************************************************************************************************************************/
static int __init gpio_init(void){
	printk(KERN_INFO"Module inserted...\n");
	// Allocate the char device in the memory
	if(alloc_chrdev_region(&devmm, 0, 1, DRIVER_NAME) < 0){
		printk(KERN_INFO"Allocating the char device\n");
		return -1;
	}
	
	printk("Read-Write - Device Major: %d, Device Minor: %d was registered\n", MAJOR(devmm), MINOR(devmm));
	
	// Create device class
	if((gpio_class=class_create(THIS_MODULE, DRIVER_CLASS)) == NULL){
		printk(KERN_INFO"Device Class can not be created\n");
		goto ClassError;
	}

	// Create and Assign the device into the class
	if(device_create(gpio_class, NULL, devmm, NULL, DRIVER_NAME)==NULL){
		printk("Can not create device file\n");
		goto FileError;
	}

	// Initialize the device
	cdev_init(&chrdev, &fops);

	// Register device to kernel
	if(cdev_add(&chrdev, devmm, 1)==-1){
		printk(KERN_INFO"Registering of the device in the kernel have failed\n");
		goto AddError;
	}

	return 0;
	
	AddError:
		device_destroy(gpio_class, devmm);
	FileError:
		class_destroy(gpio_class);
	ClassError:
		unregister_chrdev_region(devmm, 1);
		return -1;

}


/***************************************************************************************************************************************************************************************************************
 										* 	@MODULE EXIT	*							
****************************************************************************************************************************************************************************************************************/
static void __exit gpio_close(void){
	cdev_del(&chrdev);
	device_destroy(gpio_class, devmm);
	class_destroy(gpio_class);
	unregister_chrdev_region(devmm,1);
	printk(KERN_INFO"Removing the module\n");
}


module_init(gpio_init);
module_exit(gpio_close);

