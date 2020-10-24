void
update_user_mode(modes)
	u_char	*modes;
{
	int	onoff = 1;

	while (*modes)
	{
		switch(*modes++)
		{
		case '-':
			onoff = 0;
			break;
		case '+':
			onoff = 1;
			break;
		case 'o':
		case 'O':
			set_server_operator(parsing_server_index, onoff);
			break;
		case 's':
		case 'S':
			set_server_flag(parsing_server_index, USER_MODE_S, onoff);
			break;
		case 'i':
		case 'I':
			set_server_flag(parsing_server_index, USER_MODE_I, onoff);
			break;
		case 'w':
		case 'W':
			set_server_flag(parsing_server_index, USER_MODE_W, onoff);
			break;
		case 'r':
			set_server_flag(parsing_server_index, USER_MODE_R, onoff);
			break;
		case 'a':
			set_server_flag(parsing_server_index, USER_MODE_A, onoff);
			break;
		case 'z':
			set_server_flag(parsing_server_index, USER_MODE_Z, onoff);
			break;
		}
	}
}
