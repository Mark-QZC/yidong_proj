#!/bin/bash
echo "=== Runtime Power Management 状态 ==="
cat /sys/devices/platform/simple_dpm/power/runtime_status
cat /sys/devices/platform/simple_dpm/power/runtime_usage
cat /sys/devices/platform/simple_dpm/power/autosuspend_delay_ms