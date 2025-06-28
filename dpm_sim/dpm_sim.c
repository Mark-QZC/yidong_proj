#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/sysfs.h>
#include <linux/slab.h>

#define DEVICE_NAME "dpm_sim_dev"

// suspend/resume 实现
static int dpm_sim_runtime_suspend(struct device *dev)
{
    dev_info(dev, "Runtime suspend called\n");
    return 0;
}

static int dpm_sim_runtime_resume(struct device *dev)
{
    dev_info(dev, "Runtime resume called\n");
    return 0;
}

// PM 回调结构体
static const struct dev_pm_ops dpm_sim_pm_ops = {
    .runtime_suspend = dpm_sim_runtime_suspend,
    .runtime_resume  = dpm_sim_runtime_resume,
};

// sysfs 控制（手动触发 suspend/resume）
static ssize_t force_status_store(struct device *dev,
                                  struct device_attribute *attr,
                                  const char *buf, size_t count)
{
    if (sysfs_streq(buf, "get")) {
        pm_runtime_get(dev);
        dev_info(dev, "pm_runtime_get called manually\n");
    } else if (sysfs_streq(buf, "put")) {
        pm_runtime_put(dev);
        dev_info(dev, "pm_runtime_put called manually\n");
    } else {
        dev_info(dev, "Unknown command: %s\n", buf);
    }
    return count;
}
static DEVICE_ATTR_WO(force_status);

// probe 函数
static int dpm_sim_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    dev_info(dev, "DPM Simulated Device Probed\n");

    // 启用 runtime PM
    pm_runtime_enable(dev);
    pm_runtime_set_autosuspend_delay(dev, 2000);  // 自动挂起延迟 2 秒
    pm_runtime_use_autosuspend(dev);

    // 初始一次 get/put，触发 resume/suspend
    pm_runtime_get(dev);
    pm_runtime_put(dev);

    // 添加 sysfs 属性
    device_create_file(dev, &dev_attr_force_status);

    return 0;
}

// remove 函数
static int dpm_sim_remove(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    // 删除 sysfs 属性
    device_remove_file(dev, &dev_attr_force_status);

    pm_runtime_disable(dev);
    dev_info(dev, "DPM Simulated Device Removed\n");

    return 0;
}

static struct platform_driver dpm_sim_driver = {
    .driver = {
        .name = DEVICE_NAME,
        .owner = THIS_MODULE,
        .pm = &dpm_sim_pm_ops,
    },
    .probe = dpm_sim_probe,
    .remove = dpm_sim_remove,
};

static struct platform_device *dpm_sim_device;

static int __init dpm_sim_init(void)
{
    int ret;

    ret = platform_driver_register(&dpm_sim_driver);
    if (ret)
        return ret;

    dpm_sim_device = platform_device_register_simple(DEVICE_NAME, -1, NULL, 0);
    if (IS_ERR(dpm_sim_device)) {
        platform_driver_unregister(&dpm_sim_driver);
        return PTR_ERR(dpm_sim_device);
    }

    pr_info("DPM Simulated Module Loaded\n");
    return 0;
}

static void __exit dpm_sim_exit(void)
{
    platform_device_unregister(dpm_sim_device);
    platform_driver_unregister(&dpm_sim_driver);
    pr_info("DPM Simulated Module Unloaded\n");
}

module_init(dpm_sim_init);
module_exit(dpm_sim_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("老莹");
MODULE_DESCRIPTION("DPM 模拟测试模块 with sysfs");


