#!/bin/bash

# 配置参数
START_FREQ=1000000      # 起始频率（1000MHz，单位 kHz）
END_FREQ=1500000        # 结束频率（1500MHz，单位 kHz）
STEP=100000             # 每次增加的频率（100MHz，单位 kHz）
INTERVAL=5              # 频率变化间隔（秒）
LOG_FILE="/var/log/cpu_freq_scaling.log"  # 日志文件路径

# 初始化日志
log() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1" | sudo tee -a "$LOG_FILE"
}

# 检查依赖工具
check_dependencies() {
    if ! command -v cpufreq-set &> /dev/null; then
        log "错误：未安装 cpufreq-set，请先安装 cpufrequtils"
        sudo apt install cpufrequtils -y
    fi
}

# 设置所有核心频率
set_all_cores_frequency() {
    local freq=$1

    # 获取CPU核心数量
    local cores=$(nproc)

    for ((core=0; core<cores; core++)); do
        # 切换到 userspace 调速器
        echo userspace | sudo tee /sys/devices/system/cpu/cpu${core}/cpufreq/scaling_governor >/dev/null
        
        # 设置目标频率
        echo ${freq} | sudo tee /sys/devices/system/cpu/cpu${core}/cpufreq/scaling_setspeed >/dev/null

        # 验证当前频率
        local current_freq=$(cat /sys/devices/system/cpu/cpu${core}/cpufreq/scaling_cur_freq)
        log "核心 ${core} 频率已设置为: $((current_freq/1000)) MHz"
    done
}

# 主逻辑
main() {
    check_dependencies
    log "===== 所有CPU核心频率递增测试开始 ====="
    log "起始频率: $((START_FREQ/1000)) MHz"
    log "结束频率: $((END_FREQ/1000)) MHz"
    log "步长: $((STEP/1000)) MHz"
    log "间隔: ${INTERVAL} 秒"
    log "受影响核心数: $(nproc)"

    # 频率递增循环
    for ((freq=$START_FREQ; freq<=$END_FREQ; freq+=$STEP)); do
        set_all_cores_frequency $freq
        sleep $INTERVAL
    done

    log "===== 所有CPU核心频率递增测试完成 ====="
}

# 执行主函数
main
