void
whois_lastcom(from, ArgList)
	u_char	*from;
	u_char	**ArgList;
{
	if (!ignore_whois_crap)
	{
		u_char	flag, *nick, *idle_str;
		int	idle;

		PasteArgs(ArgList, 2);
		if ((nick = ArgList[0]) && (idle_str = ArgList[1]) &&
				do_hook(current_numeric, "%s %s %s %s", from,
					nick, idle_str, ArgList[2]))
		{
			if ((idle = my_atoi(idle_str)) > 59)
			{
				idle = idle/60;
				flag = 1;
			}
			else
				flag = 0;
			put_it("%s %s has been idle %d %ss", numeric_banner(),
				nick, idle, flag? "minute": "second");
		}
	}
}
