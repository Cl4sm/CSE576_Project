get_alua_info(int fd)
{
	int	rc;
	int	tpg;
	int	aas;

	rc = get_target_port_group_support(fd);
	if (rc < 0)
		return -ALUA_PRIO_TPGS_FAILED;

	if (rc == TPGS_NONE)
		return -ALUA_PRIO_NOT_SUPPORTED;

	tpg = get_target_port_group(fd);
	if (tpg < 0)
		return -ALUA_PRIO_RTPG_FAILED;

	condlog(3, "reported target port group is %i", tpg);
	rc = get_asymmetric_access_state(fd, tpg);
	if (rc < 0)
		return -ALUA_PRIO_GETAAS_FAILED;
	aas = (rc & 0x0f);

	condlog(3, "aas = %02x [%s]%s", rc, aas_print_string(rc),
		(rc & 0x80) ? " [preferred]" : "");
	return rc;
}
