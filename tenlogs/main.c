#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static uint8_t crypto_table[] = {
	0xC4, 0x5D, 0x6D, 0x61, 0x59, 0xB0, 0x1A, 0x14,
	0xB4, 0xE8, 0x2B, 0x54, 0x05, 0xB1, 0xE9, 0xB2,
	0xE6, 0x17, 0x34, 0x8B, 0x89, 0x47, 0xF2, 0x8E,
	0x2A, 0x28, 0x0D, 0xD9, 0x3C, 0x75, 0xA6, 0xAD,
	0xA5, 0x19, 0xA4, 0x6E, 0x82, 0x7C, 0x89, 0xD9,
	0xF7, 0x3E, 0x6F, 0x64, 0xC2, 0xEA, 0x8B, 0x3C,
	0xEF, 0x41, 0xE3, 0xE5, 0xD2, 0xFA, 0x4F, 0x62,
	0x0B, 0xB9, 0xB4, 0x4F, 0x55, 0x6D, 0x95, 0x6E
};

static void cryptmessage( char *input, char *output, size_t length );

int main( int argc, char **argv )
{
	char *buffer;
	int done;
	FILE *fs;
	long size;

	if (argc < 2)
	{
		printf("Usage: %s <logfile>\n", argv[0]);
		getchar();

		return 1;
	}

	fs = fopen(argv[1], "rb");
	fseek(fs, 0, SEEK_END);
	size = ftell(fs);
	fseek(fs, 0, SEEK_SET);

	buffer = (char*)malloc(size);
	fread(buffer, 1, size, fs);
	fclose(fs);

	fs = fopen(argv[1], "w");
	done = 0;
	while (done < size)
	{
		cryptmessage(buffer + done, buffer + done, size - done);
		fputs(buffer + done, fs);
		done += strlen(buffer + done) + 1;
	}
	fclose(fs);

	return 0;
}

void cryptmessage( char *input, char *output, size_t length )
{
	char *ptr;
	int i = 0;

	if (!input)
		return;

	if (!output)
		return;

	if (length == 0)
		return;

	while (i < length)
	{
		ptr = input + i;
		*ptr ^= crypto_table[i % 0x40];
		if (*ptr == 0)
			break;

		i++;
	}
}
