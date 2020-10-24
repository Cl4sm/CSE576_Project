inputcmd(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*prompt;

	if (!args || !*args)
		return;
	
	if (*args++ != '"')
	{
		say("Need \" to begin prompt for INPUT");
		return;
	}

	prompt = args;
	if ((args = my_index(prompt, '"')) != NULL)
		*args++ = '\0';
	else
	{
		say("Missing \" in INPUT");
		return;
	}

	for (; *args == ' '; args++)
		;

	add_wait_prompt(prompt, eval_inputlist, args, WAIT_PROMPT_LINE);
}
