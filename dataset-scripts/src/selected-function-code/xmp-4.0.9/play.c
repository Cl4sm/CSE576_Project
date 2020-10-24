static void play(void *b, int i)
{
	int j = 0;

	/* block until we have enough free space in the buffer */
	while (buf_free() < i)
		usleep(100000);

	while (i) {
        	if ((j = write_buffer(b, i)) > 0) {
			i -= j;
			b += j;
		} else
			break;
	}

	if (paused) {
		AudioOutputUnitStart(au);
		paused = 0;
	}
}