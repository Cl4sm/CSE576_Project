static byte *
krt_buffer_update(struct proto *p, size_t *needed)
{
  size_t req = *needed;

  if ((req > krt_buflen) ||
      ((p == krt_buffer_owner) && (req < krt_bufmin)))
  {
    /* min buflen is 32 kB, step is 8 kB, or 128 kB if > 1 MB */
    size_t step = (req < 0x100000) ? 0x2000 : 0x20000;
    krt_buflen = (req < 0x6000) ? 0x8000 : (req + step);
    krt_bufmin = (req < 0x8000) ? 0 : (req - 2*step);

    if (krt_buffer) 
      mb_free(krt_buffer);
    krt_buffer = mb_alloc(krt_pool, krt_buflen);
    krt_buffer_owner = p;
  }

  *needed = krt_buflen;
  return krt_buffer;
}
