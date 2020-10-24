bgp_create_update(struct bgp_conn *conn, byte *buf)
{
  struct bgp_proto *p = conn->bgp;
  struct bgp_bucket *buck;
  int remains = BGP_MAX_PACKET_LENGTH - BGP_HEADER_LENGTH - 4;
  byte *w;
  int wd_size = 0;
  int r_size = 0;
  int a_size = 0;

  w = buf+2;
  if ((buck = p->withdraw_bucket) && !EMPTY_LIST(buck->prefixes))
    {
      DBG("Withdrawn routes:\n");
      wd_size = bgp_encode_prefixes(p, w, buck, remains);
      w += wd_size;
      remains -= wd_size;
    }
  put_u16(buf, wd_size);

  if (remains >= 3072)
    {
      while ((buck = (struct bgp_bucket *) HEAD(p->bucket_queue))->send_node.next)
	{
	  if (EMPTY_LIST(buck->prefixes))
	    {
	      DBG("Deleting empty bucket %p\n", buck);
	      rem_node(&buck->send_node);
	      bgp_free_bucket(p, buck);
	      continue;
	    }

	  DBG("Processing bucket %p\n", buck);
	  a_size = bgp_encode_attrs(p, w+2, buck->eattrs, 2048);

	  if (a_size < 0)
	    {
	      log(L_ERR "%s: Attribute list too long, skipping corresponding routes", p->p.name);
	      bgp_flush_prefixes(p, buck);
	      rem_node(&buck->send_node);
	      bgp_free_bucket(p, buck);
	      continue;
	    }

	  put_u16(w, a_size);
	  w += a_size + 2;
	  r_size = bgp_encode_prefixes(p, w, buck, remains - a_size);
	  w += r_size;
	  break;
	}
    }
  if (!a_size)				/* Attributes not already encoded */
    {
      put_u16(w, 0);
      w += 2;
    }
  if (wd_size || r_size)
    {
      BGP_TRACE_RL(&rl_snd_update, D_PACKETS, "Sending UPDATE");
      return w;
    }
  else
    return NULL;
}
