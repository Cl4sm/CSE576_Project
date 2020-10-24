static char *poly_print(int n, int *coeff)
{
	static char str[(MAX_DEGREE+1)*20];
	char *p = str;
	int i;
	int first = 1;
	int t;
	int sign;

	str[0] = 0;

	for (i = 0; i <= n; i++) {
		if (!coeff[i])
			continue;

		if (coeff[i] < 0) {
			sign = 1;
			t = -coeff[i];
		} else {
			sign = 0;
			t = coeff[i];
		}

		p += sprintf(p, "%s", sign? "-" : (first? "" : "+"));
		first = 0;

		if (t != 1 || i == 0)
			p += sprintf(p, "%d", t);

		if (i)
			p += sprintf(p, "x");
		if (i > 1)
			p += sprintf(p, "^%d", i);
	}

	return str;
}