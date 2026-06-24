#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct CpuTimes {
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
    long steal;
    
} CpuTimes;

CpuTimes curr;
CpuTimes prev;

double cpu_calculation(CpuTimes prev, CpuTimes curr) {
    long prev_total = prev.user + prev.nice + prev.system + prev.idle +
                      prev.iowait + prev.irq + prev.softirq + prev.steal;

    long curr_total = curr.user + curr.nice + curr.system + curr.idle +
                      curr.iowait + curr.irq + curr.softirq + curr.steal;

    long prev_idle = prev.idle + prev.iowait;
    long curr_idle = curr.idle + curr.iowait;

    long total_diff = curr_total - prev_total;
    long idle_diff = curr_idle - prev_idle;

    long used_diff = total_diff - idle_diff;
    double usage = used_diff * 100.0 / total_diff;

    return usage;
}

int cpu_usage() {
    FILE *f;
    char line[256];

    f = fopen("/proc/stat", "r");
    if (f == NULL) {
        printf("Failed to open /proc/stat\n");
        return 1;
    }

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "cpu %ld %ld %ld %ld %ld %ld %ld %ld",
                   &prev.user, &prev.nice, &prev.system, &prev.idle,
                   &prev.iowait, &prev.irq, &prev.softirq, &prev.steal) == 8) {
            sleep(1);
            break;
        }
    }

    rewind(f);

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "cpu %ld %ld %ld %ld %ld %ld %ld %ld",
                   &curr.user, &curr.nice, &curr.system, &curr.idle,
                   &curr.iowait, &curr.irq, &curr.softirq, &curr.steal) == 8) {
            break;
        }
    }

    fclose(f);

    printf("CPU Usage: %.2f%%\n", cpu_calculation(prev, curr));
    return 0;
}


