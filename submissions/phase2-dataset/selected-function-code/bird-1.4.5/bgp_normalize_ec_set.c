static inline void
bgp_normalize_ec_set(struct adata *ad, u32 *src, int internal)
{
  u32 *dst = int_set_get_data(ad);

  /* Remove non-transitive communities (EC_TBIT active) on external sessions */
  if (! internal)
    {
      int len = int_set_get_size(ad);
      u32 *t = dst;
      int i;

      for (i=0; i < len; i += 2)
	{
	  if (src[i] & EC_TBIT)
	    continue;
	  
	  *t++ = src[i];
	  *t++ = src[i+1];
	}

      ad->length = (t - dst) * 4;
    }
  else
    memcpy(dst, src, ad->length);

  qsort(dst, ad->length / 8, 8, (int(*)(const void *, const void *)) bgp_compare_ec);
}
