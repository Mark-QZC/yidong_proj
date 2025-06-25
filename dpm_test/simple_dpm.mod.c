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

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x1c\x00\x00\x00\xa1\x03\x21\xa4"
	"__pm_runtime_resume\0"
	"\x20\x00\x00\x00\x08\x8d\xd5\xb6"
	"__pm_runtime_disable\0\0\0\0"
	"\x1c\x00\x00\x00\xe2\xc0\xfb\x81"
	"pm_runtime_enable\0\0\0"
	"\x2c\x00\x00\x00\xc6\xd5\x5b\xc1"
	"pm_runtime_set_autosuspend_delay\0\0\0\0"
	"\x28\x00\x00\x00\x6b\x32\x57\x54"
	"__pm_runtime_use_autosuspend\0\0\0\0"
	"\x20\x00\x00\x00\x15\x25\xdf\x0a"
	"__pm_runtime_set_status\0"
	"\x20\x00\x00\x00\x6f\x36\xbe\x6e"
	"ktime_get_mono_fast_ns\0\0"
	"\x20\x00\x00\x00\x00\x81\x2a\xdd"
	"__pm_runtime_suspend\0\0\0\0"
	"\x24\x00\x00\x00\xe9\x55\x18\x03"
	"platform_driver_unregister\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x24\x00\x00\x00\x3e\x9c\x26\x9b"
	"__platform_driver_register\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x14\x00\x00\x00\x60\xf9\x84\x98"
	"_dev_info\0\0\0"
	"\x18\x00\x00\x00\x34\x61\x23\x68"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F322C68486B57729D666C4F");
