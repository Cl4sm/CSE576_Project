ib_api_status_t
osm_pkt_randomizer_init(IN OUT osm_pkt_randomizer_t ** pp_pkt_randomizer,
			IN osm_log_t * p_log)
{
	uint8_t tmp;
	ib_api_status_t res = IB_SUCCESS;

	OSM_LOG_ENTER(p_log);

	*pp_pkt_randomizer = malloc(sizeof(osm_pkt_randomizer_t));
	if (*pp_pkt_randomizer == NULL) {
		res = IB_INSUFFICIENT_MEMORY;
		goto Exit;
	}
	memset(*pp_pkt_randomizer, 0, sizeof(osm_pkt_randomizer_t));
	(*pp_pkt_randomizer)->num_paths_initialized = 0;

	tmp = atol(getenv("OSM_PKT_DROP_RATE"));
	(*pp_pkt_randomizer)->osm_pkt_drop_rate = tmp;

	if (getenv("OSM_PKT_NUM_UNSTABLE_LINKS") != NULL
	    && (tmp = atol(getenv("OSM_PKT_NUM_UNSTABLE_LINKS"))) > 0)
		(*pp_pkt_randomizer)->osm_pkt_num_unstable_links = tmp;
	else
		(*pp_pkt_randomizer)->osm_pkt_num_unstable_links = 1;

	if (getenv("OSM_PKT_UNSTABLE_LINK_RATE") != NULL
	    && (tmp = atol(getenv("OSM_PKT_UNSTABLE_LINK_RATE"))) > 0)
		(*pp_pkt_randomizer)->osm_pkt_unstable_link_rate = tmp;
	else
		(*pp_pkt_randomizer)->osm_pkt_unstable_link_rate = 20;

	OSM_LOG(p_log, OSM_LOG_VERBOSE, "Using OSM_PKT_DROP_RATE=%u \n"
		"\t\t\t\t OSM_PKT_NUM_UNSTABLE_LINKS=%u \n"
		"\t\t\t\t OSM_PKT_UNSTABLE_LINK_RATE=%u \n",
		(*pp_pkt_randomizer)->osm_pkt_drop_rate,
		(*pp_pkt_randomizer)->osm_pkt_num_unstable_links,
		(*pp_pkt_randomizer)->osm_pkt_unstable_link_rate);

	/* allocate the fault_dr_paths variable */
	/* It is the number of the paths that will be saved as fault = osm_pkt_num_unstable_links */
	(*pp_pkt_randomizer)->fault_dr_paths = malloc(sizeof(osm_dr_path_t) *
						      (*pp_pkt_randomizer)->
						      osm_pkt_num_unstable_links);
	if ((*pp_pkt_randomizer)->fault_dr_paths == NULL) {
		res = IB_INSUFFICIENT_MEMORY;
		goto Exit;
	}

	memset((*pp_pkt_randomizer)->fault_dr_paths, 0,
	       sizeof(osm_dr_path_t) *
	       (*pp_pkt_randomizer)->osm_pkt_num_unstable_links);

Exit:
	OSM_LOG_EXIT(p_log);
	return (res);
}