void osm_subn_set_default_opt(IN osm_subn_opt_t * p_opt)
{
	memset(p_opt, 0, sizeof(osm_subn_opt_t));
	p_opt->guid = 0;
	p_opt->m_key = OSM_DEFAULT_M_KEY;
	p_opt->sm_key = OSM_DEFAULT_SM_KEY;
	p_opt->sa_key = OSM_DEFAULT_SA_KEY;
	p_opt->subnet_prefix = IB_DEFAULT_SUBNET_PREFIX;
	p_opt->m_key_lease_period = 0;
	p_opt->m_key_protect_bits = 0;
	p_opt->m_key_lookup = TRUE;
	p_opt->sweep_interval = OSM_DEFAULT_SWEEP_INTERVAL_SECS;
	p_opt->max_wire_smps = OSM_DEFAULT_SMP_MAX_ON_WIRE;
	p_opt->max_wire_smps2 = p_opt->max_wire_smps;
	p_opt->console = strdup(OSM_DEFAULT_CONSOLE);
	p_opt->console_port = OSM_DEFAULT_CONSOLE_PORT;
	p_opt->transaction_timeout = OSM_DEFAULT_TRANS_TIMEOUT_MILLISEC;
	p_opt->transaction_retries = OSM_DEFAULT_RETRY_COUNT;
	p_opt->max_smps_timeout = 1000 * p_opt->transaction_timeout *
				  p_opt->transaction_retries;
	/* by default we will consider waiting for 50x transaction timeout normal */
	p_opt->max_msg_fifo_timeout = 50 * OSM_DEFAULT_TRANS_TIMEOUT_MILLISEC;
	p_opt->sm_priority = OSM_DEFAULT_SM_PRIORITY;
	p_opt->lmc = OSM_DEFAULT_LMC;
	p_opt->lmc_esp0 = FALSE;
	p_opt->max_op_vls = OSM_DEFAULT_MAX_OP_VLS;
	p_opt->force_link_speed = 15;
	p_opt->force_link_speed_ext = 31;
	p_opt->fdr10 = 1;
	p_opt->reassign_lids = FALSE;
	p_opt->ignore_other_sm = FALSE;
	p_opt->single_thread = FALSE;
	p_opt->disable_multicast = FALSE;
	p_opt->force_log_flush = FALSE;
	p_opt->subnet_timeout = OSM_DEFAULT_SUBNET_TIMEOUT;
	p_opt->packet_life_time = OSM_DEFAULT_SWITCH_PACKET_LIFE;
	p_opt->vl_stall_count = OSM_DEFAULT_VL_STALL_COUNT;
	p_opt->leaf_vl_stall_count = OSM_DEFAULT_LEAF_VL_STALL_COUNT;
	p_opt->head_of_queue_lifetime = OSM_DEFAULT_HEAD_OF_QUEUE_LIFE;
	p_opt->leaf_head_of_queue_lifetime =
	    OSM_DEFAULT_LEAF_HEAD_OF_QUEUE_LIFE;
	p_opt->local_phy_errors_threshold = OSM_DEFAULT_ERROR_THRESHOLD;
	p_opt->overrun_errors_threshold = OSM_DEFAULT_ERROR_THRESHOLD;
	p_opt->use_mfttop = TRUE;
	p_opt->sminfo_polling_timeout =
	    OSM_SM_DEFAULT_POLLING_TIMEOUT_MILLISECS;
	p_opt->polling_retry_number = OSM_SM_DEFAULT_POLLING_RETRY_NUMBER;
	p_opt->force_heavy_sweep = FALSE;
	p_opt->log_flags = OSM_LOG_DEFAULT_LEVEL;
	p_opt->honor_guid2lid_file = FALSE;
	p_opt->daemon = FALSE;
	p_opt->sm_inactive = FALSE;
	p_opt->babbling_port_policy = FALSE;
	p_opt->drop_event_subscriptions = FALSE;
	p_opt->use_optimized_slvl = FALSE;
	p_opt->fsync_high_avail_files = TRUE;
#ifdef ENABLE_OSM_PERF_MGR
	p_opt->perfmgr = FALSE;
	p_opt->perfmgr_redir = TRUE;
	p_opt->perfmgr_sweep_time_s = OSM_PERFMGR_DEFAULT_SWEEP_TIME_S;
	p_opt->perfmgr_max_outstanding_queries =
	    OSM_PERFMGR_DEFAULT_MAX_OUTSTANDING_QUERIES;
	p_opt->perfmgr_ignore_cas = FALSE;
	p_opt->event_db_dump_file = NULL; /* use default */
	p_opt->perfmgr_rm_nodes = TRUE;
	p_opt->perfmgr_log_errors = TRUE;
	p_opt->perfmgr_query_cpi = TRUE;
	p_opt->perfmgr_xmit_wait_log = FALSE;
	p_opt->perfmgr_xmit_wait_threshold = OSM_PERFMGR_DEFAULT_XMIT_WAIT_THRESHOLD;
#endif				/* ENABLE_OSM_PERF_MGR */

	p_opt->event_plugin_name = NULL;
	p_opt->event_plugin_options = NULL;
	p_opt->node_name_map_name = NULL;

	p_opt->dump_files_dir = getenv("OSM_TMP_DIR");
	if (!p_opt->dump_files_dir || !(*p_opt->dump_files_dir))
		p_opt->dump_files_dir = strdup(OSM_DEFAULT_TMP_DIR);
	else
		p_opt->dump_files_dir = strdup(p_opt->dump_files_dir);
	p_opt->log_file = strdup(OSM_DEFAULT_LOG_FILE);
	p_opt->log_max_size = 0;
	p_opt->partition_config_file = strdup(OSM_DEFAULT_PARTITION_CONFIG_FILE);
	p_opt->no_partition_enforcement = FALSE;
	p_opt->part_enforce = strdup(OSM_PARTITION_ENFORCE_BOTH);
	p_opt->allow_both_pkeys = FALSE;
	p_opt->sm_assigned_guid = 0;
	p_opt->qos = FALSE;
	p_opt->qos_policy_file = strdup(OSM_DEFAULT_QOS_POLICY_FILE);
	p_opt->suppress_sl2vl_mad_status_errors = FALSE;
	p_opt->accum_log_file = TRUE;
	p_opt->port_prof_ignore_file = NULL;
	p_opt->hop_weights_file = NULL;
	p_opt->port_search_ordering_file = NULL;
	p_opt->port_profile_switch_nodes = FALSE;
	p_opt->sweep_on_trap = TRUE;
	p_opt->use_ucast_cache = FALSE;
	p_opt->routing_engine_names = NULL;
	p_opt->connect_roots = FALSE;
	p_opt->lid_matrix_dump_file = NULL;
	p_opt->lfts_file = NULL;
	p_opt->root_guid_file = NULL;
	p_opt->cn_guid_file = NULL;
	p_opt->io_guid_file = NULL;
	p_opt->port_shifting = FALSE;
	p_opt->scatter_ports = OSM_DEFAULT_SCATTER_PORTS;
	p_opt->max_reverse_hops = 0;
	p_opt->ids_guid_file = NULL;
	p_opt->guid_routing_order_file = NULL;
	p_opt->guid_routing_order_no_scatter = FALSE;
	p_opt->sa_db_file = NULL;
	p_opt->sa_db_dump = FALSE;
	p_opt->torus_conf_file = strdup(OSM_DEFAULT_TORUS_CONF_FILE);
	p_opt->do_mesh_analysis = FALSE;
	p_opt->exit_on_fatal = TRUE;
	p_opt->congestion_control = FALSE;
	p_opt->cc_key = OSM_DEFAULT_CC_KEY;
	p_opt->cc_max_outstanding_mads = OSM_CC_DEFAULT_MAX_OUTSTANDING_QUERIES;
	p_opt->enable_quirks = FALSE;
	p_opt->no_clients_rereg = FALSE;
	p_opt->prefix_routes_file = strdup(OSM_DEFAULT_PREFIX_ROUTES_FILE);
	p_opt->consolidate_ipv6_snm_req = FALSE;
	p_opt->lash_start_vl = 0;
	p_opt->sm_sl = OSM_DEFAULT_SL;
	p_opt->log_prefix = NULL;
	p_opt->per_module_logging_file = strdup(OSM_DEFAULT_PER_MOD_LOGGING_CONF_FILE);
	subn_init_qos_options(&p_opt->qos_options, NULL);
	subn_init_qos_options(&p_opt->qos_ca_options, NULL);
	subn_init_qos_options(&p_opt->qos_sw0_options, NULL);
	subn_init_qos_options(&p_opt->qos_swe_options, NULL);
	subn_init_qos_options(&p_opt->qos_rtr_options, NULL);
	p_opt->cc_cct.entries_len = 0;
	p_opt->cc_cct.input_str = NULL;
}