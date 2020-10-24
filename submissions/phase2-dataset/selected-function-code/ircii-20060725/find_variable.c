find_variable(org_name, cnt)
	u_char	*org_name;
	int	*cnt;
{
	IrcVariable *v,
		    *first;
	size_t	len;
	int	var_index;
	u_char	*name = (u_char *) 0;

	malloc_strcpy(&name, org_name);
	upper(name);
	len = my_strlen(name);
	var_index = 0;
	for (first = irc_variable; first->name; first++, var_index++)

	{
		if (my_strncmp(name, first->name, len) == 0)
		{
			*cnt = 1;
			break;
		}
	}
	if (first->name)

	{
		if (my_strlen(first->name) != len)
		{
			v = first;
			for (v++; v->name; v++, (*cnt)++)

			{
				if (my_strncmp(name, v->name, len) != 0)
					break;
			}
		}
		new_free(&name);
		return (var_index);
	}
	else
	{
		*cnt = 0;
		new_free(&name);
		return (-1);
	}
}
