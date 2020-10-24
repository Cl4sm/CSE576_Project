boolean_t
__osm_pkt_randomizer_process_path(IN osm_log_t * p_log,
				  IN osm_pkt_randomizer_t * p_pkt_rand,
				  IN osm_dr_path_t * p_dr_path)
{
	boolean_t res = FALSE;
	static boolean_t rand_value_init = FALSE;
	static int rand_value;
	boolean_t in_fault_paths;
	uint8_t i;
	char buf[BUF_SIZE];
	char line[BUF_SIZE];

	OSM_LOG_ENTER(p_log);

	if (rand_value_init == FALSE) {
		int seed;
#ifdef __WIN__
		SYSTEMTIME st;
#else
		struct timeval tv;
		struct timezone tz;
#endif				/*  __WIN__ */

		/* initiate the rand_value according to timeofday */
		rand_value_init = TRUE;

#ifdef __WIN__
		GetLocalTime(&st);
		seed = st.wMilliseconds;
#else
		gettimeofday(&tv, &tz);
		seed = tv.tv_usec;
#endif				/*  __WIN__ */

		srand(seed);
	}

	/* If the hop_count is 1 - then this is a mad down to our local port - don't drop it */
	if (p_dr_path->hop_count <= 1)
		goto Exit;

	rand_value = rand();

	sprintf(buf, "Path: ");
	/* update the dr_path into the buf */
	for (i = 0; i <= p_dr_path->hop_count; i++) {
		sprintf(line, "[%X]", p_dr_path->path[i]);
		strcat(buf, line);
	}

	/* Check if the path given is in one of the fault paths */
	in_fault_paths =
	    __osm_pkt_randomizer_is_path_in_fault_paths(p_log, p_dr_path,
							p_pkt_rand);

	/* Check if all paths are initialized */
	if (p_pkt_rand->num_paths_initialized <
	    p_pkt_rand->osm_pkt_num_unstable_links) {
		/* Not all packets are initialized. */
		if (in_fault_paths == FALSE) {
			/* the path is not in the false paths. Check using the rand value
			   if to update it there or not. */
			if (rand_value %
			    (p_pkt_rand->osm_pkt_unstable_link_rate) == 0) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"%s added to the fault_dr_paths list\n"
					"\t\t\t rand_value:%u, unstable_link_rate:%u \n",
					buf, rand_value,
					p_pkt_rand->osm_pkt_unstable_link_rate);

				/* update the path in the fault paths */
				memcpy(&
				       (p_pkt_rand->
					fault_dr_paths[p_pkt_rand->
						       num_paths_initialized]),
				       p_dr_path, sizeof(osm_dr_path_t));
				p_pkt_rand->num_paths_initialized++;
				in_fault_paths = TRUE;
			}
		}
	}

	if (in_fault_paths == FALSE) {
		/* If in_fault_paths is FALSE - just ignore the path */
		OSM_LOG(p_log, OSM_LOG_VERBOSE, "%s not in fault paths\n", buf);
		goto Exit;
	}

	/* The path is in the fault paths. Need to choose (randomally if to drop it
	   or not. */
	rand_value = rand();

	if (rand_value % (p_pkt_rand->osm_pkt_drop_rate) == 0) {
		/* drop the current packet */
		res = TRUE;
		OSM_LOG(p_log, OSM_LOG_VERBOSE, "Dropping path:%s\n", buf);
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return res;
}