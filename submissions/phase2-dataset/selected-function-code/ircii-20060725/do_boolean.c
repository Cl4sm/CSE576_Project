int
do_boolean(str, value)
	u_char	*str;
	int	*value;
{
	upper(str);
	if (my_strcmp(str, var_settings[ON]) == 0)
		*value = 1;
	else if (my_strcmp(str, var_settings[OFF]) == 0)
		*value = 0;
	else if (my_strcmp(str, "TOGGLE") == 0)
	{
		if (*value)
			*value = 0;
		else
			*value = 1;
	}
	else
		return (1);
	return (0);
}
