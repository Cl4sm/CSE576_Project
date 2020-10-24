get_from_history_file(which)
	int	which;
{
	u_char	*ptr;
	u_char	buffer[BIG_BUFFER_SIZE];

	if (last_dir == -1)
		last_dir = which;
	else if (last_dir != which)
	{
		last_dir = which;
		get_from_history(which);
	}
	fseek(hist_file, (long)file_pos, 0);
	if (which == NEXT)
	{
		if (!fgets(CP(buffer), sizeof buffer, hist_file))
		{
			file_pos = 0L;
			fseek(hist_file, 0L, 0);
			if (!fgets(CP(buffer), sizeof buffer, hist_file))
				return (u_char *) 0;
		}
	}
	else if (!rfgets(CP(buffer), sizeof buffer, hist_file))
	{
		fseek(hist_file, 0L, 2);
		file_pos = ftell(hist_file);
		if (!rfgets(CP(buffer), sizeof buffer, hist_file))
			return (u_char *) 0;
	}
	file_pos = ftell(hist_file);
	buffer[my_strlen(buffer) - 1] = '\0';
	parse_history(buffer, &ptr);
	return (ptr);
}
