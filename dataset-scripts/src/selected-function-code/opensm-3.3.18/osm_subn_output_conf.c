int osm_subn_output_conf(FILE *out, IN osm_subn_opt_t * p_opts)
{
	int cacongoutputcount = 0;
	int i;

	fprintf(out,
		"#\n# DEVICE ATTRIBUTES OPTIONS\n#\n"
		"# The port GUID on which the OpenSM is running\n"
		"guid 0x%016" PRIx64 "\n\n"
		"# M_Key value sent to all ports qualifying all Set(PortInfo)\n"
		"m_key 0x%016" PRIx64 "\n\n"
		"# The lease period used for the M_Key on this subnet in [sec]\n"
		"m_key_lease_period %u\n\n"
		"# The protection level used for the M_Key on this subnet\n"
		"m_key_protection_level %u\n\n"
		"# If FALSE, SM won't try to determine the m_key of unknown ports\n"
		"m_key_lookup %s\n\n"
		"# SM_Key value of the SM used for SM authentication\n"
		"sm_key 0x%016" PRIx64 "\n\n"
		"# SM_Key value to qualify rcv SA queries as 'trusted'\n"
		"sa_key 0x%016" PRIx64 "\n\n"
		"# Note that for both values above (sm_key and sa_key)\n"
		"# OpenSM version 3.2.1 and below used the default value '1'\n"
		"# in a host byte order, it is fixed now but you may need to\n"
		"# change the values to interoperate with old OpenSM running\n"
		"# on a little endian machine.\n\n"
		"# Subnet prefix used on this subnet\n"
		"subnet_prefix 0x%016" PRIx64 "\n\n"
		"# The LMC value used on this subnet\n"
		"lmc %u\n\n"
		"# lmc_esp0 determines whether LMC value used on subnet is used for\n"
		"# enhanced switch port 0. If TRUE, LMC value for subnet is used for\n"
		"# ESP0. Otherwise, LMC value for ESP0s is 0.\n"
		"lmc_esp0 %s\n\n"
		"# sm_sl determines SMSL used for SM/SA communication\n"
		"sm_sl %u\n\n"
		"# The code of maximal time a packet can live in a switch\n"
		"# The actual time is 4.096usec * 2^<packet_life_time>\n"
		"# The value 0x14 disables this mechanism\n"
		"packet_life_time 0x%02x\n\n"
		"# The number of sequential packets dropped that cause the port\n"
		"# to enter the VLStalled state. The result of setting this value to\n"
		"# zero is undefined.\n"
		"vl_stall_count 0x%02x\n\n"
		"# The number of sequential packets dropped that cause the port\n"
		"# to enter the VLStalled state. This value is for switch ports\n"
		"# driving a CA or router port. The result of setting this value\n"
		"# to zero is undefined.\n"
		"leaf_vl_stall_count 0x%02x\n\n"
		"# The code of maximal time a packet can wait at the head of\n"
		"# transmission queue.\n"
		"# The actual time is 4.096usec * 2^<head_of_queue_lifetime>\n"
		"# The value 0x14 disables this mechanism\n"
		"head_of_queue_lifetime 0x%02x\n\n"
		"# The maximal time a packet can wait at the head of queue on\n"
		"# switch port connected to a CA or router port\n"
		"leaf_head_of_queue_lifetime 0x%02x\n\n"
		"# Limit the maximal operational VLs\n"
		"max_op_vls %u\n\n"
		"# Force PortInfo:LinkSpeedEnabled on switch ports\n"
		"# If 0, don't modify PortInfo:LinkSpeedEnabled on switch port\n"
		"# Otherwise, use value for PortInfo:LinkSpeedEnabled on switch port\n"
		"# Values are (IB Spec 1.2.1, 14.2.5.6 Table 146 \"PortInfo\")\n"
		"#    1: 2.5 Gbps\n"
		"#    3: 2.5 or 5.0 Gbps\n"
		"#    5: 2.5 or 10.0 Gbps\n"
		"#    7: 2.5 or 5.0 or 10.0 Gbps\n"
		"#    2,4,6,8-14 Reserved\n"
		"#    Default 15: set to PortInfo:LinkSpeedSupported\n"
		"force_link_speed %u\n\n"
		"# Force PortInfo:LinkSpeedExtEnabled on ports\n"
		"# If 0, don't modify PortInfo:LinkSpeedExtEnabled on port\n"
		"# Otherwise, use value for PortInfo:LinkSpeedExtEnabled on port\n"
		"# Values are (MgtWG RefID #4722)\n"
		"#    1: 14.0625 Gbps\n"
		"#    2: 25.78125 Gbps\n"
		"#    3: 14.0625 Gbps or 25.78125 Gbps\n"
		"#    30: Disable extended link speeds\n"
		"#    Default 31: set to PortInfo:LinkSpeedExtSupported\n"
		"force_link_speed_ext %u\n\n"
		"# FDR10 on ports on devices that support FDR10\n"
		"# Values are:\n"
		"#    0: don't use fdr10 (no MLNX ExtendedPortInfo MADs)\n"
		"#    Default 1: enable fdr10 when supported\n"
		"#    2: disable fdr10 when supported\n"
		"fdr10 %u\n\n"
		"# The subnet_timeout code that will be set for all the ports\n"
		"# The actual timeout is 4.096usec * 2^<subnet_timeout>\n"
		"subnet_timeout %u\n\n"
		"# Threshold of local phy errors for sending Trap 129\n"
		"local_phy_errors_threshold 0x%02x\n\n"
		"# Threshold of credit overrun errors for sending Trap 130\n"
		"overrun_errors_threshold 0x%02x\n\n"
		"# Use SwitchInfo:MulticastFDBTop if advertised in PortInfo:CapabilityMask\n"
		"use_mfttop %s\n\n",
		cl_ntoh64(p_opts->guid),
		cl_ntoh64(p_opts->m_key),
		cl_ntoh16(p_opts->m_key_lease_period),
		p_opts->m_key_protect_bits,
		p_opts->m_key_lookup ? "TRUE" : "FALSE",
		cl_ntoh64(p_opts->sm_key),
		cl_ntoh64(p_opts->sa_key),
		cl_ntoh64(p_opts->subnet_prefix),
		p_opts->lmc,
		p_opts->lmc_esp0 ? "TRUE" : "FALSE",
		p_opts->sm_sl,
		p_opts->packet_life_time,
		p_opts->vl_stall_count,
		p_opts->leaf_vl_stall_count,
		p_opts->head_of_queue_lifetime,
		p_opts->leaf_head_of_queue_lifetime,
		p_opts->max_op_vls,
		p_opts->force_link_speed,
		p_opts->force_link_speed_ext,
		p_opts->fdr10,
		p_opts->subnet_timeout,
		p_opts->local_phy_errors_threshold,
		p_opts->overrun_errors_threshold,
		p_opts->use_mfttop ? "TRUE" : "FALSE");

	fprintf(out,
		"#\n# PARTITIONING OPTIONS\n#\n"
		"# Partition configuration file to be used\n"
		"partition_config_file %s\n\n"
		"# Disable partition enforcement by switches (DEPRECATED)\n"
		"# This option is DEPRECATED. Please use part_enforce instead\n"
		"no_partition_enforcement %s\n\n"
		"# Partition enforcement type (for switches)\n"
		"# Values are both, out, in and off\n"
		"# Default is both (outbound and inbound enforcement)\n"
		"part_enforce %s\n\n"
		"# Allow both full and limited membership on the same partition\n"
		"allow_both_pkeys %s\n\n"
		"# SM assigned GUID byte where GUID is formed from OpenFabrics OUI\n"
		"# followed by 40 bits xy 00 ab cd ef where xy is the SM assigned GUID byte\n"
		"# and ab cd ef is an SM autogenerated 24 bits\n"
		"# SM assigned GUID byte should be configured as subnet unique\n"
		"sm_assigned_guid 0x%02x\n\n",
		p_opts->partition_config_file,
		p_opts->no_partition_enforcement ? "TRUE" : "FALSE",
		p_opts->part_enforce,
		p_opts->allow_both_pkeys ? "TRUE" : "FALSE",
		p_opts->sm_assigned_guid);

	fprintf(out,
		"#\n# SWEEP OPTIONS\n#\n"
		"# The number of seconds between subnet sweeps (0 disables it)\n"
		"sweep_interval %u\n\n"
		"# If TRUE cause all lids to be reassigned\n"
		"reassign_lids %s\n\n"
		"# If TRUE forces every sweep to be a heavy sweep\n"
		"force_heavy_sweep %s\n\n"
		"# If TRUE every trap will cause a heavy sweep.\n"
		"# NOTE: successive identical traps (>10) are suppressed\n"
		"sweep_on_trap %s\n\n",
		p_opts->sweep_interval,
		p_opts->reassign_lids ? "TRUE" : "FALSE",
		p_opts->force_heavy_sweep ? "TRUE" : "FALSE",
		p_opts->sweep_on_trap ? "TRUE" : "FALSE");

	fprintf(out,
		"#\n# ROUTING OPTIONS\n#\n"
		"# If TRUE count switches as link subscriptions\n"
		"port_profile_switch_nodes %s\n\n",
		p_opts->port_profile_switch_nodes ? "TRUE" : "FALSE");

	fprintf(out,
		"# Name of file with port guids to be ignored by port profiling\n"
		"port_prof_ignore_file %s\n\n", p_opts->port_prof_ignore_file ?
		p_opts->port_prof_ignore_file : null_str);

	fprintf(out,
		"# The file holding routing weighting factors per output port\n"
		"hop_weights_file %s\n\n",
		p_opts->hop_weights_file ? p_opts->hop_weights_file : null_str);

	fprintf(out,
		"# The file holding non-default port order per switch for routing\n"
		"port_search_ordering_file %s\n\n",
		p_opts->port_search_ordering_file ?
		p_opts->port_search_ordering_file : null_str);

	fprintf(out,
		"# Routing engine\n"
		"# Multiple routing engines can be specified separated by\n"
		"# commas so that specific ordering of routing algorithms will\n"
		"# be tried if earlier routing engines fail.\n"
		"# Supported engines: minhop, updn, dnup, file, ftree, lash,\n"
		"#    dor, torus-2QoS, dfsssp, sssp\n"
		"routing_engine %s\n\n", p_opts->routing_engine_names ?
		p_opts->routing_engine_names : null_str);

	fprintf(out,
		"# Connect roots (use FALSE if unsure)\n"
		"connect_roots %s\n\n",
		p_opts->connect_roots ? "TRUE" : "FALSE");

	fprintf(out,
		"# Use unicast routing cache (use FALSE if unsure)\n"
		"use_ucast_cache %s\n\n",
		p_opts->use_ucast_cache ? "TRUE" : "FALSE");

	fprintf(out,
		"# Lid matrix dump file name\n"
		"lid_matrix_dump_file %s\n\n", p_opts->lid_matrix_dump_file ?
		p_opts->lid_matrix_dump_file : null_str);

	fprintf(out,
		"# LFTs file name\nlfts_file %s\n\n",
		p_opts->lfts_file ? p_opts->lfts_file : null_str);

	fprintf(out,
		"# The file holding the root node guids (for fat-tree or Up/Down)\n"
		"# One guid in each line\nroot_guid_file %s\n\n",
		p_opts->root_guid_file ? p_opts->root_guid_file : null_str);

	fprintf(out,
		"# The file holding the fat-tree compute node guids\n"
		"# One guid in each line\ncn_guid_file %s\n\n",
		p_opts->cn_guid_file ? p_opts->cn_guid_file : null_str);

	fprintf(out,
		"# The file holding the fat-tree I/O node guids\n"
		"# One guid in each line.\n"
		"# If only io_guid file is provided, the rest of nodes\n"
		"# are considered as compute nodes.\n"
		"io_guid_file %s\n\n",
		p_opts->io_guid_file ? p_opts->io_guid_file : null_str);

	fprintf(out,
		"# Number of reverse hops allowed for I/O nodes\n"
		"# Used for connectivity between I/O nodes connected to Top Switches\nmax_reverse_hops %d\n\n",
		p_opts->max_reverse_hops);

	fprintf(out,
		"# The file holding the node ids which will be used by"
		" Up/Down algorithm instead\n# of GUIDs (one guid and"
		" id in each line)\nids_guid_file %s\n\n",
		p_opts->ids_guid_file ? p_opts->ids_guid_file : null_str);

	fprintf(out,
		"# The file holding guid routing order guids (for MinHop and Up/Down)\n"
		"guid_routing_order_file %s\n\n",
		p_opts->guid_routing_order_file ? p_opts->guid_routing_order_file : null_str);

	fprintf(out,
		"# Do mesh topology analysis (for LASH algorithm)\n"
		"do_mesh_analysis %s\n\n",
		p_opts->do_mesh_analysis ? "TRUE" : "FALSE");

	fprintf(out,
		"# Starting VL for LASH algorithm\n"
		"lash_start_vl %u\n\n",
		p_opts->lash_start_vl);

	fprintf(out,
		"# Port Shifting (use FALSE if unsure)\n"
		"port_shifting %s\n\n",
		p_opts->port_shifting ? "TRUE" : "FALSE");

	fprintf(out,
		"# Assign ports in a random order instead of round-robin\n"
		"# If zero disable (default), otherwise use the value as a random seed\n"
		"scatter_ports %d\n\n",
		p_opts->scatter_ports);

	fprintf(out,
		"# SA database file name\nsa_db_file %s\n\n",
		p_opts->sa_db_file ? p_opts->sa_db_file : null_str);

	fprintf(out,
		"# If TRUE causes OpenSM to dump SA database at the end of\n"
		"# every light sweep, regardless of the verbosity level\n"
		"sa_db_dump %s\n\n",
		p_opts->sa_db_dump ? "TRUE" : "FALSE");

	fprintf(out,
		"# Torus-2QoS configuration file name\ntorus_config %s\n\n",
		p_opts->torus_conf_file ? p_opts->torus_conf_file : null_str);

	fprintf(out,
		"#\n# HANDOVER - MULTIPLE SMs OPTIONS\n#\n"
		"# SM priority used for deciding who is the master\n"
		"# Range goes from 0 (lowest priority) to 15 (highest).\n"
		"sm_priority %u\n\n"
		"# If TRUE other SMs on the subnet should be ignored\n"
		"ignore_other_sm %s\n\n"
		"# Timeout in [msec] between two polls of active master SM\n"
		"sminfo_polling_timeout %u\n\n"
		"# Number of failing polls of remote SM that declares it dead\n"
		"polling_retry_number %u\n\n"
		"# If TRUE honor the guid2lid file when coming out of standby\n"
		"# state, if such file exists and is valid\n"
		"honor_guid2lid_file %s\n\n",
		p_opts->sm_priority,
		p_opts->ignore_other_sm ? "TRUE" : "FALSE",
		p_opts->sminfo_polling_timeout,
		p_opts->polling_retry_number,
		p_opts->honor_guid2lid_file ? "TRUE" : "FALSE");

	fprintf(out,
		"#\n# TIMING AND THREADING OPTIONS\n#\n"
		"# Maximum number of SMPs sent in parallel\n"
		"max_wire_smps %u\n\n"
		"# Maximum number of timeout based SMPs allowed to be outstanding\n"
		"# A value less than or equal to max_wire_smps disables this mechanism\n"
		"max_wire_smps2 %u\n\n"
		"# The timeout in [usec] used for sending SMPs above max_wire_smps limit\n"
		"# and below max_wire_smps2 limit\n"
		"max_smps_timeout %u\n\n"
		"# The maximum time in [msec] allowed for a transaction to complete\n"
		"transaction_timeout %u\n\n"
		"# The maximum number of retries allowed for a transaction to complete\n"
		"transaction_retries %u\n\n"
		"# Maximal time in [msec] a message can stay in the incoming message queue.\n"
		"# If there is more than one message in the queue and the last message\n"
		"# stayed in the queue more than this value, any SA request will be\n"
		"# immediately be dropped but BUSY status is not currently returned.\n"
		"max_msg_fifo_timeout %u\n\n"
		"# Use a single thread for handling SA queries\n"
		"single_thread %s\n\n",
		p_opts->max_wire_smps,
		p_opts->max_wire_smps2,
		p_opts->max_smps_timeout,
		p_opts->transaction_timeout,
		p_opts->transaction_retries,
		p_opts->max_msg_fifo_timeout,
		p_opts->single_thread ? "TRUE" : "FALSE");

	fprintf(out,
		"#\n# MISC OPTIONS\n#\n"
		"# Daemon mode\n"
		"daemon %s\n\n"
		"# SM Inactive\n"
		"sm_inactive %s\n\n"
		"# Babbling Port Policy\n"
		"babbling_port_policy %s\n\n"
		"# Drop event subscriptions (InformInfo) if the port goes away\n"
		"drop_event_subscriptions %s\n\n"
		"# Use Optimized SLtoVLMapping programming if supported by device\n"
		"use_optimized_slvl %s\n\n"
		"# Sync in memory files used for high availability with storage\n"
		"fsync_high_avail_files %s\n\n",
		p_opts->daemon ? "TRUE" : "FALSE",
		p_opts->sm_inactive ? "TRUE" : "FALSE",
		p_opts->babbling_port_policy ? "TRUE" : "FALSE",
		p_opts->drop_event_subscriptions ? "TRUE" : "FALSE",
		p_opts->use_optimized_slvl ? "TRUE" : "FALSE",
		p_opts->fsync_high_avail_files ? "TRUE" : "FALSE");

#ifdef ENABLE_OSM_PERF_MGR
	fprintf(out,
		"#\n# Performance Manager Options\n#\n"
		"# perfmgr enable\n"
		"# PerfMgr is enabled if TRUE and disabled if FALSE (default FALSE)\n"
		"perfmgr %s\n\n"
		"# redirection enable\n"
		"# Redirection supported if TRUE and not supported if FALSE (default TRUE)\n"
		"perfmgr_redir %s\n\n"
		"# sweep time in seconds (default %u seconds)\n"
		"perfmgr_sweep_time_s %u\n\n"
		"# Max outstanding queries (default %u)\n"
		"perfmgr_max_outstanding_queries %u\n\n"
		"# Ignore CAs on sweep (default FALSE)\n"
		"perfmgr_ignore_cas %s\n\n"
		"# Remove missing nodes from DB (default TRUE)\n"
		"perfmgr_rm_nodes %s\n\n"
		"# Log error counters to opensm.log (default TRUE)\n"
		"perfmgr_log_errors %s\n\n"
		"# Query PerfMgt Get(ClassPortInfo) for extended capabilities\n"
		"# Extended capabilities include 64 bit extended counters\n"
		"# and transmit wait support (default TRUE)\n"
		"perfmgr_query_cpi %s\n\n"
		"# Log xmit_wait errors (default FALSE)\n"
		"perfmgr_xmit_wait_log %s\n\n"
		"# If logging xmit_wait's; set threshold (default %u)\n"
		"perfmgr_xmit_wait_threshold %u\n\n"
		,
		p_opts->perfmgr ? "TRUE" : "FALSE",
		p_opts->perfmgr_redir ? "TRUE" : "FALSE",
		OSM_PERFMGR_DEFAULT_SWEEP_TIME_S,
		p_opts->perfmgr_sweep_time_s,
		OSM_PERFMGR_DEFAULT_MAX_OUTSTANDING_QUERIES,
		p_opts->perfmgr_max_outstanding_queries,
		p_opts->perfmgr_ignore_cas ? "TRUE" : "FALSE",
		p_opts->perfmgr_rm_nodes ? "TRUE" : "FALSE",
		p_opts->perfmgr_log_errors ? "TRUE" : "FALSE",
		p_opts->perfmgr_query_cpi ? "TRUE" : "FALSE",
		p_opts->perfmgr_xmit_wait_log ? "TRUE" : "FALSE",
		OSM_PERFMGR_DEFAULT_XMIT_WAIT_THRESHOLD,
		p_opts->perfmgr_xmit_wait_threshold);

	fprintf(out,
		"#\n# Event DB Options\n#\n"
		"# Dump file to dump the events to\n"
		"event_db_dump_file %s\n\n", p_opts->event_db_dump_file ?
		p_opts->event_db_dump_file : null_str);
#endif				/* ENABLE_OSM_PERF_MGR */

	fprintf(out,
		"#\n# Event Plugin Options\n#\n"
		"# Event plugin name(s)\n"
		"event_plugin_name %s\n\n"
		"# Options string that would be passed to the plugin(s)\n"
		"event_plugin_options %s\n\n",
		p_opts->event_plugin_name ?
		p_opts->event_plugin_name : null_str,
		p_opts->event_plugin_options ?
		p_opts->event_plugin_options : null_str);

	fprintf(out,
		"#\n# Node name map for mapping node's to more descriptive node descriptions\n"
		"# (man ibnetdiscover for more information)\n#\n"
		"node_name_map_name %s\n\n", p_opts->node_name_map_name ?
		p_opts->node_name_map_name : null_str);

	fprintf(out,
		"#\n# DEBUG FEATURES\n#\n"
		"# The log flags used\n"
		"log_flags 0x%02x\n\n"
		"# Force flush of the log file after each log message\n"
		"force_log_flush %s\n\n"
		"# Log file to be used\n"
		"log_file %s\n\n"
		"# Limit the size of the log file in MB. If overrun, log is restarted\n"
		"log_max_size %lu\n\n"
		"# If TRUE will accumulate the log over multiple OpenSM sessions\n"
		"accum_log_file %s\n\n"
		"# Per module logging configuration file\n"
		"# Each line in config file contains <module_name><separator><log_flags>\n"
		"# where module_name is file name including .c\n"
		"# separator is either = , space, or tab\n"
		"# log_flags is the same flags as used in the coarse/overall logging\n"
		"per_module_logging_file %s\n\n"
		"# The directory to hold the file OpenSM dumps\n"
		"dump_files_dir %s\n\n"
		"# If TRUE enables new high risk options and hardware specific quirks\n"
		"enable_quirks %s\n\n"
		"# If TRUE disables client reregistration\n"
		"no_clients_rereg %s\n\n"
		"# If TRUE OpenSM should disable multicast support and\n"
		"# no multicast routing is performed if TRUE\n"
		"disable_multicast %s\n\n"
		"# If TRUE opensm will exit on fatal initialization issues\n"
		"exit_on_fatal %s\n\n" "# console [off|local"
#ifdef ENABLE_OSM_CONSOLE_LOOPBACK
		"|loopback"
#endif
#ifdef ENABLE_OSM_CONSOLE_SOCKET
		"|socket]\n"
#else
		"]\n"
#endif
		"console %s\n\n"
		"# Telnet port for console (default %d)\n"
		"console_port %d\n\n",
		p_opts->log_flags,
		p_opts->force_log_flush ? "TRUE" : "FALSE",
		p_opts->log_file,
		p_opts->log_max_size,
		p_opts->accum_log_file ? "TRUE" : "FALSE",
		p_opts->per_module_logging_file ?
			p_opts->per_module_logging_file : null_str,
		p_opts->dump_files_dir,
		p_opts->enable_quirks ? "TRUE" : "FALSE",
		p_opts->no_clients_rereg ? "TRUE" : "FALSE",
		p_opts->disable_multicast ? "TRUE" : "FALSE",
		p_opts->exit_on_fatal ? "TRUE" : "FALSE",
		p_opts->console,
		OSM_DEFAULT_CONSOLE_PORT, p_opts->console_port);

	fprintf(out,
		"#\n# QoS OPTIONS\n#\n"
		"# Enable QoS setup\n"
		"qos %s\n\n"
		"# QoS policy file to be used\n"
		"qos_policy_file %s\n"
		"# Supress QoS MAD status errors\n"
		"suppress_sl2vl_mad_status_errors %s\n\n",
		p_opts->qos ? "TRUE" : "FALSE", p_opts->qos_policy_file,
		p_opts->suppress_sl2vl_mad_status_errors ? "TRUE" : "FALSE");

	subn_dump_qos_options(out,
			      "QoS default options", "qos",
			      &p_opts->qos_options);
	fprintf(out, "\n");
	subn_dump_qos_options(out,
			      "QoS CA options", "qos_ca",
			      &p_opts->qos_ca_options);
	fprintf(out, "\n");
	subn_dump_qos_options(out,
			      "QoS Switch Port 0 options", "qos_sw0",
			      &p_opts->qos_sw0_options);
	fprintf(out, "\n");
	subn_dump_qos_options(out,
			      "QoS Switch external ports options", "qos_swe",
			      &p_opts->qos_swe_options);
	fprintf(out, "\n");
	subn_dump_qos_options(out,
			      "QoS Router ports options", "qos_rtr",
			      &p_opts->qos_rtr_options);
	fprintf(out, "\n");

	fprintf(out,
		"#\n# Congestion Control OPTIONS (EXPERIMENTAL)\n#\n\n"
		"# Enable Congestion Control Configuration\n"
		"congestion_control %s\n\n"
		"# CCKey to use when configuring congestion control\n"
		"# note that this does not configure a new CCkey, only the CCkey to use\n"
		"cc_key 0x%016" PRIx64 "\n\n"
		"# Congestion Control Max outstanding MAD\n"
		"cc_max_outstanding_mads %u\n\n",
		p_opts->congestion_control ? "TRUE" : "FALSE",
		cl_ntoh64(p_opts->cc_key),
		p_opts->cc_max_outstanding_mads);

	fprintf(out,
		"#\n# Congestion Control SwitchCongestionSetting options\n#\n"
		"# Control Map - bitmask indicating which of the following attributes are to be used\n"
		"# bit 0 - victim mask\n"
		"# bit 1 - credit mask\n"
		"# bit 2 - threshold + packet size\n"
		"# bit 3 - credit starvation threshold + return delay valid\n"
		"# bit 4 - marking rate valid\n"
		"cc_sw_cong_setting_control_map 0x%X\n\n",
		cl_ntoh32(p_opts->cc_sw_cong_setting_control_map));

	fprintf(out,
		"# Victim Mask - 256 bit mask representing switch ports, mark packets with FECN\n"
		"# whether they are the source or victim of congestion\n"
		"# bit 0 - port 0 (enhanced port)\n"
		"# bit 1 - port 1\n"
		"# ...\n"
		"# bit 254 - port 254\n"
		"# bit 255 - reserved\n"
		"cc_sw_cong_setting_victim_mask 0x");

	for (i = 0; i < IB_CC_PORT_MASK_DATA_SIZE; i++)
		fprintf(out, "%02X", p_opts->cc_sw_cong_setting_victim_mask[i]);
	fprintf(out, "\n\n");

	fprintf(out,
		"# Credit Mask - 256 bit mask representing switch ports to apply credit starvation\n"
		"# bit 0 - port 0 (enhanced port)\n"
		"# bit 1 - port 1\n"
		"# ...\n"
		"# bit 254 - port 254\n"
		"# bit 255 - reserved\n"
		"cc_sw_cong_setting_credit_mask 0x");

	for (i = 0; i < IB_CC_PORT_MASK_DATA_SIZE; i++)
		fprintf(out, "%02X", p_opts->cc_sw_cong_setting_credit_mask[i]);
	fprintf(out, "\n\n");

	fprintf(out,
		"# Threshold - value indicating aggressiveness of congestion marking\n"
		"# 0x0 - none, 0x1 - loose, ..., 0xF - aggressive\n"
		"cc_sw_cong_setting_threshold 0x%02X\n\n"
		"# Packet Size - any packet less than this size will not be marked with a FECN\n"
		"# units are in credits\n"
		"cc_sw_cong_setting_packet_size %u\n\n"
		"# Credit Starvation Threshold - value indicating aggressiveness of credit starvation\n"
		"# 0x0 - none, 0x1 - loose, ..., 0xF - aggressive\n"
		"cc_sw_cong_setting_credit_starvation_threshold 0x%02X\n\n"
		"# Credit Starvation Return Delay - in CCT entry shift:multiplier format, see IB spec\n"
		"cc_sw_cong_setting_credit_starvation_return_delay %u:%u\n\n"
		"# Marking Rate - mean number of packets between markings\n"
		"cc_sw_cong_setting_marking_rate %u\n\n",
		p_opts->cc_sw_cong_setting_threshold,
		p_opts->cc_sw_cong_setting_packet_size,
		p_opts->cc_sw_cong_setting_credit_starvation_threshold,
		p_opts->cc_sw_cong_setting_credit_starvation_return_delay.shift,
		p_opts->cc_sw_cong_setting_credit_starvation_return_delay.multiplier,
		cl_ntoh16(p_opts->cc_sw_cong_setting_marking_rate));

	fprintf(out,
		"#\n# Congestion Control CA Congestion Setting options\n#\n"
		"# Port Control\n"
		"# bit 0 = 0, QP based congestion control\n"
		"# bit 0 = 1, SL/port based congestion control\n"
		"cc_ca_cong_setting_port_control 0x%04X\n\n"
		"# Control Map - 16 bit bitmask indicating which SLs should be configured\n"
		"cc_ca_cong_setting_control_map 0x%04X\n\n",
		cl_ntoh16(p_opts->cc_ca_cong_setting_port_control),
		cl_ntoh16(p_opts->cc_ca_cong_setting_control_map));

	fprintf(out,
		"#\n# CA Congestion Setting Entries\n#\n"
		"# Each of congestion control settings below configures the CA Congestion\n"
		"# Settings for an individual SL.  The SL must be specified before the value.\n"
		"# These options may be specified multiple times to configure different values\n"
		"# for different SLs.\n"
		"#\n"
		"# ccti timer - when expires decrements 1 from the CCTI\n"
		"# ccti increase - number to be added to the table index on receipt of a BECN\n"
		"# trigger threshold - when the ccti is equal to this, an event is logged\n"
		"# ccti min - the minimum value for the ccti.  This imposes a minimum rate\n"
		"#            on the injection rate\n\n");

	for (i = 0; i < IB_CA_CONG_ENTRY_DATA_SIZE; i++) {
		/* Don't output unless one of the settings has been set, there's no need
		 * to output 16 chunks of this with all defaults of 0 */
		if (p_opts->cc_ca_cong_entries[i].ccti_timer
		    || p_opts->cc_ca_cong_entries[i].ccti_increase
		    || p_opts->cc_ca_cong_entries[i].trigger_threshold
		    || p_opts->cc_ca_cong_entries[i].ccti_min) {
			fprintf(out,
				"# SL = %u\n"
				"cc_ca_cong_setting_ccti_timer %u %u\n"
				"cc_ca_cong_setting_ccti_increase %u %u\n"
				"cc_ca_cong_setting_trigger_threshold %u %u\n"
				"cc_ca_cong_setting_ccti_min %u %u\n\n",
				i,
				i,
				cl_ntoh16(p_opts->cc_ca_cong_entries[i].ccti_timer),
				i,
				p_opts->cc_ca_cong_entries[i].ccti_increase,
				i,
				p_opts->cc_ca_cong_entries[i].trigger_threshold,
				i,
				p_opts->cc_ca_cong_entries[i].ccti_min);
			cacongoutputcount++;
		}
	}

	/* If by chance all the CA Cong Settings are default, output atleast 1 chunk
         * for illustration */
	if (!cacongoutputcount)
		fprintf(out,
			"# SL = 0\n"
			"cc_ca_cong_setting_ccti_timer 0 %u\n"
			"cc_ca_cong_setting_ccti_increase 0 %u\n"
			"cc_ca_cong_setting_trigger_threshold 0 %u\n"
			"cc_ca_cong_setting_ccti_min 0 %u\n\n",
			cl_ntoh16(p_opts->cc_ca_cong_entries[0].ccti_timer),
			p_opts->cc_ca_cong_entries[0].ccti_increase,
			p_opts->cc_ca_cong_entries[0].trigger_threshold,
			p_opts->cc_ca_cong_entries[0].ccti_min);

	fprintf(out,
		"#\n# Congestion Control Table\n#\n"
		"# Comma separated list of CCT entries representing CCT.\n"
		"# Format is shift:multipler,shift_multiplier,shift:multiplier,...\n"
		"cc_cct ");

	if (!p_opts->cc_cct.entries_len) {
		fprintf(out, "%s\n", null_str);
	}
	else {
		fprintf(out, "%u:%u",
			p_opts->cc_cct.entries[0].shift,
			p_opts->cc_cct.entries[0].multiplier);
		for (i = 0; i < p_opts->cc_cct.entries_len; i++) {
			fprintf(out, ",%u:%u",
				p_opts->cc_cct.entries[0].shift,
				p_opts->cc_cct.entries[0].multiplier);
		}
		fprintf(out, "\n");
	}
	fprintf(out, "\n");

	fprintf(out,
		"# Prefix routes file name\n"
		"prefix_routes_file %s\n\n",
		p_opts->prefix_routes_file);

	fprintf(out,
		"#\n# IPv6 Solicited Node Multicast (SNM) Options\n#\n"
		"consolidate_ipv6_snm_req %s\n\n",
		p_opts->consolidate_ipv6_snm_req ? "TRUE" : "FALSE");

	fprintf(out, "# Log prefix\nlog_prefix %s\n\n", p_opts->log_prefix);

	/* optional string attributes ... */

	return 0;
}