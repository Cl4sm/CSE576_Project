static void dump_sms(osm_opensm_t * p_osm, FILE * out)
{
	osm_subn_t *p_subn = &p_osm->subn;
	osm_remote_sm_t *p_rsm;

	fprintf(out, "\n   Known SMs\n"
		     "   ---------\n");
	fprintf(out, "   Port GUID       SM State    Priority\n");
	fprintf(out, "   ---------       --------    --------\n");
	fprintf(out, "   0x%" PRIx64 " %s %d        SELF\n",
		cl_ntoh64(p_subn->sm_port_guid),
		sm_state_str(p_subn->sm_state),
		p_subn->opt.sm_priority);

	CL_PLOCK_ACQUIRE(p_osm->sm.p_lock);
	p_rsm = (osm_remote_sm_t *) cl_qmap_head(&p_subn->sm_guid_tbl);
	while (p_rsm != (osm_remote_sm_t *) cl_qmap_end(&p_subn->sm_guid_tbl)) {
		fprintf(out, "   0x%" PRIx64 " %s %d\n",
			cl_ntoh64(p_rsm->smi.guid),
			sm_state_str(ib_sminfo_get_state(&p_rsm->smi)),
			ib_sminfo_get_priority(&p_rsm->smi));
		p_rsm = (osm_remote_sm_t *) cl_qmap_next(&p_rsm->map_item);
	}
	CL_PLOCK_RELEASE(p_osm->sm.p_lock);
}