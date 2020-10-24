static int load_xml_conf_file(struct openconnect_info *vpninfo, char **ptr,
			      size_t *size)
{
	struct stat st;
	int fd;
	char *xmlfile = NULL;

	fd = open(vpninfo->xmlconfig, O_RDONLY);
	if (fd < 0) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to open XML config file: %s\n"),
			     strerror(errno));
		return 0;
	}

	if (fstat(fd, &st)) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to fstat() XML config file: %s\n"),
			     strerror(errno));
		goto err;
	}

	xmlfile = malloc(st.st_size);
	if (!xmlfile) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to allocate %lu bytes for XML config file\n"),
			     (unsigned long)st.st_size);
		goto err;
	}

	if (read(fd, xmlfile, st.st_size) != st.st_size) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to read XML config file: %s\n"),
			     strerror(errno));
		goto err;
	}

	*ptr = xmlfile;
	*size = st.st_size;

	close(fd);

	return 1;

err:
	close(fd);
	free(xmlfile);
	return -1;
}