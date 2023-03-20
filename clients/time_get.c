#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"
#define N_UINT8 1000
int main()
{
    uint8_t buf[N_UINT8];
    int MAX_FIB = 1000; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (int i = 0; i <= MAX_FIB; ++i) {
        for (int j = 0; j < 50; ++j) {
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);
            read(fd, buf, i);
            clock_gettime(CLOCK_MONOTONIC, &end);
            int64_t utime = (end.tv_sec - start.tv_sec) * 1000000000 +
                            end.tv_nsec - start.tv_nsec;
            int64_t ktime = write(fd, NULL, 0);
            printf("%d %ld %ld\n", i, ktime, utime);
        }
    }

    close(fd);
    return 0;
}
