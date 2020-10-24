get_prio (struct path * pp)
{
	if (!pp)
		return 0;

	struct prio * p = &pp->prio;

	if (!prio_selected(p)) {
		select_detect_prio(pp);
		select_prio(pp);
		if (!prio_selected(p)) {
			condlog(3, "%s: no prio selected", pp->dev);
			pp->priority = PRIO_UNDEF;
			return 1;
		}
	}
	pp->priority = prio_getprio(p, pp);
	if (pp->priority < 0) {
		condlog(3, "%s: %s prio error", pp->dev, prio_name(p));
		pp->priority = PRIO_UNDEF;
		return 1;
	}
	condlog(3, "%s: %s prio = %u",
		pp->dev, prio_name(p), pp->priority);
	return 0;
}
