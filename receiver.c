#include "fr_util.h"

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

		// Ignore access times larger than 1000 cycles usually due to a disk miss.
		if (access_time < 1000) {
			// Count if it's a miss or hit depending on latency
			if (access_time > CACHE_MISS_LATENCY) {
				misses++;
			} else {
				hits++;
			}
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
	int flip_sequence = 4;
	bool current;
	bool previous = true;

	printf("Listening...\n");
	fflush(stdout);
	while (1) {
		current = detect_bit(&config);
	
		// Detect the sequence '101011' that indicates sender is sending a message	
		if (flip_sequence == 0 && current == 1 && previous == 1) {
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

		} else if (flip_sequence > 0 && current != previous) {
			flip_sequence--;
		} else if (current == previous) {
			flip_sequence = 4;
		}

		previous = current;
	}

	printf("Receiver finished\n");
	return 0;
}


