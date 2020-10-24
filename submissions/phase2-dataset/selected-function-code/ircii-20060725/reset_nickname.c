reset_nickname(from)
	u_char	*from;
{
	u_char	server_num[10];

	if (already_doing_reset_nickname ||
	    is_server_connected(from_server) ||
	    !server_list[from_server].attempting_to_connect)
		return;

	say("You have specified an illegal nickname");
	if (!dumb && !get_int_var(NO_ASK_NICKNAME_VAR))
	{
		already_doing_reset_nickname = 1;
		say("Please enter your nickname");
		snprintf(CP(server_num), sizeof server_num, "%d", parsing_server_index);
		add_wait_prompt(UP("Nickname: "), nickname_sendline, server_num,
			WAIT_PROMPT_LINE);
	}
	update_all_status();
}
