void *
fib_get(struct fib *f, ip_addr *a, int len)
{
  unsigned int h = ipa_hash(*a);
  struct fib_node **ee = f->hash_table + (h >> f->hash_shift);
  struct fib_node *g, *e = *ee;
  u32 uid = h << 16;

  while (e && (e->pxlen != len || !ipa_equal(*a, e->prefix)))
    e = e->next;
  if (e)
    return e;
#ifdef DEBUGGING
  if (len < 0 || len > BITS_PER_IP_ADDRESS || !ip_is_prefix(*a,len))
    bug("fib_get() called for invalid address");
#endif

  while ((g = *ee) && g->uid < uid)
    ee = &g->next;
  while ((g = *ee) && g->uid == uid)
    {
      ee = &g->next;
      uid++;
    }

  if ((uid >> 16) != h)
    log(L_ERR "FIB hash table chains are too long");

  // log (L_WARN "FIB_GET %I %x %x", *a, h, uid);

  e = sl_alloc(f->fib_slab);
  e->prefix = *a;
  e->pxlen = len;
  e->next = *ee;
  e->uid = uid;
  *ee = e;
  e->readers = NULL;
  f->init(e);
  if (f->entries++ > f->entries_max)
    fib_rehash(f, HASH_HI_STEP);

  return e;
}
