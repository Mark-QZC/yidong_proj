#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

static int simple_runtime_suspend(struct device *dev)
{
    dev_info(dev, "Runtime suspend called.\n");
    return 0;
}

static int simple_runtime_resume(struct device *dev)
{
    dev_info(dev, "Runtime resume called.\n");
    return 0;
}

static const struct dev_pm_ops simple_pm_ops = {
    .runtime_suspend = simple_runtime_suspend,
    .runtime_resume = simple_runtime_resume,
};

static int simple_probe(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Simple DPM device probed.\n");

    pm_runtime_enable(&pdev->dev);
    pm_runtime_set_autosuspend_delay(&pdev->dev, 2000);
    pm_runtime_use_autosuspend(&pdev->dev);
    pm_runtime_set_active(&pdev->dev);
    pm_runtime_mark_last_busy(&pdev->dev);
    pm_runtime_put_autosuspend(&pdev->dev);

    return 0;
}

static void simple_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Simple DPM device removed.\n");
    pm_runtime_disable(&pdev->dev);
}

static struct platform_driver simple_driver = {
    .probe  = simple_probe,
    .remove = simple_remove,
    .driver = {
        .name = "simple_dpm",
        .pm   = &simple_pm_ops,
    },
};

module_platform_driver(simple_driver);
MODULE_LICENSE("GPL");