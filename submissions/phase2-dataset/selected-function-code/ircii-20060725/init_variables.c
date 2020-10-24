init_variables()
{
	check_variable_order();

	set_string_var(CMDCHARS_VAR, UP(DEFAULT_CMDCHARS));
	set_string_var(LOGFILE_VAR, UP(DEFAULT_LOGFILE));
	set_string_var(SHELL_VAR, UP(DEFAULT_SHELL));
	set_string_var(SHELL_FLAGS_VAR, UP(DEFAULT_SHELL_FLAGS));
	set_string_var(DECRYPT_PROGRAM_VAR, UP(DEFAULT_DECRYPT_PROGRAM));
	set_string_var(DISPLAY_ENCODING_VAR, UP(DEFAULT_DISPLAY_ENCODING));
	set_string_var(ENCRYPT_PROGRAM_VAR, UP(DEFAULT_ENCRYPT_PROGRAM));
	set_string_var(CONTINUED_LINE_VAR, UP(DEFAULT_CONTINUED_LINE));
	set_string_var(INPUT_ENCODING_VAR, UP(DEFAULT_INPUT_ENCODING));
	set_string_var(INPUT_PROMPT_VAR, UP(DEFAULT_INPUT_PROMPT));
	set_input_prompt(get_string_var(INPUT_PROMPT_VAR));
	set_string_var(IRC_ENCODING_VAR, UP(DEFAULT_IRC_ENCODING));
	set_string_var(HIGHLIGHT_CHAR_VAR, UP(DEFAULT_HIGHLIGHT_CHAR));
	set_string_var(HISTORY_FILE_VAR, UP(DEFAULT_HISTORY_FILE));
	set_string_var(LASTLOG_LEVEL_VAR, UP(DEFAULT_LASTLOG_LEVEL));
	set_string_var(NOTIFY_HANDLER_VAR, UP(DEFAULT_NOTIFY_HANDLER));
	set_string_var(NOTIFY_LEVEL_VAR, UP(DEFAULT_NOTIFY_LEVEL));
	set_string_var(REALNAME_VAR, realname);
	set_string_var(STAR_PREFIX_VAR, UP(DEFAULT_STAR_PREFIX));
	set_string_var(STATUS_FORMAT_VAR, UP(DEFAULT_STATUS_FORMAT));
	set_string_var(STATUS_FORMAT1_VAR, UP(DEFAULT_STATUS_FORMAT1));
	set_string_var(STATUS_FORMAT2_VAR, UP(DEFAULT_STATUS_FORMAT2));
	set_string_var(STATUS_AWAY_VAR, UP(DEFAULT_STATUS_AWAY));
	set_string_var(STATUS_CHANNEL_VAR, UP(DEFAULT_STATUS_CHANNEL));
	set_string_var(STATUS_CHANOP_VAR, UP(DEFAULT_STATUS_CHANOP));
	set_string_var(STATUS_CLOCK_VAR, UP(DEFAULT_STATUS_CLOCK));
	set_string_var(STATUS_GROUP_VAR, UP(DEFAULT_STATUS_GROUP));
	set_string_var(STATUS_HOLD_VAR, UP(DEFAULT_STATUS_HOLD));
	set_string_var(STATUS_HOLD_LINES_VAR, UP(DEFAULT_STATUS_HOLD_LINES));
	set_string_var(STATUS_INSERT_VAR, UP(DEFAULT_STATUS_INSERT));
	set_string_var(STATUS_MAIL_VAR, UP(DEFAULT_STATUS_MAIL));
	set_string_var(STATUS_MODE_VAR, UP(DEFAULT_STATUS_MODE));
	set_string_var(STATUS_OPER_VAR, UP(DEFAULT_STATUS_OPER));
	set_string_var(STATUS_OVERWRITE_VAR, UP(DEFAULT_STATUS_OVERWRITE));
	set_string_var(STATUS_QUERY_VAR, UP(DEFAULT_STATUS_QUERY));
	set_string_var(STATUS_SCROLLED_VAR, UP(DEFAULT_STATUS_SCROLLED));
	set_string_var(STATUS_SCROLLED_LINES_VAR, UP(DEFAULT_STATUS_SCROLLED_LINES));
	set_string_var(STATUS_SERVER_VAR, UP(DEFAULT_STATUS_SERVER));
	set_string_var(STATUS_UMODE_VAR, UP(DEFAULT_STATUS_UMODE));
	set_string_var(STATUS_USER_VAR, UP(DEFAULT_STATUS_USER));
	set_string_var(STATUS_USER1_VAR, UP(DEFAULT_STATUS_USER1));
	set_string_var(STATUS_USER2_VAR, UP(DEFAULT_STATUS_USER2));
	set_string_var(STATUS_USER3_VAR, UP(DEFAULT_STATUS_USER3));
	set_string_var(STATUS_WINDOW_VAR, UP(DEFAULT_STATUS_WINDOW));
	set_string_var(USER_INFO_VAR, UP(DEFAULT_USERINFO));
	set_string_var(XTERM_GEOMOPTSTR_VAR, UP(DEFAULT_XTERM_GEOMOPTSTR));
	set_string_var(XTERM_OPTIONS_VAR, UP(DEFAULT_XTERM_OPTIONS));
	set_string_var(XTERM_PATH_VAR, UP(DEFAULT_XTERM_PATH));
	set_alarm(DEFAULT_CLOCK_ALARM);
	set_beep_on_msg(UP(DEFAULT_BEEP_ON_MSG));
	set_string_var(STATUS_NOTIFY_VAR, UP(DEFAULT_STATUS_NOTIFY));
	set_string_var(CLIENTINFO_VAR, UP(IRCII_COMMENT));
	set_string_var(IRCHOST_VAR, source_host);
	set_string_var(DCCHOST_VAR, dcc_source_host);
#ifdef HAVE_ICONV_OPEN
	set_irc_encoding(UP(irc_variable[IRC_ENCODING_VAR].string));
	set_input_encoding(UP(irc_variable[INPUT_ENCODING_VAR].string));
	set_display_encoding(UP(irc_variable[DISPLAY_ENCODING_VAR].string));
#endif /* HAVE_ICONV_OPEN */
	set_string_var(HELP_PATH_VAR, UP(DEFAULT_HELP_PATH));
	set_lastlog_size(irc_variable[LASTLOG_VAR].integer);
	set_history_size(irc_variable[HISTORY_VAR].integer);
	set_history_file(UP(irc_variable[HISTORY_FILE_VAR].string));
	set_highlight_char(UP(irc_variable[HIGHLIGHT_CHAR_VAR].string));
	set_lastlog_level(UP(irc_variable[LASTLOG_LEVEL_VAR].string));
	set_notify_level(UP(irc_variable[NOTIFY_LEVEL_VAR].string));
	if (get_int_var(LOG_VAR))
		set_int_var(LOG_VAR, 1);
}
