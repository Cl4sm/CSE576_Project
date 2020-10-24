SCM reopen_file(filename, modes, port)
     SCM filename, modes, port;
{
  FILE *f;
  char cmodes[4];
  long flags;
  ASRTER(NIMP(filename) && STRINGP(filename), filename, ARG1, s_reopen_file);
  ASRTER(NIMP(modes) && (STRINGP(modes) || SYMBOLP(modes)), modes, ARG2, s_reopen_file);
  flags =  mode_bits(CHARS(modes), cmodes);
  ASRTER(flags, modes, ARG2, s_reopen_file);
  DEFER_INTS;
  ASRTER(NIMP(port) && FPORTP(port) && OPENP(port), port, ARG3, s_reopen_file);
  SCM_OPENCALL(f = freopen(CHARS(filename), cmodes, STREAM(port)));
  if (!f) {
    ALLOW_INTS;
    return BOOL_F;
  }
  else {
    SCM_PORTFLAGS(port) = flags;
    SCM_SETFLAGS(port, flags);
    if (BUF0 & flags)
      i_setbuf0(port);
    SCM_PORTDATA(port) = filename;
  }
  ALLOW_INTS;
  return port;
}
