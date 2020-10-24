static int
krt_start(struct proto *P)
{
  struct krt_proto *p = (struct krt_proto *) P;

  add_tail(&krt_proto_list, &p->krt_node);

#ifdef KRT_ALLOW_LEARN
  krt_learn_init(p);
#endif

  krt_sys_start(p);

  krt_scan_timer_start(p);

  if (P->gr_recovery && KRT_CF->graceful_restart)
    P->gr_wait = 1;

  return PS_UP;
}
