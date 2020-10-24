randm(l)
	long	l;
{
	static	u_long	z = 0;
	long	t;

#ifndef __MSDOS__
	if (!z)
		z = (u_long) getuid();
#endif /* __MSDOS__ */
	if (!l)
	{
		t = RAND_A * (z % RAND_Q) - RAND_R * (z / RAND_Q);
		if (t > 0)
			z = t;
		else
			z = t + RAND_M;
		return (z >> 8) | ((z & 255) << 23);
	}
	else
	{
		if (l < 0)
#ifdef __MSDOS__
			z = 0;
#else
			z = (u_long) getuid();
#endif /* __MSDOS__ */
		else
			z = l;
		return 0;
	}
}
