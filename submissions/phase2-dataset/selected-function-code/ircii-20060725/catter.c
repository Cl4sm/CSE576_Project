catter(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char *target = next_arg(args, &args);

	if (target && args && *args)
	{
		u_char *text = args;
		FILE *fp = fopen(CP(target), "r+");

		if (!fp)
		{
			fp = fopen(CP(target), "w");
			if (!fp)
			{
				say("CAT: error: '%s': %s", target, strerror(errno));
				return;
		}	}
		
		fseek(fp, 0, SEEK_END);
		fprintf(fp, "%s\n", text),
		fclose(fp);
	}
	else
		say("Usage: /CAT <destfile> <line>");
}
