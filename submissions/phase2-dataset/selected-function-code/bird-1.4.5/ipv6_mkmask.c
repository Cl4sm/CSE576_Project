ip_addr
ipv6_mkmask(unsigned n)
{
  ip_addr a;
  int i;

  for(i=0; i<4; i++)
    {
      if (!n)
	a.addr[i] = 0;
      else if (n >= 32)
	{
	  a.addr[i] = ~0;
	  n -= 32;
	}
      else
	{
	  a.addr[i] = u32_mkmask(n);
	  n = 0;
	}
    }
  return a;
}
