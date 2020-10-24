init_commands(void)
{
  if (restricted)
    {
       submit_server_command("restrict");
       restricted = 0;
       return;
    }

  if (init_cmd)
    {
      /* First transition - client received hello from BIRD
	 and there is waiting initial command */
      submit_init_command(init_cmd);
      init_cmd = NULL;
      return;
    }

  if (once)
    {
      /* Initial command is finished and we want to exit */
      cleanup();
      exit(0);
    }

  input_init();

  term_lns = (term_lns > 0) ? term_lns : 25;
  term_cls = (term_cls > 0) ? term_cls : 80;

  init = 0;
}
