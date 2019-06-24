#include "util.h"

// Cache miss latency threshold
int CACHE_MISS_LATENCY = 100;

/*
 * Detects a bit by repeatedly measuring the access time of the load from config->addr
 * and counting the number of misses for the clock length of config->interval.
 *
 * Detect a bit 1 if misses >= hits
 * Detect a bit 0 otherwise
 */
bool detect_bit(struct config *config)
{
	int misses = 0;
	int hits = 0;

	// Sync with sender
	CYCLES start_t = cc_sync();
	while ((get_time() - start_t) < config->interval) {
		// Load data from config->addr and measure latency
		CYCLES access_time = measure_one_block_access_time(config->addr); 

		// Count if it's a miss or hit depending on latency
		if (access_time > CACHE_MISS_LATENCY) {
			misses++;
		} else {
			hits++;
		}
	}

	return misses >= hits;
}

int main(int argc, char **argv)
{
	// Initialize config and local variables
	struct config config;
	init_config(&config, argc, argv);
	char msg_ch[MAX_BUFFER_LEN + 1];

	uint32_t bitSequence = 0;
	uint32_t sequenceMask = ((uint32_t) 1<<6) - 1;
	uint32_t expSequence = 0b101011;
	
	printf("Listening...\n");
	fflush(stdout);
	while (1) {
		bool bitReceived = detect_bit(&config);

		// Detect the sequence '101011' that indicates sender is sending a message	
		bitSequence = ((uint32_t) bitSequence<<1) | bitReceived;
		if ((bitSequence & sequenceMask) == expSequence) {
			int binary_msg_len = 0;
			int strike_zeros = 0;
			for (int i = 0; i < MAX_BUFFER_LEN; i++) {
				binary_msg_len++;

				if (detect_bit(&config)) {
					msg_ch[i] = '1';
					strike_zeros = 0;
				} else {
					msg_ch[i] = '0';
					if (++strike_zeros >= 8 && i % 8 == 0) {
						break;
					}
				}
			}
			msg_ch[binary_msg_len - 8] = '\0';

			// Print out message
			int ascii_msg_len = binary_msg_len / 8;
			char msg[ascii_msg_len];
			printf("> %s\n", conv_char(msg_ch, ascii_msg_len, msg));
	
			// Terminate loop if received "exit" message
			if (strcmp(msg, "exit") == 0) {
				break;
			}
		}
	}

	printf("Receiver finished\n");
	return 0;
}


