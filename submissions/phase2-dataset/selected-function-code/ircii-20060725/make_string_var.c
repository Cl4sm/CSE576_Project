u_char	*
make_string_var(var_name)
	u_char	*var_name;
{
	int	cnt,
		var_index;
	u_char	lbuf[BIG_BUFFER_SIZE],
		*ret = (u_char *) 0,
		*cmd = (u_char *) 0;

	malloc_strcpy(&cmd, var_name);
	upper(cmd);
	if (((var_index = find_variable(cmd, &cnt)) == -1) ||
	    (cnt > 1) ||
	    my_strcmp(cmd, irc_variable[var_index].name))
		goto out;
	switch (irc_variable[var_index].type)
	{
	case STR_TYPE_VAR:
		malloc_strcpy(&ret, irc_variable[var_index].string);
		break;
	case INT_TYPE_VAR:
		snprintf(CP(lbuf), sizeof lbuf, "%u", irc_variable[var_index].integer);
		malloc_strcpy(&ret, lbuf);
		break;
	case BOOL_TYPE_VAR:
		malloc_strcpy(&ret, UP(var_settings[irc_variable[var_index].integer]));
		break;
	case CHAR_TYPE_VAR:
		snprintf(CP(lbuf), sizeof lbuf, "%c", irc_variable[var_index].integer);
		malloc_strcpy(&ret, lbuf);
		break;
	}
out:
	new_free(&cmd);
	return (ret);

}
