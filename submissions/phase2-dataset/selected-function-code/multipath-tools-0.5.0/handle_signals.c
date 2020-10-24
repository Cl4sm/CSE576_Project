handle_signals(void)
{
	if (reconfig_sig && running_state == DAEMON_RUNNING) {
		condlog(2, "reconfigure (signal)");
		pthread_cleanup_push(cleanup_lock,
				&gvecs->lock);
		lock(gvecs->lock);
		pthread_testcancel();
		reconfigure(gvecs);
		lock_cleanup_pop(gvecs->lock);
	}
	if (log_reset_sig) {
		condlog(2, "reset log (signal)");
		pthread_mutex_lock(&logq_lock);
		log_reset("multipathd");
		pthread_mutex_unlock(&logq_lock);
	}
	reconfig_sig = 0;
	log_reset_sig = 0;
}
