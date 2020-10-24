void
_mcd_plugin_loader_init (void)
{
  if (g_once_init_enter (&ready))
    {
#if ENABLE_AEGIS
      GObject *pseudo_plugin;
#endif
      const gchar *dir = g_getenv ("MC_FILTER_PLUGIN_DIR");

      if (dir == NULL)
        dir = MCD_PLUGIN_LOADER_DIR;

      mcp_read_dir (dir);

#if ENABLE_AEGIS
      /* The last object added by mcp_add_object() will be treated as highest
       * priority, at least for the interfaces used here */
      DEBUG ("Initialising built-in Aegis ACL plugin");
      pseudo_plugin = G_OBJECT (aegis_acl_new ());
      mcp_add_object (pseudo_plugin);
      g_object_unref (pseudo_plugin);
#endif

      g_once_init_leave (&ready, 1);
    }
}