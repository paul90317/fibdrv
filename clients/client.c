#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define FIB_DEV "/dev/fibonacci"
#define N_UINT8 32

int main()
{
    long long sz;

    uint8_t buf[N_UINT8];
    char s[N_UINT8 * 3 + 1];
    char write_buf[] = "testing writing";
    int offset = 100; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (int i = 0; i <= offset; i++) {
        sz = write(fd, write_buf, strlen(write_buf));
        printf("Writing to " FIB_DEV ", returned the sequence %lld\n", sz);
    }

    for (int i = 0; i <= offset; i++) {
        sz = read(fd, buf, i);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.\n",
               i, bin_to_str(s, buf, sz));
    }

    for (int i = offset; i >= 0; i--) {
        sz = read(fd, buf, i);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.\n",
               i, bin_to_str(s, buf, sz));
    }

    close(fd);
    return 0;
}
