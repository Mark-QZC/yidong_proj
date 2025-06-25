#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NETWORK_INTERFACE "ens33"
#define REFRESH_INTERVAL 1
#define SCORE_THRESHOLD 60.0

// 权重
#define CPU_WEIGHT 0.4
#define MEMORY_WEIGHT 0.3
#define NETWORK_WEIGHT 0.3

// 函数声明
float get_cpu_load();
float get_memory_usage();
float get_network_usage();
float calculate_composite_score(float cpu_load, float memory_usage, float network_usage);
void print_status(float cpu_load, float memory_usage, float network_usage, float composite_score);
int get_network_usage_percentage(const char *interface);

int main() {
    while (1) {
        // 获取 CPU、内存和网络负载数据
        float cpu_load = get_cpu_load();
        float memory_usage = get_memory_usage();
        float network_usage = get_network_usage();
        
        // 计算综合评分
        float composite_score = calculate_composite_score(cpu_load, memory_usage, network_usage);
        
        // 打印状态
        print_status(cpu_load, memory_usage, network_usage, composite_score);
        
        // 根据综合评分判断是否切换到硬转发
        if (composite_score >= SCORE_THRESHOLD) {
            printf("转发状态：硬转发\n");
            printf("\n");
        } else {
            printf("转发状态：软转发\n");
            printf("\n");
        }
        
        // 每 1 秒刷新一次
        sleep(REFRESH_INTERVAL);
    }
    
    return 0;
}

// 获取 CPU 使用率
float get_cpu_load() {
    FILE *fp;
    char buffer[128];
    long long int user1, nice1, system1, idle1, iowait1, irq1, softirq1;
    long long int total1, total2;
    long long int user2, nice2, system2, idle2, iowait2, irq2, softirq2;
    float load = 0.0;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Unable to read /proc/stat");
        return -1.0;
    }

    // 获取第一次数据
    fscanf(fp, "cpu %lld %lld %lld %lld %lld %lld %lld", 
            &user1, &nice1, &system1, &idle1, &iowait1, &irq1, &softirq1);
    fclose(fp);

    sleep(REFRESH_INTERVAL);

    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Unable to read /proc/stat");
        return -1.0;
    }

    // 获取第二次数据
    fscanf(fp, "cpu %lld %lld %lld %lld %lld %lld %lld", 
            &user2, &nice2, &system2, &idle2, &iowait2, &irq2, &softirq2);
    fclose(fp);

    // 计算负载变化
    total1 = user1 + nice1 + system1 + idle1 + iowait1 + irq1 + softirq1;
    total2 = user2 + nice2 + system2 + idle2 + iowait2 + irq2 + softirq2;

    // 计算 CPU 使用率
    load = 100.0 * (total2 - total1 - (idle2 - idle1)) / (total2 - total1);
    
    return load;
}

// 获取内存使用率
float get_memory_usage() {
    FILE *fp;
    char buffer[128];
    long total_memory = 0, free_memory = 0;

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Unable to read /proc/meminfo");
        return -1.0;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, "MemTotal:")) {
            sscanf(buffer, "MemTotal: %ld", &total_memory);
        } else if (strstr(buffer, "MemFree:")) {
            sscanf(buffer, "MemFree: %ld", &free_memory);
        }
    }

    fclose(fp);
    return ((float)(total_memory - free_memory) / total_memory) * 100.0;
}

// 获取网络使用率
float get_network_usage() {
    return get_network_usage_percentage(NETWORK_INTERFACE);
}

// 获取指定网络接口的使用率
int get_network_usage_percentage(const char *interface) {
    char command[128];
    char buffer[128];
    int received_bytes_before = 0, transmitted_bytes_before = 0;
    int received_bytes_after = 0, transmitted_bytes_after = 0;
    
    // 获取网络接口的接收和发送字节数
    sprintf(command, "cat /sys/class/net/%s/statistics/rx_bytes", interface);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Unable to read rx_bytes");
        return -1;
    }
    fscanf(fp, "%d", &received_bytes_before);
    fclose(fp);
    
    fp = popen("cat /sys/class/net/ens33/statistics/tx_bytes", "r");
    if (fp == NULL) {
        perror("Unable to read tx_bytes");
        return -1;
    }
    fscanf(fp, "%d", &transmitted_bytes_before);
    fclose(fp);

    // 等待 1 秒后获取新的数据
    sleep(REFRESH_INTERVAL);

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Unable to read rx_bytes");
        return -1;
    }
    fscanf(fp, "%d", &received_bytes_after);
    fclose(fp);

    fp = popen("cat /sys/class/net/ens33/statistics/tx_bytes", "r");
    if (fp == NULL) {
        perror("Unable to read tx_bytes");
        return -1;
    }
    fscanf(fp, "%d", &transmitted_bytes_after);
    fclose(fp);

    // 计算网络流量变化
    int received_diff = received_bytes_after - received_bytes_before;
    int transmitted_diff = transmitted_bytes_after - transmitted_bytes_before;

    // 总流量
    int total_diff = received_diff + transmitted_diff;

    // 获取网络带宽
    int total_bandwidth = 1000000000; // 假设带宽为 1 Gbps

    // 返回网络使用率百分比
    return (float)(total_diff * 8) / total_bandwidth * 100.0;
}

// 计算综合评分
float calculate_composite_score(float cpu_load, float memory_usage, float network_usage) {
    return cpu_load * CPU_WEIGHT + memory_usage * MEMORY_WEIGHT + network_usage * NETWORK_WEIGHT;
}

// 打印状态信息
void print_status(float cpu_load, float memory_usage, float network_usage, float composite_score) {
    printf("综合评分: %.2f\n", composite_score);
    printf("CPU负载: %.2f%%\n", cpu_load);
    printf("内存使用率: %.2f%%\n", memory_usage);
    printf("网络负载: %.2f%%\n", network_usage);
}
