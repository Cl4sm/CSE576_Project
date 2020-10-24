static	void
oper(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*password;
	u_char	*nick;

	oper_command = 1;
	if (!(nick =  next_arg(args, &args)))
		nick =  nickname;
	if (!(password = next_arg(args, &args)))
	{
		add_wait_prompt(UP("Operator Password:"),
			oper_password_received, nick, WAIT_PROMPT_LINE);
		return;
	}
	send_to_server("OPER %s %s", nick, password);
}
