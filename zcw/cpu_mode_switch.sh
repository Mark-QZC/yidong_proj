#!/bin/bash

# 配置参数
CHECK_INTERVAL=5          # 模式切换间隔（秒）
FREQ_PRINT_DELAY=1        # 切换后等待多久打印频率（秒）
LOG_FILE="/var/log/cpu_mode_switch.log"  # 日志文件路径

# 初始化日志
log() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1" | sudo tee -a "$LOG_FILE"
}

# 检查依赖
check_dependencies() {
    if [[ ! -f /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor ]]; then
        log "错误：CPU调速器不可用！请检查是否加载了cpufreq驱动。"
        exit 1
    fi
}

# 获取所有CPU核心的当前频率（MHz）
get_cpu_freqs() {
    grep "MHz" /proc/cpuinfo | awk '{print $4}' | paste -sd ","
}

# 切换CPU模式并记录
switch_governor() {
    local target_governor=$1
    echo "$target_governor" | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor >/dev/null 2>&1
    
    # 验证是否设置成功
    local current_governor=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor)
    if [[ "$current_governor" != "$target_governor" ]]; then
        log "警告：未能成功切换为 $target_governor 模式（当前：$current_governor）"
    else
        log "已切换为: $target_governor 模式"
        sleep "$FREQ_PRINT_DELAY"
        local freqs=$(get_cpu_freqs)
        log "当前各核心频率(MHz): $freqs"
    fi
}

# 主循环
main() {
    check_dependencies
    log "开始CPU模式轮换测试..."
    
    while true; do
        current_governor=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor)
        
        if [[ "$current_governor" == "powersave" ]]; then
            switch_governor "performance"
	elif [[ "$current_governor" == "performance" ]]; then
    	    switch_governor "powersave"
	else switch_governor "powersave" # 默认切到 performance
        fi

        sleep "$CHECK_INTERVAL"
    done
}

main
