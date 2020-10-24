static void parse_cmd_line(char *line, osm_opensm_t * p_osm)
{
	char *p_cmd, *p_last;
	int i, found = 0;
	FILE *out = p_osm->console.out;

	while (isspace(*line))
		line++;
	if (!*line)
		return;

	/* find first token which is the command */
	p_cmd = strtok_r(line, " \t\n\r", &p_last);
	if (p_cmd) {
		for (i = 0; console_cmds[i].name; i++) {
			if (loop_command.on) {
				if (!strcmp(p_cmd, "q")) {
					loop_command.on = 0;
				}
				found = 1;
				break;
			}
			if (!strcmp(p_cmd, console_cmds[i].name)) {
				found = 1;
				console_cmds[i].parse_function(&p_last, p_osm,
							       out);
				break;
			}
		}
		if (!found) {
			fprintf(out, "%s : Command not found\n\n", p_cmd);
			help_command(out, 0);
		}
	} else {
		fprintf(out, "Error parsing command line: `%s'\n", line);
	}
	if (loop_command.on) {
		fprintf(out, "use \"q<ret>\" to quit loop\n");
		fflush(out);
	}
}