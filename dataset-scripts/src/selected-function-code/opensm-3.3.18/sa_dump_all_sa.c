static void sa_dump_all_sa(osm_opensm_t * p_osm, FILE * file)
{
	struct opensm_dump_context dump_context;
	osm_mgrp_t *p_mgrp;

	dump_context.p_osm = p_osm;
	dump_context.file = file;
	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG, "Dump guidinfo\n");
	cl_plock_acquire(&p_osm->lock);
	cl_qmap_apply_func(&p_osm->subn.port_guid_tbl,
			   sa_dump_one_port_guidinfo, &dump_context);
	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG, "Dump multicast\n");
	for (p_mgrp = (osm_mgrp_t *) cl_fmap_head(&p_osm->subn.mgrp_mgid_tbl);
	     p_mgrp != (osm_mgrp_t *) cl_fmap_end(&p_osm->subn.mgrp_mgid_tbl);
	     p_mgrp = (osm_mgrp_t *) cl_fmap_next(&p_mgrp->map_item))
		sa_dump_one_mgrp(p_mgrp, &dump_context);
	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG, "Dump inform\n");
	cl_qlist_apply_func(&p_osm->subn.sa_infr_list,
			    sa_dump_one_inform, &dump_context);
	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG, "Dump services\n");
	cl_qlist_apply_func(&p_osm->subn.sa_sr_list,
			    sa_dump_one_service, &dump_context);
	cl_plock_release(&p_osm->lock);
}