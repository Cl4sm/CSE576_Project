load_a_module(const char *path, int warn)
{
  lt_dlhandle tmpptr = NULL;
  const char *mod_basename = NULL;
  struct module *modp = NULL;

  if (findmodule_byname((mod_basename = libio_basename(path))))
    return 1;

  if (!(tmpptr = lt_dlopen(path)))
  {
    const char *err = ((err = lt_dlerror())) ? err : "<unknown>";

    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Error loading module %s: %s",
                         mod_basename, err);
    ilog(LOG_TYPE_IRCD, "Error loading module %s: %s", mod_basename, err);
    return -1;
  }

  if ((modp = lt_dlsym(tmpptr, "module_entry")) == NULL)
  {
    const char *err = ((err = lt_dlerror())) ? err : "<unknown>";

    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Error loading module %s: %s",
                         mod_basename, err);
    ilog(LOG_TYPE_IRCD, "Error loading module %s: %s", mod_basename, err);
    lt_dlclose(tmpptr);
    return -1;
  }

  modp->handle = tmpptr;

  if (EmptyString(modp->version))
    modp->version = unknown_ver;

  modp->name = xstrdup(mod_basename);
  dlinkAdd(modp, &modp->node, &modules_list);

  if (modp->modinit)
    modp->modinit();

  if (warn == 1)
  {
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Module %s [version: %s handle: %p] loaded.",
                         modp->name, modp->version, tmpptr);
    ilog(LOG_TYPE_IRCD, "Module %s [version: %s handle: %p] loaded.",
         modp->name, modp->version, tmpptr);
  }

  return 0;
}
