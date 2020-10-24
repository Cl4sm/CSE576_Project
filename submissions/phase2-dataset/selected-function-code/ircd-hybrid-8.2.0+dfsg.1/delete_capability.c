int
delete_capability(const char *capab_name)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  DLINK_FOREACH_SAFE(ptr, ptr_next, cap_list.head)
  {
    struct Capability *cap = ptr->data;

    if (cap->cap)
    {
      if (!irccmp(cap->name, capab_name))
      {
        default_server_capabs &= ~(cap->cap);
        dlinkDelete(ptr, &cap_list);
        MyFree(cap->name);
        MyFree(cap);
      }
    }
  }

  return 0;
}
