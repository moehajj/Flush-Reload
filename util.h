
// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print 
// out chat messages
//
// You may use memory allocators and helper functions
// (e.g., rand()).  You may not use system().
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t
#define CYCLES uint32_t

#define CHANNEL_DEFAULT_INTERVAL        0x00008000
#define DEFAULT_FILE_OFFSET	0x0
#define DEFAULT_FILE_SIZE	4096
#define DEFAULT_CACHE_BLOCK_SIZE	64
#define CHANNEL_SYNC_TIMEMASK           0x000FFFFF
#define CHANNEL_SYNC_JITTER             0x0100
#define MAX_BUFFER_LEN	1024

struct state {
	ADDR_PTR addr;
	int interval;
};

CYCLES measure_one_block_access_time(ADDR_PTR addr);
CYCLES rdtscp(void);
CYCLES get_time();
CYCLES cc_sync();
 
void clflush(ADDR_PTR addr);

char *string_to_binary(char *s);

char *conv_char(char *data, int size, char *msg);

int init_state(struct state *state, int argc, char **argv);

#endif
