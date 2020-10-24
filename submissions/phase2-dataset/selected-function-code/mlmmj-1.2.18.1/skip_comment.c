static char *
skip_comment(char *cp)
{
	int nesting = 1;

	for (; nesting > 0 && *cp; cp++) {
		switch (*cp) {
			case '\\':
				if (cp[1])
					cp++;
				break;
			case '(':
				nesting++;
				break;
			case ')':
				nesting--;
				break;
		}
	}
	return (cp);
}