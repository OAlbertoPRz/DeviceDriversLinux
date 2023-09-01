#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe63d5a03, "module_layout" },
	{ 0x6da07ee0, "device_destroy" },
	{ 0x6906c0d6, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xb298edeb, "class_destroy" },
	{ 0xd57fb629, "device_create" },
	{ 0x4a253998, "__class_create" },
	{ 0xe9916d24, "cdev_add" },
	{ 0x9613cc8e, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x14994725, "kmem_cache_alloc_trace" },
	{ 0x6104427f, "kmalloc_caches" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xc5850110, "printk" },
	{ 0x37a0cba, "kfree" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

