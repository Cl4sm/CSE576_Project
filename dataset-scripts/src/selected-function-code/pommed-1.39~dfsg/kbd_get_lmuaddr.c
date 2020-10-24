int
kbd_get_lmuaddr(void)
{
  struct device_node *node;
  int plen;
  unsigned long *reg = NULL;

  of_init();

  node = of_find_node_by_type("lmu-controller", 0);
  if (node == NULL)
    {
      node = of_find_node_by_name("lmu-controller", 0);
      if (node == NULL)
	{
	  logmsg(LOG_ERR, "Error: no lmu-controller found in device-tree");

	  return -1;
	}
    }

  reg = of_find_property(node, "reg", &plen);
  lmu_info.lmuaddr = (unsigned int) (*reg >> 1);

  free(reg);
  of_free_node(node);

  logdebug("Found LMU controller at address 0x%x\n", lmu_info.lmuaddr);

  return 0;
}