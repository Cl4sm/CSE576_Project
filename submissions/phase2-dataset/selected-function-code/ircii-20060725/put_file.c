void
put_file(filename)
	u_char	*filename;
{
	FILE	*fp;
	char	line[1024];		/* too big?  too small?  who cares? */
	size_t	len;

	if ((fp = fopen(CP(filename), "r")) != (FILE *) 0)
	{
		while (fgets(line, 1024, fp))
		{
			if (line && *line)
			{
				if ((len = my_strlen(line)))
				{
					if (*(line + len - 1) == '\n')
						*(line + len - 1) = (u_char) 0;
				}
				put_it("%s", line);
			}
			else
				put_it(" ");
		}
		fclose(fp);
	}
}
