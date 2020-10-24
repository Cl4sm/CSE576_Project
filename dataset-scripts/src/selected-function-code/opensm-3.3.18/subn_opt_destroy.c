static void subn_opt_destroy(IN osm_subn_opt_t * p_opt)
{
	free(p_opt->console);
	free(p_opt->port_prof_ignore_file);
	free(p_opt->hop_weights_file);
	free(p_opt->port_search_ordering_file);
	free(p_opt->routing_engine_names);
	free(p_opt->log_file);
	free(p_opt->partition_config_file);
	free(p_opt->qos_policy_file);
	free(p_opt->dump_files_dir);
	free(p_opt->part_enforce);
	free(p_opt->lid_matrix_dump_file);
	free(p_opt->lfts_file);
	free(p_opt->root_guid_file);
	free(p_opt->cn_guid_file);
	free(p_opt->io_guid_file);
	free(p_opt->ids_guid_file);
	free(p_opt->guid_routing_order_file);
	free(p_opt->sa_db_file);
	free(p_opt->torus_conf_file);
#ifdef ENABLE_OSM_PERF_MGR
	free(p_opt->event_db_dump_file);
#endif /* ENABLE_OSM_PERF_MGR */
	free(p_opt->event_plugin_name);
	free(p_opt->event_plugin_options);
	free(p_opt->node_name_map_name);
	free(p_opt->prefix_routes_file);
	free(p_opt->log_prefix);
	subn_destroy_qos_options(&p_opt->qos_options);
	subn_destroy_qos_options(&p_opt->qos_ca_options);
	subn_destroy_qos_options(&p_opt->qos_sw0_options);
	subn_destroy_qos_options(&p_opt->qos_swe_options);
	subn_destroy_qos_options(&p_opt->qos_rtr_options);
	free(p_opt->cc_cct.input_str);
}