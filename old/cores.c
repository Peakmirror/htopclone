#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct CoreTimes {
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
    long steal;
} CoreTimes;

double calculate_core_usage(const CoreTimes *prev, const CoreTimes *curr) {
    long prev_total = prev->user + prev->nice + prev->system + prev->idle +
                      prev->iowait + prev->irq + prev->softirq + prev->steal;
    long curr_total = curr->user + curr->nice + curr->system + curr->idle +
                      curr->iowait + curr->irq + curr->softirq + curr->steal;

    long prev_idle = prev->idle + prev->iowait;
    long curr_idle = curr->idle + curr->iowait;

    long total_diff = curr_total - prev_total;
    long idle_diff = curr_idle - prev_idle;

    if (total_diff <= 0) {
        return 0.0;
    }

    long used_diff = total_diff - idle_diff;
    return used_diff * 100.0 / total_diff;
}

int read_core_snapshots(FILE *f, CoreTimes *snapshots, size_t core_count) {
    char line[256];
    int index;
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
    long steal;
    size_t found = 0;

    rewind(f);
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "cpu%d %ld %ld %ld %ld %ld %ld %ld %ld",
                   &index,
                   &user,
                   &nice,
                   &system,
                   &idle,
                   &iowait,
                   &irq,
                   &softirq,
                   &steal) == 9) {
            if (index >= 0 && (size_t)index < core_count) {
                snapshots[index].user = user;
                snapshots[index].nice = nice;
                snapshots[index].system = system;
                snapshots[index].idle = idle;
                snapshots[index].iowait = iowait;
                snapshots[index].irq = irq;
                snapshots[index].softirq = softirq;
                snapshots[index].steal = steal;
                found++;
            }
        }
    }

    return found == core_count ? 0 : -1;
}

int cores_usage(void) {
    long core_count_long = sysconf(_SC_NPROCESSORS_ONLN);
    size_t core_count;

    if (core_count_long <= 0) {
        fprintf(stderr, "Failed to detect CPU cores\n");
        return 1;
    }

    core_count = (size_t)core_count_long;

    CoreTimes *prev = calloc(core_count, sizeof(*prev));
    CoreTimes *curr = calloc(core_count, sizeof(*curr));
    FILE *f;

    if (prev == NULL || curr == NULL) {
        free(prev);
        free(curr);
        printf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    f = fopen("/proc/stat", "r");
    if (f == NULL) {
        printf(stderr, "Failed to open /proc/stat\n");
        free(prev);
        free(curr);
        return 1;
    }

    if (read_core_snapshots(f, prev, core_count) != 0) {
        printf(stderr, "Failed to read CPU snapshots\n");
        fclose(f);
        free(prev);
        free(curr);
        return 1;
    }

    sleep(1);

    if (read_core_snapshots(f, curr, core_count) != 0) {
        printf(stderr, "Failed to read CPU snapshots\n");
        fclose(f);
        free(prev);
        free(curr);
        return 1;
    }

    fclose(f);

    for (size_t i = 0; i < core_count; i++) {
        double usage = calculate_core_usage(&prev[i], &curr[i]);
        printf("Core %zu: %.2f%%\n", i, usage);
    }

    free(prev);
    free(curr);
    return 0;
}

