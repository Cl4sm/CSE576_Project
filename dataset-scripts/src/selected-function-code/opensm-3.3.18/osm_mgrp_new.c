osm_mgrp_t *osm_mgrp_new(IN osm_subn_t * subn, IN ib_net16_t mlid,
			 IN ib_member_rec_t * mcmr)
{
	osm_mgrp_t *p_mgrp;
	osm_mgrp_box_t *mbox;

	p_mgrp = (osm_mgrp_t *) malloc(sizeof(*p_mgrp));
	if (!p_mgrp)
		return NULL;

	memset(p_mgrp, 0, sizeof(*p_mgrp));
	cl_qmap_init(&p_mgrp->mcm_port_tbl);
	cl_qmap_init(&p_mgrp->mcm_alias_port_tbl);
	p_mgrp->mlid = mlid;
	p_mgrp->mcmember_rec = *mcmr;

	mbox = osm_get_mbox_by_mlid(subn, p_mgrp->mlid);
	if (!mbox && !(mbox = mgrp_box_new(cl_ntoh16(p_mgrp->mlid)))) {
		free(p_mgrp);
		return NULL;
	}

	cl_qlist_insert_tail(&mbox->mgrp_list, &p_mgrp->list_item);
	subn->mboxes[mbox->mlid - IB_LID_MCAST_START_HO] = mbox;

	cl_fmap_insert(&subn->mgrp_mgid_tbl, &p_mgrp->mcmember_rec.mgid,
		       &p_mgrp->map_item);

	return p_mgrp;
}