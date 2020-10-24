static void read_stdin(char **string, int hidden)
{
	char *c = malloc(1025), *ret;

	if (!c) {
		fprintf(stderr, _("Allocation failure for string from stdin\n"));
		exit(1);
	}

	if (hidden) {
		disable_echo();
		ret = fgets(c, 1025, stdin);
		restore_echo();
		fprintf(stderr, "\n");
	} else
		ret = fgets(c, 1025, stdin);

	if (!ret) {
		perror(_("fgets (stdin)"));
		exit(1);
	}

	*string = c;

	c = strchr(*string, '\n');
	if (c)
		*c = 0;
}