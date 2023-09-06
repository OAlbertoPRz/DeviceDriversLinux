#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>					// kmalloc() function
#include <linux/uaccess.h>				// copy_from/to_user() functions
#include <linux/ioctl.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MEM_SIZE 1024


// Procfs
char chr_array[40]="A text for my Example\n";
static int len = 1;



// Define de ioctl code
#define WR_DATA _IOW('a','a', int32_t*)
#define RD_DATA _IOR('a','b', int32_t*)

int32_t val=0;


dev_t dev = 0;				// Device
static struct class *dev_class;
static struct cdev my_cdev;		
uint8_t *kernel_buffer;

/*     											FUNCTIONS									*/
static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static long chr_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

/*
 										* PROCFS FUNCTIONS *		
 */
static ssize_t read_proc(struct file *file, char __user *user, size_t size, loff_t *off);
static ssize_t write_proc(struct file *file, const char __user *user, size_t size, loff_t *off);




/************************************************************************************ procfs *****************************************************************************************************************/
static char data_buffer[MEM_SIZE];


static const struct proc_ops ldd_proc_fops = {
	.proc_read = read_proc,
	.proc_write = write_proc,
};


static ssize_t read_proc(struct file *file, char __user *user, size_t size, loff_t *off){
	copy_to_user(user, "Hello!\n",7);
	return 7;

}


static ssize_t write_proc(struct file *file, const char __user *user, size_t size, loff_t *off){
	memset(data_buffer,0x0,sizeof(data_buffer));
	if(size > MEM_SIZE)
		size = MEM_SIZE;
	copy_from_user(data_buffer, user, size);
	printk("You said '%s'!\n", data_buffer);
	return size;
}
/************************************************************************************ char device driver functions *****************************************************************************************/
static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.unlocked_ioctl = chr_ioctl,
	.release = my_release,
};



static int my_open(struct inode *inode, struct file *file)
{
	/* Creating a Physical Memory */
	if((kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL)) == 0)
	{
		printk(KERN_INFO "Cannot allocate memory to the kernel...\n");
		return -1;
	}
	printk(KERN_INFO"Memory succesfully allocated into the kernel...\nDevice file opened...\n");
	return 0;
}


static int my_release(struct inode *inode, struct file *file)
{
	kfree(kernel_buffer);
	printk(KERN_INFO "Memory released...\nDevice file closed...\n");
	return 0;
}


static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	copy_to_user(buf,kernel_buffer,MEM_SIZE);
	printk(KERN_INFO "Data read: DONE...\n");
	return MEM_SIZE;
}


static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	copy_from_user(kernel_buffer, buf, len);
	printk(KERN_INFO "Data was written succesfully...\n");
	return len;
}


static long chr_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	switch(cmd){
		case WR_DATA:
			copy_from_user(&val, (int32_t*)arg, sizeof(val));
			printk(KERN_INFO"Val= %d\n", val);
			break;
		case RD_DATA:
			copy_to_user((int32_t*)arg, &val, sizeof(val));
			break;
	}
	return 0;
}




static int __init chr_driver_init(void)
{
	/* Allocation of memory */
	if((alloc_chrdev_region(&dev,0,1,"My_device")) < 0){
		printk(KERN_INFO"Cannot allocate the major number..\n");
		return -1;
	}
	printk(KERN_INFO"Major = %d. Minor = %d..\n", MAJOR(dev), MINOR(dev));
	
	/* Creating a cdev structure */
	cdev_init(&my_cdev, &fops);

	/* Adding character device to the system */
	if((cdev_add(&my_cdev,dev,1)) < 0) 
	{
		printk(KERN_INFO"Cannot add the device to the system ...\n");
		goto r_class;
	}

	/* Creating structure class */
	if((dev_class = class_create(THIS_MODULE,"My_class")) == NULL)
	{
		printk(KERN_INFO"Enable to creacre the struct class...\n");
		goto r_class;
	}

	/* Creating device */
	if((device_create(dev_class,NULL,dev,NULL,"My_device")) == NULL)
	{
		printk(KERN_INFO"Unable to allocate the device...\n");
		goto r_device;
	}


	/**************************************************************************************  Creating the Proc entry **************************************************************************************/
	int ldd_proc = proc_create("lll-proc", 066, NULL, &ldd_proc_fops);
	if (ldd_proc == NULL){
		printk(KERN_INFO"Cannot create lll_proc");
		return -1;
	}
		

	printk(KERN_INFO"Device driver inserted...\nDone properly...\n");
	return 0;

	r_device:
		class_destroy(dev_class);

	r_class:
		unregister_chrdev_region(dev,0);
		cdev_del(&my_cdev);
		return -1;
}


static void __exit chr_driver_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO"Device driver was removed succesfully...\n");

}


module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OA Perez Ruiz, oa.perezruiz@gmail.com");
MODULE_DESCRIPTION("The character device driver");
