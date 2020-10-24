int prune_host_list(unsigned logopt, struct host **list,
		    unsigned int vers, int port)
{
	struct host *this, *last, *first;
	struct host *new = NULL;
	unsigned int proximity, selected_version = 0;
	unsigned int v2_tcp_count, v3_tcp_count, v4_tcp_count;
	unsigned int v2_udp_count, v3_udp_count, v4_udp_count;
	unsigned int max_udp_count, max_tcp_count, max_count;
	int status;
	int kern_vers;

	if (!*list)
		return 0;

	/* Use closest hosts to choose NFS version */

	first = *list;

	/* Get proximity of first entry after local entries */
	this = first;
	while (this && this->proximity == PROXIMITY_LOCAL)
		this = this->next;
	first = this;

	/*
	 * Check for either a list containing only proximity local hosts
	 * or a single host entry whose proximity isn't local. If so
	 * return immediately as we don't want to add probe latency for
	 * the common case of a single filesystem mount request.
	 *
	 * But, if the kernel understands text nfs mount options then
	 * mount.nfs most likely bypasses its probing and lets the kernel
	 * do all the work. This can lead to long timeouts for hosts that
	 * are not available so check the kernel version and mount.nfs
	 * version and probe singleton mounts if the kernel version is
	 * greater than 2.6.22 and mount.nfs version is greater than 1.1.1.
	 * But also allow the MOUNT_WAIT configuration parameter to override
	 * the probing.
	 */
	/* we assume kernel and mount.nfs are recent enough */
	if (defaults_get_mount_wait() == -1) {
		if (!this)
			return 1;
	} else {
		if (!this || !this->next)
			return 1;
	}

	proximity = this->proximity;
	while (this) {
		struct host *next = this->next;

		if (this->proximity != proximity)
			break;

		if (this->name) {
			status = get_vers_and_cost(logopt, this, vers, port);
			if (!status) {
				if (this == first) {
					first = next;
					if (next)
						proximity = next->proximity;
				}
				delete_host(list, this);
			}
		}
		this = next;
	}

	/*
	 * The list of hosts that aren't proximity local may now
	 * be empty if we haven't been able probe any so we need
	 * to check again for a list containing only proximity
	 * local hosts.
	 */
	if (!first)
		return 1;

	last = this;

	/* Select NFS version of highest number of closest servers */

	v4_tcp_count = v3_tcp_count = v2_tcp_count = 0;
	v4_udp_count = v3_udp_count = v2_udp_count = 0;

	this = first;
	do {
		if (this->version & NFS4_TCP_SUPPORTED)
			v4_tcp_count++;

		if (this->version & NFS3_TCP_SUPPORTED)
			v3_tcp_count++;

		if (this->version & NFS2_TCP_SUPPORTED)
			v2_tcp_count++;

		if (this->version & NFS4_UDP_SUPPORTED)
			v4_udp_count++;

		if (this->version & NFS3_UDP_SUPPORTED)
			v3_udp_count++;

		if (this->version & NFS2_UDP_SUPPORTED)
			v2_udp_count++;

		this = this->next; 
	} while (this && this != last);

	max_tcp_count = mmax(v4_tcp_count, v3_tcp_count, v2_tcp_count);
	max_udp_count = mmax(v4_udp_count, v3_udp_count, v2_udp_count);
	max_count = max(max_tcp_count, max_udp_count);

	if (max_count == v4_tcp_count) {
		selected_version = NFS4_TCP_SUPPORTED;
		debug(logopt,
		      "selected subset of hosts that support NFS4 over TCP");
	} else if (max_count == v3_tcp_count) {
		selected_version = NFS3_TCP_SUPPORTED;
		debug(logopt,
		      "selected subset of hosts that support NFS3 over TCP");
	} else if (max_count == v2_tcp_count) {
		selected_version = NFS2_TCP_SUPPORTED;
		debug(logopt,
		      "selected subset of hosts that support NFS2 over TCP");
	} else if (max_count == v4_udp_count) {
		selected_version = NFS4_UDP_SUPPORTED;
		debug(logopt,
		      "selected subset of hosts that support NFS4 over UDP");
	} else if (max_count == v3_udp_count) {
		selected_version = NFS3_UDP_SUPPORTED;
		debug(logopt,
		      "selected subset of hosts that support NFS3 over UDP");
	} else if (max_count == v2_udp_count) {
		selected_version = NFS2_UDP_SUPPORTED;
		debug(logopt,
		      "selected subset of hosts that support NFS2 over UDP");
	}

	/* Add local and hosts with selected version to new list */
	this = *list;
	do {
		struct host *next = this->next;
		if (this->version & selected_version ||
		    this->proximity == PROXIMITY_LOCAL) {
			this->version = selected_version;
			remove_host(list, this);
			add_host(&new, this);
		}
		this = next;
	} while (this && this != last);

	/*
	 * Now go through rest of list and check for chosen version
	 * and add to new list if selected version is supported.
	 */ 

	first = last;
	this = first;
	while (this) {
		struct host *next = this->next;
		if (!this->name) {
			remove_host(list, this);
			add_host(&new, this);
		} else {
			status = get_supported_ver_and_cost(logopt, this,
						selected_version, port);
			if (status) {
				this->version = selected_version;
				remove_host(list, this);
				add_host(&new, this);
			}
		}
		this = next;
	}

	free_host_list(list);
	*list = new;

	return 1;
}