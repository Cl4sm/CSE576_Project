bgp_open(struct bgp_proto *p)
{
  struct config *cfg = p->cf->c.global;
  int errcode;

  if (!bgp_listen_sk)
    bgp_listen_sk = bgp_setup_listen_sk(cfg->listen_bgp_addr, cfg->listen_bgp_port, cfg->listen_bgp_flags);

  if (!bgp_listen_sk)
    {
      errcode = BEM_NO_SOCKET;
      goto err;
    }

  if (!bgp_linpool)
    bgp_linpool = lp_new(&root_pool, 4080);

  bgp_counter++;

  if (p->cf->password)
    if (sk_set_md5_auth(bgp_listen_sk, p->cf->remote_ip, p->cf->iface, p->cf->password) < 0)
      {
	sk_log_error(bgp_listen_sk, p->p.name);
	bgp_close(p, 0);
	errcode = BEM_INVALID_MD5;
	goto err;
      }

  return 0;

err:
  p->p.disabled = 1;
  bgp_store_error(p, NULL, BE_MISC, errcode);
  proto_notify_state(&p->p, PS_DOWN);
  return -1;
}
