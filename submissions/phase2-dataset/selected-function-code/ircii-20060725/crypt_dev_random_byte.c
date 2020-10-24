static int
crypt_dev_random_byte()
{
	static	int	devrndfd = -1;
	u_char	c;

	if (devrndfd == -1)
	{
		devrndfd = open(DEV_RANDOM_PATH, O_RDONLY);

		if (devrndfd == -1)
		{
			printf("--- HELP!!!!rypt_dev_random_byte: can not open %s: %s\n",
			    DEV_RANDOM_PATH, strerror(errno));
			printf("--- using random()\n");
			devrndfd = -2;
		}
	}
	if (devrndfd == -2)
		goto do_random_instead;

	if (read(devrndfd, &c, 1) != 1)
	{
		/* if we were just interrupted, don't bail on /dev/random */
		if (errno == EINTR)
		{
			printf("--- crypt_dev_random_byte: timeout, using random()\n");
			goto do_random_instead;
		}
		printf("--- HELP!  crypt_dev_random_byte: read of one byte on %s failed: %s\n",
		    DEV_RANDOM_PATH, strerror(errno));
		printf("--- using random()\n");
		devrndfd = -2;
		goto do_random_instead;
	}
	return ((int)c);

do_random_instead:
	return (random() & 255);
}
