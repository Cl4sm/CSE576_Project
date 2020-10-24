bgp_rx(sock *sk, int size)
{
  struct bgp_conn *conn = sk->data;
  byte *pkt_start = sk->rbuf;
  byte *end = pkt_start + size;
  unsigned i, len;

  DBG("BGP: RX hook: Got %d bytes\n", size);
  while (end >= pkt_start + BGP_HEADER_LENGTH)
    {
      if ((conn->state == BS_CLOSE) || (conn->sk != sk))
	return 0;
      for(i=0; i<16; i++)
	if (pkt_start[i] != 0xff)
	  {
	    bgp_error(conn, 1, 1, NULL, 0);
	    break;
	  }
      len = get_u16(pkt_start+16);
      if (len < BGP_HEADER_LENGTH || len > BGP_MAX_PACKET_LENGTH)
	{
	  bgp_error(conn, 1, 2, pkt_start+16, 2);
	  break;
	}
      if (end < pkt_start + len)
	break;
      bgp_rx_packet(conn, pkt_start, len);
      pkt_start += len;
    }
  if (pkt_start != sk->rbuf)
    {
      memmove(sk->rbuf, pkt_start, end - pkt_start);
      sk->rpos = sk->rbuf + (end - pkt_start);
    }
  return 0;
}
