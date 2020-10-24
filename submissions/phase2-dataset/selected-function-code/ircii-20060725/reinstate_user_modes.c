void
reinstate_user_modes()
{
	u_char	modes[10];
	u_char	*c;

	if (get_server_version(parsing_server_index) < Server2_7)
		return;
	c = modes;
	if (get_server_flag(parsing_server_index, USER_MODE_W))
		*c++ = 'w';
	if (get_server_flag(parsing_server_index, USER_MODE_S))
		*c++ = 's';
	if (get_server_flag(parsing_server_index, USER_MODE_I))
		*c++ = 'i';
	*c = '\0';
	if (c != modes)
		send_to_server("MODE %s +%s",
			get_server_nickname(parsing_server_index),
		modes);
}
