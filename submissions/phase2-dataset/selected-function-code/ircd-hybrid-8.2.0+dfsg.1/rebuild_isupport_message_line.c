rebuild_isupport_message_line(void)
{
  char isupportbuffer[IRCD_BUFSIZE];
  char *p = isupportbuffer;
  dlink_node *ptr = NULL, *ptr_next = NULL;
  int n = 0;
  int tokens = 0;
  size_t len = 0;
  size_t reserve = strlen(me.name) + HOSTLEN + strlen(numeric_form(RPL_ISUPPORT));

  DLINK_FOREACH_SAFE(ptr, ptr_next, support_list_lines.head)
  {
    dlinkDelete(ptr, &support_list_lines);
    MyFree(ptr->data);
    free_dlink_node(ptr);
  }

  DLINK_FOREACH(ptr, support_list.head)
  {
    struct Isupport *support = ptr->data;

    p += (n = sprintf(p, "%s", support->name));
    len += n;

    if (support->options)
    {
      p += (n = sprintf(p, "=%s", support->options));
      len += n;
    }

    if (support->number > 0)
    {
      p += (n = sprintf(p, "=%d", support->number));
      len += n;
    }

    *p++ = ' ';
    len++;
    *p = '\0';

    if (++tokens == (MAXPARA-2) || len >= (sizeof(isupportbuffer)-reserve))
    { /* arbritrary for now */
      if (*--p == ' ')
        *p = '\0';

      dlinkAddTail(xstrdup(isupportbuffer), make_dlink_node(), &support_list_lines);
      p = isupportbuffer;
      len = 0;
      n = tokens = 0;
    }
  }

  if (len)
  {
    if (*--p == ' ')
      *p = '\0';
    dlinkAddTail(xstrdup(isupportbuffer), make_dlink_node(), &support_list_lines);
  }
}
