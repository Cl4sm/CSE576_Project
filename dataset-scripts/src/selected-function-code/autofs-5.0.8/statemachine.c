static void *statemachine(void *arg)
{
	sigset_t signalset;
	int sig;

	memcpy(&signalset, &block_sigs, sizeof(signalset));
	sigdelset(&signalset, SIGCHLD);
	sigdelset(&signalset, SIGCONT);

	while (1) {
		sigwait(&signalset, &sig);

		switch (sig) {
		case SIGTERM:
		case SIGINT:
		case SIGUSR2:
			master_mutex_lock();
			if (list_empty(&master_list->completed)) {
				if (list_empty(&master_list->mounts)) {
					master_mutex_unlock();
					return NULL;
				}
			} else {
				if (master_done(master_list)) {
					master_mutex_unlock();
					return NULL;
				}
				master_mutex_unlock();
				break;
			}
			master_mutex_unlock();

		case SIGUSR1:
			do_signals(master_list, sig);
			break;

		case SIGHUP:
			do_hup_signal(master_list, time(NULL));
			break;

		default:
			logerr("got unexpected signal %d!", sig);
			continue;
		}
	}
}