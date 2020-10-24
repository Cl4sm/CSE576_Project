static struct hwentry *
extract_hwe_from_path(struct multipath * mpp)
{
	struct path * pp = NULL;
	int pg_num = -1, p_num = -1, i;
	struct pathgroup * pgp = NULL;

	condlog(3, "%s: searching paths for valid hwe", mpp->alias);

	if (mpp && mpp->pg) {
		vector_foreach_slot(mpp->pg, pgp, i) {
			if (pgp->status == PGSTATE_ACTIVE ||
			    pgp->status == PGSTATE_ENABLED) {
				pg_num = i;
				break;
			}
		}
		if (pg_num >= 0)
			pgp = VECTOR_SLOT(mpp->pg, pg_num);
	}

	if (pgp && pgp->paths) {
		vector_foreach_slot(pgp->paths, pp, i) {
			if (pp->dmstate == PSTATE_FAILED)
				continue;
			if (strlen(pp->vendor_id) > 0 &&
			    strlen(pp->product_id) > 0 &&
			    strlen(pp->rev) > 0) {
				p_num = i;
				break;
			}
		}
		if (p_num >= 0)
			pp = VECTOR_SLOT(pgp->paths, i);
	}

	if (pp) {
		condlog(3, "%s: vendor = %s", pp->dev, pp->vendor_id);
		condlog(3, "%s: product = %s", pp->dev, pp->product_id);
		condlog(3, "%s: rev = %s", pp->dev, pp->rev);
		if (!pp->hwe) {
			condlog(3, "searching hwtable");
			pp->hwe = find_hwe(conf->hwtable, pp->vendor_id,
					   pp->product_id, pp->rev);
		}
	}

	return pp?pp->hwe:NULL;
}
