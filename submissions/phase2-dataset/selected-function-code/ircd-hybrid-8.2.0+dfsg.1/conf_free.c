conf_free(struct MaskItem *conf)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;
  dlink_list *list = NULL;

  if ((list = map_to_list(conf->type)))
    dlinkFindDelete(list, conf);

  MyFree(conf->name);

  if (conf->dns_pending)
    delete_resolver_queries(conf);
  if (conf->passwd)
    memset(conf->passwd, 0, strlen(conf->passwd));
  if (conf->spasswd)
    memset(conf->spasswd, 0, strlen(conf->spasswd));

  conf->class = NULL;

  MyFree(conf->passwd);
  MyFree(conf->spasswd);
  MyFree(conf->reason);
  MyFree(conf->certfp);
  MyFree(conf->user);
  MyFree(conf->host);
#ifdef HAVE_LIBCRYPTO
  MyFree(conf->cipher_list);

  if (conf->rsa_public_key)
    RSA_free(conf->rsa_public_key);
#endif
  DLINK_FOREACH_SAFE(ptr, ptr_next, conf->hub_list.head)
  {
    MyFree(ptr->data);
    dlinkDelete(ptr, &conf->hub_list);
    free_dlink_node(ptr);
  }

  DLINK_FOREACH_SAFE(ptr, ptr_next, conf->leaf_list.head)
  {
    MyFree(ptr->data);
    dlinkDelete(ptr, &conf->leaf_list);
    free_dlink_node(ptr);
  }

  DLINK_FOREACH_SAFE(ptr, ptr_next, conf->exempt_list.head)
  {
    struct exempt *exptr = ptr->data;

    dlinkDelete(ptr, &conf->exempt_list);
    MyFree(exptr->name);
    MyFree(exptr->user);
    MyFree(exptr->host);
    MyFree(exptr);
  }

  MyFree(conf);
}
