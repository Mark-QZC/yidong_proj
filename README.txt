DPM 驱动示例
============

使用说明：

1. 编译模块：
   make

2. 加载模块：
   sudo insmod simple_dpm.ko
   sudo insmod simple_device.ko

3. 启用自动挂起：
   echo auto | sudo tee /sys/devices/platform/simple_dpm/power/control

4. 查看日志：
   sudo dmesg -w | grep simple_dpm

5. 查看设备状态：
   ./check_pm.sh