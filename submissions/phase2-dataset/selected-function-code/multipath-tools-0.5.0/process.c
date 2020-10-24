static void process(int fd)
{
	char *line;
	char *reply;

	cli_init();
	rl_readline_name = "multipathd";
	rl_completion_entry_function = key_generator;
	while ((line = readline("multipathd> "))) {
		size_t len;
		size_t llen = strlen(line);

		if (!llen) {
			free(line);
			continue;
		}

		if (need_quit(line, llen))
			break;

		if (send_packet(fd, line, llen + 1) != 0) break;
		if (recv_packet(fd, &reply, &len) != 0) break;

		print_reply(reply);

		if (line && *line)
			add_history(line);

		free(line);
		FREE(reply);
	}
}
