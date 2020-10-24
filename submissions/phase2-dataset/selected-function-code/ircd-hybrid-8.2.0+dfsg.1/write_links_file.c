void
write_links_file(void *unused)
{
  FILE *file = NULL;
  dlink_node *ptr = NULL, *ptr_next = NULL;
  char buff[IRCD_BUFSIZE] = "";

  if ((file = fopen(LIPATH, "w")) == NULL)
    return;

  DLINK_FOREACH_SAFE(ptr, ptr_next, flatten_links.head)
  {
    dlinkDelete(ptr, &flatten_links);
    MyFree(ptr->data);
    free_dlink_node(ptr);
  }

  DLINK_FOREACH(ptr, global_server_list.head)
  {
    const struct Client *target_p = ptr->data;

    /*
     * Skip hidden servers, aswell as ourselves, since we already send
     * ourselves in /links
     */
    if (IsHidden(target_p) || IsMe(target_p))
      continue;

    if (HasFlag(target_p, FLAGS_SERVICE) && ConfigServerHide.hide_services)
      continue;

    /*
     * Attempt to format the file in such a way it follows the usual links output
     * ie  "servername uplink :hops info"
     * Mostly for aesthetic reasons - makes it look pretty in mIRC ;)
     * - madmax
     */
    snprintf(buff, sizeof(buff), "%s %s :1 %s",   target_p->name,
             me.name, target_p->info);
    dlinkAddTail(xstrdup(buff), make_dlink_node(), &flatten_links);
    snprintf(buff, sizeof(buff), "%s %s :1 %s\n", target_p->name,
             me.name, target_p->info);

    fputs(buff, file);
  }

  fclose(file);
}
