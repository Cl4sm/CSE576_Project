void
read_links_file(void)
{
  FILE *file = NULL;
  char *p = NULL;
  char buff[IRCD_BUFSIZE] = "";

  if ((file = fopen(LIPATH, "r")) == NULL)
    return;

  while (fgets(buff, sizeof(buff), file))
  {
    if ((p = strchr(buff, '\n')))
      *p = '\0';

    dlinkAddTail(xstrdup(buff), make_dlink_node(), &flatten_links);
  }

  fclose(file);
}
