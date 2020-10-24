int random_int()
{
	static int init = 0;
	unsigned int seed;
	int devrandom;
	unsigned char ch;

	if (init) return rand();

	seed = (unsigned int)time(NULL);

	devrandom = open("/dev/urandom", O_RDONLY);
	if(devrandom < 0)
		devrandom = open("/dev/random", O_RDONLY);

	if (devrandom >= 0) {
		readn(devrandom, &ch, 1);
		seed ^= ch;
		readn(devrandom, &ch, 1);
		seed ^= ch << 8;
		readn(devrandom, &ch, 1);
		seed ^= ch << 16;
		readn(devrandom, &ch, 1);
		seed ^= ch << 24;
		close(devrandom);
	}

	srand(seed);
	init = 1;

	return rand();
}