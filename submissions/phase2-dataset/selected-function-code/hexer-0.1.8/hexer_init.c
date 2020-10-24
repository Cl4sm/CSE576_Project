hexer_init()
  /* this function is called by `process_args()' (main.c) before executing
   * the commands given at the command line.
   */
{
  int i;
  char *hexerinit, *home;
  char path[1024];
  char line[1024];
  FILE *fp;

  he_pagerprg = getenv("PAGER");
  if (!he_pagerprg) he_pagerprg = HE_DEFAULT_PAGER;
  for (i = 0; hexer_options[i].option; ++i) {
    s_set_type(hexer_options[i].option, hexer_options[i].type);
    s_set_option(hexer_options[i].option, hexer_options[i].default_value, 1);
    s_set_action(hexer_options[i].option, hexer_options[i].action);
  }
  s_set_option("TERM", terminal_name, 1);

  he_map_special = 1;
  for (i = 0; exh_initialize[i]; ++i)
    exh_command(current_buffer->hedit, exh_initialize[i]);

  if (!(home = getenv("HOME"))) home = ".";
  if (!(hexerinit = getenv("HEXERRC"))) hexerinit = HEXERINIT_FILE;
  strcpy(path, home);
  strcat(path, "/");
  strcat(path, hexerinit);
  if ((fp = fopen(path, "r"))) {
    while (!feof(fp)) {
      if (fgets(line, 1024, fp) == NULL || !*line)
	break;
      line[strlen(line) - 1] = 0; /* discard the trailing newline */
      if (*line && *line != '"')
        exh_command(current_buffer->hedit, line);
      /* the command might have quit the editor, so we gotta check */
      if (!current_buffer) {
        fprintf(stderr,
                "warning: a command in your `%s' causes the editor to quit.\n",
                hexerinit);
        break;
      }
    }
    fclose(fp);
  }
}
