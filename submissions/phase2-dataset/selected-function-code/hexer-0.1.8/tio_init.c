  int
tio_init(prog)
  char *prog;
  /* Initialize.  This function should be called before any other
   * `tio_*'-function.  `prog' should be the name of the application.
   * If you call `tio_init()' with `prog == 0', the program name is set
   * to `"??"' when called fir the first time or left unchanged if
   * `tio_init()' has been called before.
   */
{
  static int init_f = 0;
  static char stdout_buffer[STDOUT_BUFFER_SIZE];
  char *buf = t_string_buffer;
  char **area = &buf;
  int i;

  if (!init_f) {
    if (!(terminal_name = getenv("TERM"))) terminal_name = "dumb";
#if USE_SET
  } else {
    terminal_name = s_get_option_string("TERM");
#endif
  }

  t_line = -1;
  t_column = -1;

  /* Read the termcap entry for `terminal_name' to `t_bp'.
   */
  switch (tgetent(t_bp, terminal_name)) {
  case -1:
    error_msg("no termcap file found.\n");
    goto fail;
  case 0:
    error_msg("can't get termcap entry for `%s'.\n", terminal_name);
    goto fail;
  default:
    break;
  }

  /* Read string capabilities an check if we found all needed capabilies.
   */
  for (i = 0; t_strings[i].id; ++i)
    *(t_strings[i].string) = tgetstr(t_strings[i].id, area);
  if (!t_return) t_return = "\r";
  if (!t_key_backspace) t_key_backspace = "\b";
  if (!t_key_delete) t_key_delete = "\177";
    /* If the string capabilities "cr", "kD" or "kb" are not found in the
     * termcap entry, use ASCII-codes.
     */
  for (i = 0; *t_required[i].required; ++i) {
    char **p;
    int met;
    for (p = t_required[i].required, met = 0; *p; ++p)
      if (tio_cap(*p)) { met = 1; break; }
    if (!met) {
      if (!init_f) {
        char message[1024], *m = message;
        int j;
        int prefix = 1;
        /* We don't want to redisplay all termcap warnings every time the
         * window is resized...
         */
        for (j = 0; t_required[i].required[j]; ++j);
        for (++j; j < T_REQ_MAX_CAPS; ++j) t_required[i].required[j] = 0;
        if (*t_required[i].message == '\b')
          prefix = 0;
        else {
          sprintf(m, "%s ", TERMINAL_CANT);
          m += strlen(m);
        }
        sprintf(m, t_required[i].message + !prefix, t_required[i].required[0],
                t_required[i].required[1], t_required[i].required[2],
                t_required[i].required[3], t_required[i].required[4],
                t_required[i].required[5], t_required[i].required[6],
                t_required[i].required[7]);
        if (t_required[i].error) {
          error_msg("%s", message);
          goto fail;
        } else
          warning_msg("%s", message);
      }
    }
  }

  /* Try to figure out the size of the screen.
   */
  for (;;) {
    char *p;
    /* Get the screen size via `ioctl()'.
     */
#ifdef TIOCGWINSZ
    struct winsize size1;
#endif
#ifdef TIOCGSIZE
    struct ttysize size2;
    if (!ioctl(0, TIOCGSIZE, &size2)) {
      hx_lines = size2.ts_lines;
      hx_columns = size2.ts_cols;
      if (hx_lines > 0 && hx_columns > 0) break;
    }
#endif
#ifdef TIOCGWINSZ
    if (!ioctl(0, TIOCGWINSZ, &size1)) {
      hx_lines = size1.ws_row;
      hx_columns = size1.ws_col;
      if (hx_lines > 0 && hx_columns > 0) break;
    }
#endif
    /* `ioctl()' didn't work. See if we can get the screen size form the
     * the environment variables `LINES' ans `COLUMNS'.
     */
    if ((p = getenv("LINES"))) hx_lines = atoi(p);
    if ((p = getenv("COLUMNS"))) hx_columns = atoi(p);
    if (hx_lines > 0 && hx_columns > 0) break;
    /* Hmm... didn't work... we gotta believe what termcap tells us :-|
     */
    hx_lines = tgetnum("li");
    hx_columns = tgetnum("co");
    if (hx_lines > 0 && hx_columns > 0) break;
    /* Yuck!  Default the screen size to 80x24.
     */
    hx_lines = 24;
    hx_columns = 80;
    break;
  }

  if (!init_f) {
    /* The `tio_init()' function may be called more than once (i.e. the
     * `SIGWINCH'-handler calls `tio_init()').  The following stuff should
     * be done only when `tio_init()' is called for the first time.
     */
    init_f = 1;
    tcgetattr(0, &ts_start); /* Remember the terminal settings. */
    setvbuf(stdout, stdout_buffer, _IOFBF, STDOUT_BUFFER_SIZE);
      /* Turn on block buffering for `stdout'.
       */
    program_name = "??";
    tio_unread_count = 0;
    window_changed = 0;
    t_keypad_transmit = 0;
    t_application_mode = 0;
    if (t_kp_transmit_off) tio_command(t_kp_transmit_off, 1);
    t_timeout = 10; /* 1 second. */
  }
  if (prog) program_name = prog;

#ifdef SIGWINCH
  {
#ifdef SV_INTERRUPT
    static struct sigvec vec;
    vec.sv_handler = (void (*)(int))sigwinch_handler;
    vec.sv_mask = 0;
    vec.sv_flags = SV_INTERRUPT;
    sigvec(SIGWINCH, &vec, 0);
#else
#ifdef SA_INTERRUPT
    static struct sigaction act;
    act.sa_handler = sigwinch_handler;
    act.sa_mask = 0;
    act.sa_flags = SA_INTERRUPT;
    sigaction(SIGWINCH, &act, 0);
#else
    signal(SIGWINCH, sigwinch_handler);
    /* siginterrupt(SIGWINCH, 1); */
#endif
#endif
  }
#endif
  tio_echo(1);  /* Set echo on. */
  return 0;

fail:
  return -1;
}
