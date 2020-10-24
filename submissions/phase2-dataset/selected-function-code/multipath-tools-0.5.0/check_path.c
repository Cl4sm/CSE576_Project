check_path (struct vectors * vecs, struct path * pp)
{
	int newstate;
	int new_path_up = 0;
	int chkr_new_path_up = 0;
	int oldchkrstate = pp->chkrstate;

	if (!pp->mpp)
		return 0;

	if (pp->tick && --pp->tick)
		return 0; /* don't check this path yet */

	/*
	 * provision a next check soonest,
	 * in case we exit abnormaly from here
	 */
	pp->tick = conf->checkint;

	newstate = path_offline(pp);
	if (newstate == PATH_REMOVED) {
		condlog(2, "%s: remove path (checker)", pp->dev);
		ev_remove_path(pp, vecs);
		return 0;
	}
	if (newstate == PATH_UP)
		newstate = get_state(pp, 1);
	else
		checker_clear_message(&pp->checker);

	if (newstate == PATH_WILD || newstate == PATH_UNCHECKED) {
		condlog(2, "%s: unusable path", pp->dev);
		pathinfo(pp, conf->hwtable, 0);
		return 1;
	}
	if (!pp->mpp) {
		if (!strlen(pp->wwid) &&
		    (newstate == PATH_UP || newstate == PATH_GHOST)) {
			condlog(2, "%s: add missing path", pp->dev);
			if (pathinfo(pp, conf->hwtable, DI_ALL) == 0) {
				ev_add_path(pp, vecs);
				pp->tick = 1;
			}
		}
		return 0;
	}
	/*
	 * Async IO in flight. Keep the previous path state
	 * and reschedule as soon as possible
	 */
	if (newstate == PATH_PENDING) {
		pp->tick = 1;
		return 0;
	}
	/*
	 * Synchronize with kernel state
	 */
	if (update_multipath_strings(pp->mpp, vecs->pathvec)) {
		condlog(1, "%s: Could not synchronize with kernel state",
			pp->dev);
		pp->dmstate = PSTATE_UNDEF;
	}
	pp->chkrstate = newstate;
	if (newstate != pp->state) {
		int oldstate = pp->state;
		pp->state = newstate;

		if (strlen(checker_message(&pp->checker)))
			LOG_MSG(1, checker_message(&pp->checker));

		/*
		 * upon state change, reset the checkint
		 * to the shortest delay
		 */
		pp->checkint = conf->checkint;

		if (newstate == PATH_DOWN || newstate == PATH_SHAKY) {
			/*
			 * proactively fail path in the DM
			 */
			if (oldstate == PATH_UP ||
			    oldstate == PATH_GHOST)
				fail_path(pp, 1);
			else
				fail_path(pp, 0);

			/*
			 * cancel scheduled failback
			 */
			pp->mpp->failback_tick = 0;

			pp->mpp->stat_path_failures++;
			return 1;
		}

		if(newstate == PATH_UP || newstate == PATH_GHOST){
			if ( pp->mpp && pp->mpp->prflag ){
				/*
				 * Check Persistent Reservation.
				 */
			condlog(2, "%s: checking persistent reservation "
				"registration", pp->dev);
			mpath_pr_event_handle(pp);
			}
		}

		/*
		 * reinstate this path
		 */
		if (oldstate != PATH_UP &&
		    oldstate != PATH_GHOST)
			reinstate_path(pp, 1);
		else
			reinstate_path(pp, 0);

		new_path_up = 1;

		if (oldchkrstate != PATH_UP && oldchkrstate != PATH_GHOST)
			chkr_new_path_up = 1;

		/*
		 * if at least one path is up in a group, and
		 * the group is disabled, re-enable it
		 */
		if (newstate == PATH_UP)
			enable_group(pp);
	}
	else if (newstate == PATH_UP || newstate == PATH_GHOST) {
		if (pp->dmstate == PSTATE_FAILED ||
		    pp->dmstate == PSTATE_UNDEF) {
			/* Clear IO errors */
			reinstate_path(pp, 0);
		} else {
			LOG_MSG(4, checker_message(&pp->checker));
			if (pp->checkint != conf->max_checkint) {
				/*
				 * double the next check delay.
				 * max at conf->max_checkint
				 */
				if (pp->checkint < (conf->max_checkint / 2))
					pp->checkint = 2 * pp->checkint;
				else
					pp->checkint = conf->max_checkint;

				condlog(4, "%s: delay next check %is",
					pp->dev_t, pp->checkint);
			}
			pp->tick = pp->checkint;
		}
	}
	else if (newstate == PATH_DOWN &&
		 strlen(checker_message(&pp->checker))) {
		if (conf->log_checker_err == LOG_CHKR_ERR_ONCE)
			LOG_MSG(3, checker_message(&pp->checker));
		else
			LOG_MSG(2, checker_message(&pp->checker));
	}

	pp->state = newstate;

	/*
	 * path prio refreshing
	 */
	condlog(4, "path prio refresh");

	if (update_prio(pp, new_path_up) &&
	    (pp->mpp->pgpolicyfn == (pgpolicyfn *)group_by_prio) &&
	     pp->mpp->pgfailback == -FAILBACK_IMMEDIATE)
		update_path_groups(pp->mpp, vecs, !new_path_up);
	else if (need_switch_pathgroup(pp->mpp, 0)) {
		if (pp->mpp->pgfailback > 0 &&
		    (new_path_up || pp->mpp->failback_tick <= 0))
			pp->mpp->failback_tick =
				pp->mpp->pgfailback + 1;
		else if (pp->mpp->pgfailback == -FAILBACK_IMMEDIATE ||
			 (chkr_new_path_up && followover_should_failback(pp)))
			switch_pathgroup(pp->mpp);
	}
	return 1;
}
