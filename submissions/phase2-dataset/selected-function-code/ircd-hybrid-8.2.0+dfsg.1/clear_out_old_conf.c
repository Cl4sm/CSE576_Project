static void
clear_out_old_conf(void)
{
  dlink_node *ptr = NULL, *next_ptr = NULL;
  dlink_list *free_items [] = {
    &server_items,   &oconf_items,
     &uconf_items,   &xconf_items,
     &nresv_items, &cluster_items,  &service_items, &cresv_items, NULL
  };

  dlink_list ** iterator = free_items; /* C is dumb */

  /* We only need to free anything allocated by yyparse() here.
   * Resetting structs, etc, is taken care of by set_default_conf().
   */

  for (; *iterator != NULL; iterator++)
  {
    DLINK_FOREACH_SAFE(ptr, next_ptr, (*iterator)->head)
    {
      struct MaskItem *conf = ptr->data;

      conf->active = 0;
      dlinkDelete(&conf->node, *iterator);

      /* XXX This is less than pretty */
      if (conf->type == CONF_SERVER || conf->type == CONF_OPER)
      {
        if (!conf->ref_count)
          conf_free(conf);
      }
      else if (conf->type == CONF_XLINE)
      {
        if (!conf->until)
          conf_free(conf);
      }
      else
        conf_free(conf);
    }
  }

  motd_clear();

  /*
   * don't delete the class table, rather mark all entries
   * for deletion. The table is cleaned up by class_delete_marked. - avalon
   */
  class_mark_for_deletion();

  clear_out_address_conf();

  /* clean out module paths */
  mod_clear_paths();

  pseudo_clear();

  /* clean out ConfigServerInfo */
  MyFree(ConfigServerInfo.description);
  ConfigServerInfo.description = NULL;
  MyFree(ConfigServerInfo.network_name);
  ConfigServerInfo.network_name = NULL;
  MyFree(ConfigServerInfo.network_desc);
  ConfigServerInfo.network_desc = NULL;
#ifdef HAVE_LIBCRYPTO
  if (ConfigServerInfo.rsa_private_key)
  {
    RSA_free(ConfigServerInfo.rsa_private_key);
    ConfigServerInfo.rsa_private_key = NULL;
  }

  MyFree(ConfigServerInfo.rsa_private_key_file);
  ConfigServerInfo.rsa_private_key_file = NULL;
#endif

  /* clean out ConfigAdminInfo */
  MyFree(ConfigAdminInfo.name);
  ConfigAdminInfo.name = NULL;
  MyFree(ConfigAdminInfo.email);
  ConfigAdminInfo.email = NULL;
  MyFree(ConfigAdminInfo.description);
  ConfigAdminInfo.description = NULL;

  /* clean out listeners */
  close_listeners();
}
