#!/bin/bash

set -e

echo "==> 清空内核日志..."
sudo dmesg -C

echo "==> 清理旧模块..."
sudo rmmod dpm_sim 2>/dev/null || true

echo "==> 编译模块..."
make

echo "==> 加载模块..."
sudo insmod dpm_sim.ko

echo "==> 等待 3 秒以触发 autosuspend..."
sleep 3

echo "==> 当前设备状态："
cat /sys/devices/platform/dpm_sim_dev/power/runtime_status

echo "==> 手动触发 resume（get）..."
echo get | sudo tee /sys/devices/platform/dpm_sim_dev/force_status

echo "==> 当前设备状态："
cat /sys/devices/platform/dpm_sim_dev/power/runtime_status

echo "==> 手动触发 suspend（put）..."
echo put | sudo tee /sys/devices/platform/dpm_sim_dev/force_status

sleep 3

echo "==> 当前设备状态："
cat /sys/devices/platform/dpm_sim_dev/power/runtime_status

echo "==> 查看内核日志："
sudo dmesg | tail -n 20

