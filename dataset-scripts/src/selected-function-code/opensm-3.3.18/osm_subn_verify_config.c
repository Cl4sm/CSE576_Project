int osm_subn_verify_config(IN osm_subn_opt_t * p_opts)
{
	if (p_opts->lmc > 7) {
		log_report(" Invalid Cached Option Value:lmc = %u:"
			   "Using Default:%u\n", p_opts->lmc, OSM_DEFAULT_LMC);
		p_opts->lmc = OSM_DEFAULT_LMC;
	}

	if (15 < p_opts->sm_priority) {
		log_report(" Invalid Cached Option Value:sm_priority = %u:"
			   "Using Default:%u\n",
			   p_opts->sm_priority, OSM_DEFAULT_SM_PRIORITY);
		p_opts->sm_priority = OSM_DEFAULT_SM_PRIORITY;
	}

	if ((15 < p_opts->force_link_speed) ||
	    (p_opts->force_link_speed > 7 && p_opts->force_link_speed < 15)) {
		log_report(" Invalid Cached Option Value:force_link_speed = %u:"
			   "Using Default:%u\n", p_opts->force_link_speed,
			   IB_PORT_LINK_SPEED_ENABLED_MASK);
		p_opts->force_link_speed = IB_PORT_LINK_SPEED_ENABLED_MASK;
	}

	if ((31 < p_opts->force_link_speed_ext) ||
	    (p_opts->force_link_speed_ext > 3 && p_opts->force_link_speed_ext < 30)) {
		log_report(" Invalid Cached Option Value:force_link_speed_ext = %u:"
			   "Using Default:%u\n", p_opts->force_link_speed_ext,
			   31);
		p_opts->force_link_speed_ext = 31;
	}

	if (2 < p_opts->fdr10) {
		log_report(" Invalid Cached Option Value:fdr10 = %u:"
			   "Using Default:%u\n", p_opts->fdr10, 1);
		p_opts->fdr10 = 1;
	}

	if (p_opts->max_wire_smps == 0)
		p_opts->max_wire_smps = 0x7FFFFFFF;
	else if (p_opts->max_wire_smps > 0x7FFFFFFF) {
		log_report(" Invalid Cached Option Value: max_wire_smps = %u,"
			   " Using Default: %u\n",
			   p_opts->max_wire_smps, OSM_DEFAULT_SMP_MAX_ON_WIRE);
		p_opts->max_wire_smps = OSM_DEFAULT_SMP_MAX_ON_WIRE;
	}

	if (p_opts->max_wire_smps2 > 0x7FFFFFFF) {
		log_report(" Invalid Cached Option Value: max_wire_smps2 = %u,"
			   " Using Default: %u",
			   p_opts->max_wire_smps2, p_opts->max_wire_smps);
		p_opts->max_wire_smps2 = p_opts->max_wire_smps;
	}

	if (strcmp(p_opts->console, OSM_DISABLE_CONSOLE)
	    && strcmp(p_opts->console, OSM_LOCAL_CONSOLE)
#ifdef ENABLE_OSM_CONSOLE_LOOPBACK
	    && strcmp(p_opts->console, OSM_LOOPBACK_CONSOLE)
#endif
#ifdef ENABLE_OSM_CONSOLE_SOCKET
	    && strcmp(p_opts->console, OSM_REMOTE_CONSOLE)
#endif
	    ) {
		log_report(" Invalid Cached Option Value:console = %s"
			   ", Using Default:%s\n",
			   p_opts->console, OSM_DEFAULT_CONSOLE);
		free(p_opts->console);
		p_opts->console = strdup(OSM_DEFAULT_CONSOLE);
	}

	if (p_opts->no_partition_enforcement == TRUE) {
		strcpy(p_opts->part_enforce, OSM_PARTITION_ENFORCE_OFF);
		p_opts->part_enforce_enum = OSM_PARTITION_ENFORCE_TYPE_OFF;
	} else {
		if (strcmp(p_opts->part_enforce, OSM_PARTITION_ENFORCE_BOTH) == 0)
			p_opts->part_enforce_enum = OSM_PARTITION_ENFORCE_TYPE_BOTH;
		else if (strcmp(p_opts->part_enforce, OSM_PARTITION_ENFORCE_IN) == 0)
			p_opts->part_enforce_enum = OSM_PARTITION_ENFORCE_TYPE_IN;
		else if (strcmp(p_opts->part_enforce, OSM_PARTITION_ENFORCE_OUT) == 0)
			p_opts->part_enforce_enum = OSM_PARTITION_ENFORCE_TYPE_OUT;
		else if (strcmp(p_opts->part_enforce, OSM_PARTITION_ENFORCE_OFF) == 0)
			p_opts->part_enforce_enum = OSM_PARTITION_ENFORCE_TYPE_OFF;
		else {
			log_report(" Invalid Cached Option Value:part_enforce = %s"
	                           ", Using Default:%s\n",
	                           p_opts->part_enforce, OSM_PARTITION_ENFORCE_BOTH);
			strcpy(p_opts->part_enforce, OSM_PARTITION_ENFORCE_BOTH);
			p_opts->part_enforce_enum = OSM_PARTITION_ENFORCE_TYPE_BOTH;
		}
	}

	if (p_opts->qos) {
		subn_verify_qos_set(&p_opts->qos_options, "qos");
		subn_verify_qos_set(&p_opts->qos_ca_options, "qos_ca");
		subn_verify_qos_set(&p_opts->qos_sw0_options, "qos_sw0");
		subn_verify_qos_set(&p_opts->qos_swe_options, "qos_swe");
		subn_verify_qos_set(&p_opts->qos_rtr_options, "qos_rtr");
	}

#ifdef ENABLE_OSM_PERF_MGR
	if (p_opts->perfmgr_sweep_time_s < 1) {
		log_report(" Invalid Cached Option Value:perfmgr_sweep_time_s "
			   "= %u Using Default:%u\n",
			   p_opts->perfmgr_sweep_time_s,
			   OSM_PERFMGR_DEFAULT_SWEEP_TIME_S);
		p_opts->perfmgr_sweep_time_s = OSM_PERFMGR_DEFAULT_SWEEP_TIME_S;
	}
	if (p_opts->perfmgr_max_outstanding_queries < 1) {
		log_report(" Invalid Cached Option Value:"
			   "perfmgr_max_outstanding_queries = %u"
			   " Using Default:%u\n",
			   p_opts->perfmgr_max_outstanding_queries,
			   OSM_PERFMGR_DEFAULT_MAX_OUTSTANDING_QUERIES);
		p_opts->perfmgr_max_outstanding_queries =
		    OSM_PERFMGR_DEFAULT_MAX_OUTSTANDING_QUERIES;
	}
#endif

	if (p_opts->m_key_protect_bits > 3) {
		log_report(" Invalid Cached Option Value:"
			   "m_key_protection_level = %u Setting to %u "
			   "instead\n", p_opts->m_key_protect_bits, 2);
		p_opts->m_key_protect_bits = 2;
	}
	if (p_opts->m_key_protect_bits && p_opts->m_key_lease_period) {
		if (!p_opts->sweep_interval) {
			log_report(" Sweep disabled with protected mkey "
				   "leases in effect; re-enabling sweeping "
				   "with interval %u\n",
				   cl_ntoh16(p_opts->m_key_lease_period) - 1);
			p_opts->sweep_interval =
				cl_ntoh16(p_opts->m_key_lease_period) - 1;
		}
		if (p_opts->sweep_interval >=
			cl_ntoh16(p_opts->m_key_lease_period)) {
			log_report(" Sweep interval %u >= mkey lease period "
				   "%u. Setting lease period to %u\n",
				   p_opts->sweep_interval,
				   cl_ntoh16(p_opts->m_key_lease_period),
				   p_opts->sweep_interval + 1);
			p_opts->m_key_lease_period =
				cl_hton16(p_opts->sweep_interval + 1);
		}
	}

	return 0;
}