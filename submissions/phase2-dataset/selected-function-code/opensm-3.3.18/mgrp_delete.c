void mgrp_delete(IN osm_mgrp_t * p_mgrp)
{
	osm_mcm_alias_guid_t *p_mcm_alias_guid, *p_next_mcm_alias_guid;
	osm_mcm_port_t *p_mcm_port, *p_next_mcm_port;

	CL_ASSERT(p_mgrp);

	p_next_mcm_alias_guid =
	    (osm_mcm_alias_guid_t *) cl_qmap_head(&p_mgrp->mcm_alias_port_tbl);
	while (p_next_mcm_alias_guid !=
	       (osm_mcm_alias_guid_t *) cl_qmap_end(&p_mgrp->mcm_alias_port_tbl)) {
		p_mcm_alias_guid = p_next_mcm_alias_guid;
		p_next_mcm_alias_guid =
		    (osm_mcm_alias_guid_t *) cl_qmap_next(&p_mcm_alias_guid->map_item);
		osm_mcm_alias_guid_delete(&p_mcm_alias_guid);
	}

	p_next_mcm_port =
	    (osm_mcm_port_t *) cl_qmap_head(&p_mgrp->mcm_port_tbl);
	while (p_next_mcm_port !=
	       (osm_mcm_port_t *) cl_qmap_end(&p_mgrp->mcm_port_tbl)) {
		p_mcm_port = p_next_mcm_port;
		p_next_mcm_port =
		    (osm_mcm_port_t *) cl_qmap_next(&p_mcm_port->map_item);
		osm_mcm_port_delete(p_mcm_port);
	}

	free(p_mgrp);
}