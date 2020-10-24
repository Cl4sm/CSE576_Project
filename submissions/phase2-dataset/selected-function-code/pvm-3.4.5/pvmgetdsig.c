int
pvmgetdsig()
{
	static int myfmt = -1;

	short i0;
	int i1;
	long i2;
	float f0;
	double f1;
	int i, j;
	int fmt;

	if (myfmt != -1)
		return myfmt;

	fmt = 0;

	i0 = 0;
	for (i = 0; i < sizeof(i0); i++)
		i0 += (short)i << (i * 8);
	fmt |= ibol(0, (char *) &i0, (int) sizeof(i0));

	i1 = 0;
	for (i = 0; i < sizeof(i1); i++)
		i1 += (int)i << (i * 8);
	fmt |= ibol(5, (char *) &i1, (int) sizeof(i1));

	i2 = 0;
	for (i = 0; i < sizeof(i2); i++)
		i2 += (long)i << (i * 8);
	fmt |= ibol(10, (char *) &i2, (int) sizeof(i2));

	f0 = 1.0;
	fmt |= fbol(15, (unsigned char *) &f0, (int) sizeof(f0));

	f1 = 1.0;
	fmt |= fbol(21, (unsigned char *) &f1, (int) sizeof(f1));

	myfmt = fmt;
	return fmt;
}
