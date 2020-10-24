inline unsigned int
ea_hash(ea_list *e)
{
  u32 h = 0;
  int i;

  if (e)			/* Assuming chain of length 1 */
    {
      for(i=0; i<e->count; i++)
	{
	  struct eattr *a = &e->attrs[i];
	  h ^= a->id;
	  if (a->type & EAF_EMBEDDED)
	    h ^= a->u.data;
	  else
	    {
	      struct adata *d = a->u.ptr;
	      int size = d->length;
	      byte *z = d->data;
	      while (size >= 4)
		{
		  h ^= *(u32 *)z;
		  z += 4;
		  size -= 4;
		}
	      while (size--)
		h = (h >> 24) ^ (h << 8) ^ *z++;
	    }
	}
      h ^= h >> 16;
      h ^= h >> 6;
      h &= 0xffff;
    }
  return h;
}
