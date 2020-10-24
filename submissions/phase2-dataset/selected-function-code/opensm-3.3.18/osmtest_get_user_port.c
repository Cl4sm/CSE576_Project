static uint32_t
osmtest_get_user_port(IN osmtest_t * const p_osmt,
		      IN const ib_port_attr_t p_attr_array[],
		      IN uint32_t const num_ports)
{
	uint32_t i, choice = 0;

	OSM_LOG_ENTER(&p_osmt->log);

	/*
	 * User needs prompting for the local port GUID with which
	 * to bind.
	 */

	while (1) {
		printf("\nChoose a local port number with which to bind:\n\n");
		for (i = 0; i < num_ports; i++) {
			/*
			 * Print the index + 1 since by convention, port numbers
			 * start with 1 on host channel adapters.
			 */

			printf("\t%u: GUID = 0x%8" PRIx64
			       ", lid = 0x%04X, state = %s\n", i + 1,
			       cl_ntoh64(p_attr_array[i].port_guid),
			       p_attr_array[i].lid,
			       ib_get_port_state_str(p_attr_array[i].
						     link_state));
		}

		printf("\nEnter choice (1-%u): ", i);
		fflush(stdout);
		if (scanf("%u", &choice) <= 0) {
			char junk[256];
			if (scanf("%s", junk) <= 0)
				printf("\nError: Cannot scan!\n");
		} else if (choice && choice <= num_ports)
			break;
		printf("\nError: Lame choice!\n");
	}
	printf("\n");
	OSM_LOG_EXIT(&p_osmt->log);
	return (choice - 1);
}