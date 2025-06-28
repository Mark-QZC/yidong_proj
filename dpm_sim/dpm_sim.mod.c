#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

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
	{ 0x90d65992, "__pm_runtime_disable" },
	{ 0xe561266b, "pm_runtime_enable" },
	{ 0xb26cd21d, "pm_runtime_set_autosuspend_delay" },
	{ 0xf1459675, "__pm_runtime_use_autosuspend" },
	{ 0x89e9880, "device_create_file" },
	{ 0xdc67eab7, "platform_device_unregister" },
	{ 0x2cd5d0e1, "platform_driver_unregister" },
	{ 0x122c3a7e, "_printk" },
	{ 0x9e07cb93, "__platform_driver_register" },
	{ 0x9cea2b24, "platform_device_register_full" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x128d9757, "_dev_info" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x222e7ce2, "sysfs_streq" },
	{ 0x15f8f87b, "__pm_runtime_resume" },
	{ 0xf4b13bb4, "__pm_runtime_idle" },
	{ 0xc8cd2ef5, "device_remove_file" },
	{ 0xb2b23fc2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "CFA86FDAD16D40154EDD7D2");
