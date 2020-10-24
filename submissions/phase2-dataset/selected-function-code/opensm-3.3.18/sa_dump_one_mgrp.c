static void sa_dump_one_mgrp(osm_mgrp_t *p_mgrp, void *cxt)
{
	struct opensm_dump_context dump_context;
	osm_opensm_t *p_osm = ((struct opensm_dump_context *)cxt)->p_osm;
	FILE *file = ((struct opensm_dump_context *)cxt)->file;

	fprintf(file, "MC Group 0x%04x %s:"
		" mgid=0x%016" PRIx64 ":0x%016" PRIx64
		" port_gid=0x%016" PRIx64 ":0x%016" PRIx64
		" qkey=0x%08x mlid=0x%04x mtu=0x%02x tclass=0x%02x"
		" pkey=0x%04x rate=0x%02x pkt_life=0x%02x sl_flow_hop=0x%08x"
		" scope_state=0x%02x proxy_join=0x%x" "\n\n",
		cl_ntoh16(p_mgrp->mlid),
		p_mgrp->well_known ? " (well known)" : "",
		cl_ntoh64(p_mgrp->mcmember_rec.mgid.unicast.prefix),
		cl_ntoh64(p_mgrp->mcmember_rec.mgid.unicast.interface_id),
		cl_ntoh64(p_mgrp->mcmember_rec.port_gid.unicast.prefix),
		cl_ntoh64(p_mgrp->mcmember_rec.port_gid.unicast.interface_id),
		cl_ntoh32(p_mgrp->mcmember_rec.qkey),
		cl_ntoh16(p_mgrp->mcmember_rec.mlid),
		p_mgrp->mcmember_rec.mtu,
		p_mgrp->mcmember_rec.tclass,
		cl_ntoh16(p_mgrp->mcmember_rec.pkey),
		p_mgrp->mcmember_rec.rate,
		p_mgrp->mcmember_rec.pkt_life,
		cl_ntoh32(p_mgrp->mcmember_rec.sl_flow_hop),
		p_mgrp->mcmember_rec.scope_state,
		p_mgrp->mcmember_rec.proxy_join);

	dump_context.p_osm = p_osm;
	dump_context.file = file;

	cl_qmap_apply_func(&p_mgrp->mcm_alias_port_tbl,
			   mcast_mgr_dump_one_port, &dump_context);
}