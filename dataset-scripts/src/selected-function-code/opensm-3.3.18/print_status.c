static void print_status(osm_opensm_t * p_osm, FILE * out)
{
	cl_list_item_t *item;

	if (out) {
		const char *re_str;

		cl_plock_acquire(&p_osm->lock);
		fprintf(out, "   OpenSM Version       : %s\n", p_osm->osm_version);
		fprintf(out, "   SM State             : %s\n",
			sm_state_str(p_osm->subn.sm_state));
		fprintf(out, "   SM Priority          : %d\n",
			p_osm->subn.opt.sm_priority);
		fprintf(out, "   SA State             : %s\n",
			sa_state_str(p_osm->sa.state));

		re_str = p_osm->routing_engine_used ?
			osm_routing_engine_type_str(p_osm->routing_engine_used->type) :
			osm_routing_engine_type_str(OSM_ROUTING_ENGINE_TYPE_NONE);
		fprintf(out, "   Routing Engine       : %s\n", re_str);

		fprintf(out, "   Loaded event plugins :");
		if (cl_qlist_head(&p_osm->plugin_list) ==
			cl_qlist_end(&p_osm->plugin_list)) {
			fprintf(out, " <none>");
		}
		for (item = cl_qlist_head(&p_osm->plugin_list);
		     item != cl_qlist_end(&p_osm->plugin_list);
		     item = cl_qlist_next(item))
			fprintf(out, " %s",
				((osm_epi_plugin_t *)item)->plugin_name);
		fprintf(out, "\n");

#ifdef ENABLE_OSM_PERF_MGR
		fprintf(out, "\n   PerfMgr state/sweep state : %s/%s\n",
			osm_perfmgr_get_state_str(&p_osm->perfmgr),
			osm_perfmgr_get_sweep_state_str(&p_osm->perfmgr));
#endif
		fprintf(out, "\n   MAD stats\n"
			"   ---------\n"
			"   QP0 MADs outstanding           : %d\n"
			"   QP0 MADs outstanding (on wire) : %d\n"
			"   QP0 MADs rcvd                  : %d\n"
			"   QP0 MADs sent                  : %d\n"
			"   QP0 unicasts sent              : %d\n"
			"   QP0 unknown MADs rcvd          : %d\n"
			"   SA MADs outstanding            : %d\n"
			"   SA MADs rcvd                   : %d\n"
			"   SA MADs sent                   : %d\n"
			"   SA unknown MADs rcvd           : %d\n"
			"   SA MADs ignored                : %d\n",
			p_osm->stats.qp0_mads_outstanding,
			p_osm->stats.qp0_mads_outstanding_on_wire,
			p_osm->stats.qp0_mads_rcvd,
			p_osm->stats.qp0_mads_sent,
			p_osm->stats.qp0_unicasts_sent,
			p_osm->stats.qp0_mads_rcvd_unknown,
			p_osm->stats.sa_mads_outstanding,
			p_osm->stats.sa_mads_rcvd,
			p_osm->stats.sa_mads_sent,
			p_osm->stats.sa_mads_rcvd_unknown,
			p_osm->stats.sa_mads_ignored);
		fprintf(out, "\n   Subnet flags\n"
			"   ------------\n"
			"   Sweeping enabled               : %d\n"
			"   Sweep interval (seconds)       : %d\n"
			"   Ignore existing lfts           : %d\n"
			"   Subnet Init errors             : %d\n"
			"   In sweep hop 0                 : %d\n"
			"   First time master sweep        : %d\n"
			"   Coming out of standby          : %d\n",
			p_osm->subn.sweeping_enabled,
			p_osm->subn.opt.sweep_interval,
			p_osm->subn.ignore_existing_lfts,
			p_osm->subn.subnet_initialization_error,
			p_osm->subn.in_sweep_hop_0,
			p_osm->subn.first_time_master_sweep,
			p_osm->subn.coming_out_of_standby);
		dump_sms(p_osm, out);
		fprintf(out, "\n");
		cl_plock_release(&p_osm->lock);
	}
}