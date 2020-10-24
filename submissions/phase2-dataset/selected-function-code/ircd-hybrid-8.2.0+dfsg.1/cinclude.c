cinclude(void)
{
  char *p = NULL;

  if ((p = strchr(yytext, '<')) == NULL)
    *strchr(p = strchr(yytext, '"') + 1, '"') = '\0';
  else
    *strchr(++p, '>') = '\0';

  /* log(L_NOTICE, "got include %s!", c); */

  /* do stacking and co. */
  if (include_stack_ptr >= MAX_INCLUDE_DEPTH)
    ilog(LOG_TYPE_IRCD, "Includes nested too deep in %s", p);
  else
  {
    FILE *tmp_fbfile_in = NULL;
    char filenamebuf[IRCD_BUFSIZE];

    if (*p == '/')  /* if it is an absolute path */
      snprintf(filenamebuf, sizeof(filenamebuf), "%s", p);
    else
      snprintf(filenamebuf, sizeof(filenamebuf), "%s/%s", ETCPATH, p);

    tmp_fbfile_in = fopen(filenamebuf, "r");

    if (tmp_fbfile_in == NULL)
    {
      ilog(LOG_TYPE_IRCD, "Unable to read configuration file '%s': %s",
           filenamebuf, strerror(errno));
      return;
    }

    lineno_stack[include_stack_ptr] = lineno;
    lineno = 1;
    inc_fbfile_in[include_stack_ptr] = conf_parser_ctx.conf_file;
    strlcpy(conffile_stack[include_stack_ptr], conffilebuf, IRCD_BUFSIZE);
    include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;
    conf_parser_ctx.conf_file = tmp_fbfile_in;
    snprintf(conffilebuf, sizeof(conffilebuf), "%s", filenamebuf);
    yy_switch_to_buffer(yy_create_buffer(yyin,YY_BUF_SIZE));
  }
}
