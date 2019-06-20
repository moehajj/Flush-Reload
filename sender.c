#include "util.h"

/*
 * Sends a bit to the receiver by repeatedly flushing the addresses of the eviction_set
 * for the clock length of state->interval when we are sending a one, or by doing nothing
 * for the clock length of state->interval when we are sending a zero.
 */
void send_bit(bool one, struct state *state)
{
	CYCLES start_t = cc_sync();

	if (one) {
		ADDR_PTR addr = state->addr;
		while ((rdtscp() - start_t) < state->interval) {
			clflush(addr);
		}	

	} else {
		start_t = rdtscp();
		while (rdtscp() - start_t < state->interval) {}
	}
}

int main(int argc, char **argv)
{
    // Initialize state and local variables
    struct state state;
    init_state(&state, argc, argv);
    int sending = 1;

    printf("Please type a message (exit to stop).\n");
    while (sending) {

        // Get a message to send from the user
        printf("< ");
        char text_buf[128];
        fgets(text_buf, sizeof(text_buf), stdin);

        if (strcmp(text_buf, "exit\n") == 0) {
            sending = 0;
        }

        // Convert that message to binary
        char *msg = string_to_binary(text_buf);


        // Send a '10101011' byte to let the receiver detect that
        // I am about to send a start string and sync
        for (int i = 0; i < 6; i++) {
            send_bit(i % 2 == 0, &state);
        }
        send_bit(true, &state);
        send_bit(true, &state);

        // Send the message bit by bit
        size_t msg_len = strlen(msg);
        for (int ind = 0; ind < msg_len; ind++) {
            if (msg[ind] == '0') {
                send_bit(false, &state);
            } else {
                send_bit(true, &state);
            }
        }

    }

    printf("Sender finished\n");
    return 0;
}











