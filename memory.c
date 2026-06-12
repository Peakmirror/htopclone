#include <stdio.h>
#include <stdlib.h>


 //read a line, check whether it starts with the label you want, pull the number out, store it in a variable
int memory_amount() {
    
    char line[256];
    long mem_total = 0;
    long mem_available = 0;

    FILE *f = fopen("/proc/meminfo", "r");
    if (f == NULL) {
        perror("Failed to open meminfo");
        return 1;
    }

    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "MemTotal: %ld", &mem_total) == 1) {
            printf("Total Memory: %ld kB\n", mem_total);
        }
        if (sscanf(line, "MemAvailable: %ld", &mem_available) == 1) {
            printf("Available Memory: %ld kB\n", mem_available);
        } 
    }   
        if(mem_total >0) {
        long used_memory = mem_total - mem_available;
        double used_memory_procentage = (used_memory * 100.0) / mem_total;
        printf("Used Memory: %ld kB\n", used_memory);
        printf("Used Memory Percentage: %.2f%%\n", used_memory_procentage);

        } else {
            printf("Error: Total memory is zero or negative.\n");
            fclose(f);
            return 1;
        }

        
        fclose(f);
        return 0;
    }




