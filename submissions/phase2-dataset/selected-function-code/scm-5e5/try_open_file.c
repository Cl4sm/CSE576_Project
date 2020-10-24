SCM try_open_file(filename, modes)
     SCM filename, modes;
{
  register SCM port;
  FILE *f;
  char cmodes[4];
  long flags;
  ASRTER(NIMP(filename) && STRINGP(filename), filename, ARG1, s_open_file);
  ASRTER(NIMP(modes) && (STRINGP(modes) || SYMBOLP(modes)), modes, ARG2, s_open_file);
  flags = mode_bits(CHARS(modes), cmodes);
  ASRTER(flags, modes, ARG2, s_open_file);
  if ((EXCLUSIVE & flags) && NIMP(*loc_try_create_file)) {
    port = apply(*loc_try_create_file, filename, cons(modes, listofnull));
    if (UNSPECIFIED != port) return port;
  }
  DEFER_INTS;
  SCM_OPENCALL((f = fopen(CHARS(filename), cmodes)));
  if (!f) {
    ALLOW_INTS;
    return BOOL_F;
  }
  port = scm_port_entry(f, tc16_fport, flags);
  if (BUF0 & flags) i_setbuf0(port);
  ALLOW_INTS;
  SCM_PORTDATA(port) = filename;
  return port;
}
