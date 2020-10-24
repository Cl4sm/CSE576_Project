void *
lp_alloc(linpool *m, unsigned size)
{
  byte *a = (byte *) BIRD_ALIGN((unsigned long) m->ptr, CPU_STRUCT_ALIGN);
  byte *e = a + size;

  if (e <= m->end)
    {
      m->ptr = e;
      return a;
    }
  else
    {
      struct lp_chunk *c;
      if (size >= m->threshold)
	{
	  /* Too large => allocate large chunk */
	  c = xmalloc(sizeof(struct lp_chunk) + size);
	  m->total_large += size;
	  c->next = m->first_large;
	  m->first_large = c;
	  c->size = size;
	}
      else
	{
	  if (m->current)
	    {
	      /* Still have free chunks from previous incarnation (before lp_flush()) */
	      c = m->current;
	      m->current = c->next;
	    }
	  else
	    {
	      /* Need to allocate a new chunk */
	      c = xmalloc(sizeof(struct lp_chunk) + m->chunk_size);
	      m->total += m->chunk_size;
	      *m->plast = c;
	      m->plast = &c->next;
	      c->next = NULL;
	      c->size = m->chunk_size;
	    }
	  m->ptr = c->data + size;
	  m->end = c->data + m->chunk_size;
	}
      return c->data;
    }
}
