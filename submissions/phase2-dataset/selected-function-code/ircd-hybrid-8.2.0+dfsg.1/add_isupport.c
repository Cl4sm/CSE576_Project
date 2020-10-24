void
add_isupport(const char *name, const char *options, int n)
{
  dlink_node *ptr;
  struct Isupport *support = NULL;

  DLINK_FOREACH(ptr, support_list.head)
  {
    support = ptr->data;
    if (irccmp(support->name, name) == 0)
    {
      MyFree(support->name);
      MyFree(support->options);
      break;
    }
  }

  if (ptr == NULL)
  {
    support = MyCalloc(sizeof(*support));
    dlinkAddTail(support, &support->node, &support_list);
  }

  support->name = xstrdup(name);
  if (options)
    support->options = xstrdup(options);
  support->number = n;

  rebuild_isupport_message_line();
}
