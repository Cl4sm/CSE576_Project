char	*
rfgets(lbuf, size, file)
	char	*lbuf;
	int	size;
	FILE	*file;
{
	char	*ptr;
	off_t	pos;

	if (fseek(file, -2L, 1))
		return NULL;
	do
	{
		switch (fgetc(file))
		{
		case EOF:
			return NULL;
		case '\n':
			pos = ftell(file);
			ptr = fgets(lbuf, size, file);
			fseek(file, (long)pos, 0);
			return ptr;
		}
	}
	while (fseek(file, -2L, 1) == 0);
	rewind(file);
	pos = 0L;
	ptr = fgets(lbuf, size, file);
	fseek(file, (long)pos, 0);
	return ptr;
}
