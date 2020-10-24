unload_one_module(const char *name, int warn)
{
  struct module *modp = NULL;

  if ((modp = findmodule_byname(name)) == NULL)
    return -1;

  if (modp->modexit)
   modp->modexit();

  assert(dlink_list_length(&modules_list) > 0);
  dlinkDelete(&modp->node, &modules_list);
  MyFree(modp->name);

  lt_dlclose(modp->handle);

  if (warn == 1)
  {
    ilog(LOG_TYPE_IRCD, "Module %s unloaded", name);
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Module %s unloaded", name);
  }

  return 0;
}
