static
bool parse_size(unsigned *tsz, unsigned *tflags, unsigned mask,
		const char *parse_sep)
{
	char *val, *nextchar;

	val = strtok(NULL, parse_sep);
	if (!val)
		return false;
	*tsz = strtoul(val, &nextchar, 0);
	if (*tsz) {
		if (*nextchar == 't' || *nextchar == 'T')
			*tflags &= ~mask;
		else if (*nextchar == 'm' || *nextchar == 'M')
			*tflags |= mask;
		/*
		 * A torus of radix two is also a mesh of radix two
		 * with multiple links between switches in that direction.
		 *
		 * Make it so always, otherwise the failure case routing
		 * logic gets confused.
		 */
		if (*tsz == 2)
			*tflags |= mask;
	}
	return true;
}