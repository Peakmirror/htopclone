#include <stdio.h>
#include <unistd.h>
#include "cpu.h"
#include "memory.h"
#include "cores.h"

int main(void) {
    while (1) {
        printf("\033[H\033[J"); // clear the terminal screen

        cpu_usage();
        memory_amount();
        cores_usage();

        sleep(1);
    }
    return 0;
}