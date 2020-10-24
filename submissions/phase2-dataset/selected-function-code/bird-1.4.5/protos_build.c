protos_build(void)
{
  init_list(&protocol_list);
  init_list(&proto_list);
  init_list(&active_proto_list);
  init_list(&inactive_proto_list);
  init_list(&initial_proto_list);
  init_list(&flush_proto_list);
  proto_build(&proto_device);
#ifdef CONFIG_RADV
  proto_build(&proto_radv);
#endif
#ifdef CONFIG_RIP
  proto_build(&proto_rip);
#endif
#ifdef CONFIG_STATIC
  proto_build(&proto_static);
#endif
#ifdef CONFIG_OSPF
  proto_build(&proto_ospf);
#endif
#ifdef CONFIG_PIPE
  proto_build(&proto_pipe);
#endif
#ifdef CONFIG_BGP
  proto_build(&proto_bgp);
#endif
#ifdef CONFIG_BFD
  proto_build(&proto_bfd);
  bfd_init_all();
#endif

  proto_pool = rp_new(&root_pool, "Protocols");
  proto_flush_event = ev_new(proto_pool);
  proto_flush_event->hook = proto_flush_loop;
  proto_shutdown_timer = tm_new(proto_pool);
  proto_shutdown_timer->hook = proto_shutdown_loop;
}
