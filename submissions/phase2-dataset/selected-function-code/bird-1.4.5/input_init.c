input_init(void)
{
  if (!interactive)
    return;

  if (tcgetattr(0, &stored_tty) < 0)
    die("tcgetattr: %m");

  if (signal(SIGINT, sig_handler) == SIG_IGN)
    signal(SIGINT, SIG_IGN);
  if (signal(SIGTERM, sig_handler) == SIG_IGN)
    signal(SIGTERM, SIG_IGN);

  struct winsize tws;
  if (ioctl(0, TIOCGWINSZ, &tws) == 0)
    {
      term_lns = tws.ws_row;
      term_cls = tws.ws_col;
    }
}
