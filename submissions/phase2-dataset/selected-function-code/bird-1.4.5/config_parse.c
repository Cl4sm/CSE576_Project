int
config_parse(struct config *c)
{
  DBG("Parsing configuration file `%s'\n", c->file_name);
  new_config = c;
  cfg_mem = c->mem;
  if (setjmp(conf_jmpbuf))
    return 0;
  cf_lex_init(0, c);
  sysdep_preconfig(c);
  protos_preconfig(c);
  rt_preconfig(c);
  roa_preconfig(c);
  cf_parse();
  protos_postconfig(c);
  if (EMPTY_LIST(c->protos))
    cf_error("No protocol is specified in the config file");
#ifdef IPV6
  if (!c->router_id)
    cf_error("Router ID must be configured manually on IPv6 routers");
#endif
  return 1;
}
