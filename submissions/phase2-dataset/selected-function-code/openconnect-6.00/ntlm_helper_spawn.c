static int ntlm_helper_spawn(struct openconnect_info *vpninfo, struct oc_text_buf *buf)
{
	char *username;
	int pipefd[2];
	pid_t pid;
	char helperbuf[4096];
	int len;

	if (access("/usr/bin/ntlm_auth", X_OK))
		return -errno;

	username = vpninfo->proxy_user;
	if (!username)
		username = getenv("NTLMUSER");
	if (!username)
		username = getenv("USER");
	if (!username)
		return -EINVAL;

#ifdef SOCK_CLOEXEC
	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, pipefd))
#endif
	{
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd))
			return -errno;
		set_fd_cloexec(pipefd[0]);
		set_fd_cloexec(pipefd[1]);
	}
	pid = fork();
	if (pid == -1)
		return -errno;

	if (!pid) {
		int i;
		char *p;
		const char *argv[9];

		/* Fork again to detach grandchild */
		if (fork())
			exit(1);

		close(pipefd[1]);
		/* The duplicated fd does not have O_CLOEXEC */
		dup2(pipefd[0], 0);
		dup2(pipefd[0], 1);
		/* Should we leave stderr open? */
		for (i = 3; i < 1024 ; i++)
			close(i);


		i = 0;
		argv[i++] = "/usr/bin/ntlm_auth";
		argv[i++] = "--helper-protocol";
		argv[i++] = "ntlmssp-client-1";
		argv[i++] = "--use-cached-creds";
		argv[i++] = "--username";
		p = strchr(username, '\\');
		if (p) {
			argv[i++] = p+1;
			argv[i++] = "--domain";
			argv[i++] = strndup(username, p - username);
		} else
			argv[i++] = username;
		argv[i++] = NULL;

		execv(argv[0], (char **)argv);
		exit(1);
	}
	waitpid(pid, NULL, 0);
	close(pipefd[0]);

	if (write(pipefd[1], "YR\n", 3) != 3) {
		close(pipefd[1]);
		return -EIO;
	}

	len = read(pipefd[1], helperbuf, sizeof(helperbuf));
	if (len < 4 || helperbuf[0] != 'Y' || helperbuf[1] != 'R' ||
	    helperbuf[2] != ' ' || helperbuf[len - 1] != '\n') {
		close(pipefd[1]);
		return -EIO;
	}
	helperbuf[len - 1] = 0;
	buf_append(buf, "Proxy-Authorization: NTLM %s\r\n", helperbuf + 3);
	vpninfo->ntlm_helper_fd = pipefd[1];
	return 0;
}