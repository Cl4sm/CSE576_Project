static void handle_signal(int sig)
{
	char cmd;

	switch (sig) {
	case SIGINT:
		cmd = OC_CMD_CANCEL;
		break;
	case SIGHUP:
		cmd = OC_CMD_DETACH;
		break;
	case SIGUSR2:
	default:
		cmd = OC_CMD_PAUSE;
		break;
	}

	if (write(sig_cmd_fd, &cmd, 1) < 0) {
	/* suppress warn_unused_result */
	}
}