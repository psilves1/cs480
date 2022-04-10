#include "util.h"

// Cache miss latency threshold
int CACHE_MISS_LATENCY = 100;

bool detect_bit(struct config *config)
{
	int misses = 0;
	int hits = 0;

	// Sync with sender
	CYCLES start_t = cc_sync();
	while ((get_time() - start_t) < config->interval)
	{

		CYCLES access_time = measure_one_block_access_time(config->addr);

		if (access_time > CACHE_MISS_LATENCY)
		{
			misses++;
		}
		else
		{
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
	uint32_t sequenceMask = ((uint32_t)1 << 6) - 1; // 1 000 000 - 1 ----->>>  11 11 11 & (XXXXXXX 10 10 11) === 10 10 11
	uint32_t expSequence = 0b101011;

	printf("Listening...\n");
	fflush(stdout);
	while (1)
	{
		bool bitReceived = detect_bit(&config);

		// 1  0   101011		0101011 0 -> 0101011 1
		//  Detect the sequence '101011' that indicates sender is sending a message
		bitSequence = ((uint32_t)bitSequence << 1) | bitReceived;
		if ((bitSequence & sequenceMask) == expSequence)
		{
			int binary_msg_len = 0;
			int strike_zeros = 0;
			
			for (int i = 0; i < MAX_BUFFER_LEN; i++)
			{
				binary_msg_len++;

				if (detect_bit(&config))
				{
					msg_ch[i] = '1';
					strike_zeros = 0;
				}
				else
				{
					msg_ch[i] = '0';
					if (++strike_zeros >= 8 && i % 8 == 0)
					{ // checks for	0000 0000. AKA end of the message
						break;
					}
				}
			}
			

			/*
			msg_ch[binary_msg_l	en - 8] = '\0';  //removes the end 0000 0000 byte and adds '/0' to mark end of the string

			// Print out message
			int ascii_msg_len = binary_msg_len / 8; //converts bin len to ascii len
			char msg[ascii_msg_len]; //creates message buffer
			printf("> %s\n", conv_char(msg_ch, ascii_msg_len, msg)); //converts from bin to ascii and prints

			// Terminate loop if received "exit" message
			if (strcmp(msg, "exit") == 0) {
				break;
			}
			*/

			FILE *ptr = fopen("/home/psilves1/Desktop/cs480/Project/CovertChannel-FlushReload/senderFile.txt", "r");

			char *line;
			int len;

			char buf[4096];
			fgets(buf, 4096, ptr);
			// Convert that message to binary
			char *msg = string_to_binary(buf);
			int matches = 0;

			for (int i = 0; i < binary_msg_len; i++)
			{
				if (msg_ch[i] == msg[i])
				{
					matches++;
				}
			}

			float succRate = ((float)matches / binary_msg_len);

			printf("Error Rate: %f" , (1 - succRate));
			//printf("hits are : %d", matches);

			break;
		}
	}

	printf("\nReceiver finished\n");
	return 0;
}
