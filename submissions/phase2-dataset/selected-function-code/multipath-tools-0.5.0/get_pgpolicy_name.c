get_pgpolicy_name (char * buff, int len, int id)
{
	char * s;

	switch (id) {
	case FAILOVER:
		s = "failover";
		break;
	case MULTIBUS:
		s = "multibus";
		break;
	case GROUP_BY_SERIAL:
		s = "group_by_serial";
		break;
	case GROUP_BY_PRIO:
		s = "group_by_prio";
		break;
	case GROUP_BY_NODE_NAME:
		s = "group_by_node_name";
		break;
	default:
		s = "undefined";
		break;
	}
	return snprintf(buff, POLICY_NAME_SIZE, "%s", s);
}
