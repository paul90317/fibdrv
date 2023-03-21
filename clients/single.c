#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"

#define FIB_DEV "/dev/fibonacci"

int main(int argc, char *argv[])
{
    int64_t term = argc > 1 ? atoll(argv[1]) : 0;
    int mode = argc > 3 ? atoi(argv[2]) : 0;
    uint8_t *buf = calloc(0.087 * term + 100, sizeof(char));
    char *sbuf = calloc(0.21 * term + 100, sizeof(char));
    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    lseek(fd, mode, SEEK_SET);
    ssize_t sz = read(fd, buf, term);
    clock_gettime(CLOCK_MONOTONIC, &end);
    int64_t utime =
        (end.tv_sec - start.tv_sec) * 1000000000 + end.tv_nsec - start.tv_nsec;
    int64_t ktime = write(fd, NULL, 0);
    printf("%ld %ld %ld %s\n", term, ktime, utime, bin_to_str(sbuf, buf, sz));

    close(fd);
    free(sbuf);
    free(buf);
    return 0;
}
