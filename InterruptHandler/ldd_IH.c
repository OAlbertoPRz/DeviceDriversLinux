#include "ldd_headers.h"			// Includes the essential libraries

// CONSTANTS
#define MEMSIZE 1024


/**************************************************************************************************************************************************************************************************************
 										** GENERAL DECLARATIONS **
**************************************************************************************************************************************************************************************************************/
static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);

// file operations
static int chr_open(struct inode *inode, struct file *file);
static int chr_release(struct inode *inode, struct file *file);
static ssize_t chr_read(struct file *file, char __user *buf, size_t len, loff_t *offset);
static ssize_t chr_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);

// defining the device structure (Major/minor)
dev_t dev = 0;

// Structure of the character device
struct cdev chrdev;

// Kernel buffer
uint8_t *kernel_buffer;

// Defining class
static struct class *chr_class;

/**************************************************************************************************************************************************************************************************************
 										** DEFINITION OF FILE OPERATIONS **
**************************************************************************************************************************************************************************************************************/
//Structure of file operations
static struct file_operations fops={
	.owner = THIS_MODULE,
	.open = chr_open,
	.release = chr_release,
	.read = chr_read,
	.write = chr_write,
};


static int chr_open(struct inode *inode, struct file *file){
	/* Creating a Physical Memory */
	if((kernel_buffer = kmalloc(MEMSIZE, GFP_KERNEL)) == 0)
	{
		printk(KERN_INFO "Cannot allocate memory to the kernel...\n");
		return -1;
	}
	printk(KERN_INFO"Memory succesfully allocated into the kernel...\nDevice file opened...\n");
	return 0;
}


static int chr_release(struct inode *inode, struct file *file){
	kfree(kernel_buffer);
	printk(KERN_INFO"Memory succesfully released\n");
	return 0;
}


static ssize_t chr_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
	int8_t resp;
	resp = copy_to_user(buf, kernel_buffer, MEMSIZE);
	printk(KERN_INFO "Data was read succesfully: %d\n", resp);
	return MEMSIZE;
}


static ssize_t chr_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
	int8_t resp;
	resp = copy_from_user(kernel_buffer,buf, len);
	printk(KERN_INFO"Data was written succesfully: %d\n", resp);
	return len;
}


/**************************************************************************************************************************************************************************************************************
 										**	 MODULE INIT 	**
**************************************************************************************************************************************************************************************************************/
static int __init chr_driver_init(){
	// Allocation of memory
	if((alloc_chrdev_region(&dev,0,1,"Char_dev")) < 0){				// Dinamically allocates the Major and Minor number by itself
		printk(KERN_INFO"Cannot allocate the major number...\n");
		return -1;
	}
	printk(KERN_INFO"Major = %d.\nMinor = %d.\n", MAJOR(dev), MINOR (dev));

	// Creating a char type device structure
	cdev_init(&chrdev, &fops);							// unitializes the dev

	// Adding char type device to the system
	if((cdev_add(&chrdev, dev, 1)) <0){
		printk(KERN_INFO"Cannot add the device to the system\n");
		goto release_class;
	}

	//Creating the class structure
	if((chr_class = class_create(THIS_MODULE,"Char_class")) == NULL){
	       	printk(KERN_INFO"Enable to create the class structure\n");
       		goto release_class;
	}		
	
	// Creating the device
	if((device_create(chr_class,NULL, dev, NULL, "Char_dev")) == NULL){
		printk(KERN_INFO"Unable to allocate the device\n");
		goto release_device;
	}
	
	printk(KERN_INFO"Char Device Driver inserted...\nProperly done...\n");
	return 0;


	// Release subrutines
	release_class:
		class_destroy(chr_class);
	release_device:
		unregister_chrdev_region(dev,0);
		cdev_del(&chrdev);
		return -1;

}


/**************************************************************************************************************************************************************************************************************
 										**	 MODULE EXIT 	**
**************************************************************************************************************************************************************************************************************/
static void __exit chr_driver_exit(void){
	device_destroy(chr_class,dev);
	class_destroy(chr_class);
	cdev_del(&chrdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO"Device Driver was removed succesfully\n\n");
}





/**************************************************************************************************************************************************************************************************************
 										** INFORMATION AND MODULE INITIALIZATION **
**************************************************************************************************************************************************************************************************************/

module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OA Perez Ruiz, oa.perezruiz@gmail.com");
MODULE_DESCRIPTION("The character device driver");
