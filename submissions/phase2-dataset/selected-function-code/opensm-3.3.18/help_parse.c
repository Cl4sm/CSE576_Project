static void help_parse(char **p_last, osm_opensm_t * p_osm, FILE * out)
{
	char *p_cmd;
	int i, found = 0;

	p_cmd = next_token(p_last);
	if (!p_cmd)
		help_command(out, 0);
	else {
		for (i = 1; console_cmds[i].name; i++) {
			if (!strcmp(p_cmd, console_cmds[i].name)) {
				found = 1;
				console_cmds[i].help_function(out, 1);
				break;
			}
		}
		if (!found) {
			fprintf(out, "%s : Command not found\n\n", p_cmd);
			help_command(out, 0);
		}
	}
}