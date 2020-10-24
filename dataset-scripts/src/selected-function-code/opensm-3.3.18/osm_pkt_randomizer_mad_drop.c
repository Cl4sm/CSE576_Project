boolean_t
osm_pkt_randomizer_mad_drop(IN osm_log_t * p_log,
			    IN osm_pkt_randomizer_t * p_pkt_randomizer,
			    IN const ib_mad_t * p_mad)
{
	const ib_smp_t *p_smp;
	boolean_t res = FALSE;
	osm_dr_path_t dr_path;

	OSM_LOG_ENTER(p_log);

	p_smp = (ib_smp_t *) p_mad;

	if (p_smp->mgmt_class != IB_MCLASS_SUBN_DIR)
		/* This is a lid route mad. Don't drop it */
		goto Exit;

	osm_dr_path_init(&dr_path, p_smp->hop_count, p_smp->initial_path);

	if (__osm_pkt_randomizer_process_path
	    (p_log, p_pkt_randomizer, &dr_path)) {
		/* the mad should be dropped o */
		OSM_LOG(p_log, OSM_LOG_VERBOSE,
			"mad TID: 0x%" PRIx64 " is being dropped\n",
			cl_ntoh64(p_smp->trans_id));
		res = TRUE;
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return res;
}