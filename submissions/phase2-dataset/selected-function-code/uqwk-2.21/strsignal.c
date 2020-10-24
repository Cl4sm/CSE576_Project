{
  struct
  {
    int s;
    char *str;
  }
  sigs[]= {
    {SIGSEGV,"segmentation error"},
    {SIGILL,"illegal instruction"},
    {SIGABRT,"abort signal"},
    {SIGTERM,"termination signal"},
    {SIGINT,"interrupt"},
    {SIGKILL,"kill process"},
    {SIGHUP,"hangup signal"},
    {SIGQUIT,"quit signal"},
    {SIGBUS,"bus error"},
    {-99,"call for undefined signal message"}},*p;
  
  for(p=sigs;p->s!=-99;p++)
    if (p->s==sig)
      break;
  return p->str;
}
