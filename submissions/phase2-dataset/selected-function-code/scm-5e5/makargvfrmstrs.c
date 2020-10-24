     SCM args;
     const char *s_name;
{
  char ** argv;
  int argc = ilength(args);
  argv = (char **)must_malloc((1L+argc)*sizeof(char *), s_vector);
  for (argc = 0; NNULLP(args); args=CDR(args), ++argc) {
    ASRTER(NIMP(CAR(args)) && STRINGP(CAR(args)), CAR(args), ARG2, s_name);
    {
      sizet len = 1 + LENGTH(CAR(args));
      char *dst = (char *)must_malloc((long)len, s_string);
      char *src = CHARS(CAR(args));
      while (len--) dst[len] = src[len];
      argv[argc] = dst;
    }
  }
  argv[argc] = 0;
  return argv;
}
