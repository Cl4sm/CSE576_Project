void
proto_copy_config(struct proto_config *dest, struct proto_config *src)
{
  node old_node;
  int old_class;
  char *old_name;

  if (dest->protocol != src->protocol)
    cf_error("Can't copy configuration from a different protocol type");

  if (dest->protocol->copy_config == NULL)
    cf_error("Inheriting configuration for %s is not supported", src->protocol->name);

  DBG("Copying configuration from %s to %s\n", src->name, dest->name);

  /* 
   * Copy struct proto_config here. Keep original node, class and name.
   * protocol-specific config copy is handled by protocol copy_config() hook
   */

  old_node = dest->n;
  old_class = dest->class;
  old_name = dest->name;

  memcpy(dest, src, sizeof(struct proto_config));

  dest->n = old_node;
  dest->class = old_class;
  dest->name = old_name;

  dest->protocol->copy_config(dest, src);
}
