static int
krt_shutdown(struct proto *P)
{
  struct krt_proto *p = (struct krt_proto *) P;

  krt_scan_timer_stop(p);

  /* FIXME we should flush routes even when persist during reconfiguration */
  if (p->initialized && !KRT_CF->persist)
    krt_flush_routes(p);

  p->ready = 0;
  p->initialized = 0;

  krt_sys_shutdown(p);

  rem_node(&p->krt_node);

  return PS_DOWN;
}
