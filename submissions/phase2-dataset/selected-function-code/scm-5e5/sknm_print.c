     SCM exp; SCM port; int writing;
{
  lputs("#<", port);
  switch (((struct sockaddr *)CDR(exp))->sa_family) {
  case AF_UNIX:
    lputs("unix-addr ", port);
    lputs(((struct sockaddr_un *)CDR(exp))->sun_path, port);
    break;
  case AF_INET:
    lputs("inet-addr ", port);
    lputs(inet_ntoa(((struct sockaddr_in *)CDR(exp))->sin_addr), port);
    lputc(':', port);
    scm_intprint(0L + ntohs(((struct sockaddr_in *)CDR(exp))->sin_port), 10, port);
    break;
  default: lputs(s_unkfam, port);
    lputc(' ', port);
    scm_intprint(0L+((struct sockaddr *)CDR(exp))->sa_family, 10, port);
  }
  lputc('>', port);
  return !0;
}
