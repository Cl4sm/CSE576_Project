static unsigned int
bgp_encode_prefixes(struct bgp_proto *p, byte *w, struct bgp_bucket *buck, unsigned int remains)
{
  byte *start = w;
  ip_addr a;
  int bytes;

  while (!EMPTY_LIST(buck->prefixes) && (remains >= (5+sizeof(ip_addr))))
    {
      struct bgp_prefix *px = SKIP_BACK(struct bgp_prefix, bucket_node, HEAD(buck->prefixes));
      DBG("\tDequeued route %I/%d\n", px->n.prefix, px->n.pxlen);

      if (p->add_path_tx)
	{
	  put_u32(w, px->path_id);
	  w += 4;
	  remains -= 4;
	}

      *w++ = px->n.pxlen;
      bytes = (px->n.pxlen + 7) / 8;
      a = px->n.prefix;
      ipa_hton(a);
      memcpy(w, &a, bytes);
      w += bytes;
      remains -= bytes + 1;
      rem_node(&px->bucket_node);
      bgp_free_prefix(p, px);
      // fib_delete(&p->prefix_fib, px);
    }
  return w - start;
}
