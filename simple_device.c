#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static struct platform_device *simple_device;

static int __init simple_device_init(void)
{
    simple_device = platform_device_register_simple("simple_dpm", -1, NULL, 0);
    if (IS_ERR(simple_device)) {
        pr_err("Failed to register platform device\n");
        return PTR_ERR(simple_device);
    }
    pr_info("Simple platform device registered.\n");
    return 0;
}

static void __exit simple_device_exit(void)
{
    platform_device_unregister(simple_device);
    pr_info("Simple platform device unregistered.\n");
}

module_init(simple_device_init);
module_exit(simple_device_exit);
MODULE_LICENSE("GPL");