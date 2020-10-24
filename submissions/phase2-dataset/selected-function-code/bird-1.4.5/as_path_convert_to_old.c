int
as_path_convert_to_old(struct adata *path, byte *dst, int *new_used)
{
  byte *src = path->data;
  byte *src_end = src + path->length;
  byte *dst_start = dst;
  u32 as;
  int i, n;
  *new_used = 0;

  while (src < src_end)
    {
      n = src[1];
      *dst++ = *src++;
      *dst++ = *src++;

      for(i=0; i<n; i++)
	{
	  as = get_u32(src);
	  if (as > 0xFFFF) 
	    {
	      as = AS_TRANS;
	      *new_used = 1;
	    }
	  put_u16(dst, as);
	  src += 4;
	  dst += 2;
	}
    }

  return dst - dst_start;
}
