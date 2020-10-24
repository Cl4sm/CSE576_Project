void osm_mgrp_cleanup(osm_subn_t * subn, osm_mgrp_t * mgrp)
{
	osm_mgrp_box_t *mbox;
	osm_mcm_alias_guid_t *mcm_alias_guid;
	osm_mcm_port_t *mcm_port;

	if (mgrp->full_members)
		return;

	while (cl_qmap_count(&mgrp->mcm_alias_port_tbl)) {
		mcm_alias_guid = (osm_mcm_alias_guid_t *) cl_qmap_head(&mgrp->mcm_alias_port_tbl);
		cl_qmap_remove_item(&mgrp->mcm_alias_port_tbl, &mcm_alias_guid->map_item);
		osm_mcm_alias_guid_delete(&mcm_alias_guid);
	}

	while (cl_qmap_count(&mgrp->mcm_port_tbl)) {
		mcm_port = (osm_mcm_port_t *) cl_qmap_head(&mgrp->mcm_port_tbl);
		cl_qmap_remove_item(&mgrp->mcm_port_tbl, &mcm_port->map_item);
		cl_qlist_remove_item(&mcm_port->port->mcm_list,
				     &mcm_port->list_item);
		osm_mcm_port_delete(mcm_port);
	}

	if (mgrp->well_known)
		return;

	cl_fmap_remove_item(&subn->mgrp_mgid_tbl, &mgrp->map_item);

	mbox = osm_get_mbox_by_mlid(subn, mgrp->mlid);
	cl_qlist_remove_item(&mbox->mgrp_list, &mgrp->list_item);
	if (cl_is_qlist_empty(&mbox->mgrp_list)) {
		subn->mboxes[cl_ntoh16(mgrp->mlid) - IB_LID_MCAST_START_HO] = NULL;
		mgrp_box_delete(mbox);
	}
	free(mgrp);
}