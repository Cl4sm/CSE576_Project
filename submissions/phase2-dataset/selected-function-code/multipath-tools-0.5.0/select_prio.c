extern int
select_prio (struct path * pp)
{
	struct mpentry * mpe;
	struct prio * p = &pp->prio;

	if (pp->detect_prio == DETECT_PRIO_ON) {
		detect_prio(pp);
		if (prio_selected(p)) {
			condlog(3, "%s: prio = %s (detected setting)",
				pp->dev, prio_name(p));
			return 0;
		}
	}

	if ((mpe = find_mpe(pp->wwid))) {
		if (mpe->prio_name) {
			prio_get(p, mpe->prio_name, mpe->prio_args);
			condlog(3, "%s: prio = %s (LUN setting)",
				pp->dev, prio_name(p));
			return 0;
		}
	}

	if (pp->hwe && pp->hwe->prio_name) {
		prio_get(p, pp->hwe->prio_name, pp->hwe->prio_args);
		condlog(3, "%s: prio = %s (controller setting)",
			pp->dev, pp->hwe->prio_name);
		condlog(3, "%s: prio args = %s (controller setting)",
			pp->dev, pp->hwe->prio_args);
		return 0;
	}
	if (conf->prio_name) {
		prio_get(p, conf->prio_name, conf->prio_args);
		condlog(3, "%s: prio = %s (config file default)",
			pp->dev, conf->prio_name);
		condlog(3, "%s: prio args = %s (config file default)",
			pp->dev, conf->prio_args);
		return 0;
	}
	prio_get(p, DEFAULT_PRIO, DEFAULT_PRIO_ARGS);
	condlog(3, "%s: prio = %s (internal default)",
		pp->dev, DEFAULT_PRIO);
	condlog(3, "%s: prio args = %s (internal default)",
		pp->dev, DEFAULT_PRIO_ARGS);
	return 0;
}
