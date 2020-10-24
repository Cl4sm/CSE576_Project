void
delete_isupport(const char *name)
{
  dlink_node *ptr;
  struct Isupport *support;

  DLINK_FOREACH(ptr, support_list.head)
  {
    support = ptr->data;
    if (irccmp(support->name, name) == 0)
    {
      dlinkDelete(ptr, &support_list);
      MyFree(support->name);
      MyFree(support->options);
      MyFree(support);
      break;
    }
  }

  rebuild_isupport_message_line();
}
