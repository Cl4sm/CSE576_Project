static int run_csd_script(struct openconnect_info *vpninfo, char *buf, int buflen)
{
#ifdef _WIN32
	vpn_progress(vpninfo, PRG_ERR,
		     _("Error: Running the 'Cisco Secure Desktop' trojan on Windows is not yet implemented.\n"));
	return -EPERM;
#else
	char fname[64];
	int fd, ret;

	if (!vpninfo->csd_wrapper && !buflen) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error: Server asked us to run CSD hostscan.\n"
			       "You need to provide a suitable --csd-wrapper argument.\n"));
		return -EINVAL;
	}

	if (!vpninfo->uid_csd_given && !vpninfo->csd_wrapper) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error: Server asked us to download and run a 'Cisco Secure Desktop' trojan.\n"
			       "This facility is disabled by default for security reasons, so you may wish to enable it.\n"));
		return -EPERM;
	}

#ifndef __linux__
	vpn_progress(vpninfo, PRG_INFO,
		     _("Trying to run Linux CSD trojan script.\n"));
#endif

	fname[0] = 0;
	if (buflen) {
		char *tmpdir = getenv("TMPDIR");
		snprintf(fname, 64, "%s/csdXXXXXX", tmpdir ? tmpdir : "/tmp");
		fd = mkstemp(fname);
		if (fd < 0) {
			int err = -errno;
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to open temporary CSD script file: %s\n"),
				     strerror(errno));
			return err;
		}

		ret = write(fd, (void *)buf, buflen);
		if (ret != buflen) {
			int err = -errno;
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to write temporary CSD script file: %s\n"),
				     strerror(errno));
			return err;
		}
		fchmod(fd, 0755);
		close(fd);
	}

	if (!fork()) {
		char scertbuf[MD5_SIZE * 2 + 1];
		char ccertbuf[MD5_SIZE * 2 + 1];
		char *csd_argv[32];
		int i = 0;

		if (vpninfo->uid_csd_given && vpninfo->uid_csd != getuid()) {
			struct passwd *pw;

			if (setuid(vpninfo->uid_csd)) {
				fprintf(stderr, _("Failed to set uid %ld\n"),
					(long)vpninfo->uid_csd);
				exit(1);
			}
			if (!(pw = getpwuid(vpninfo->uid_csd))) {
				fprintf(stderr, _("Invalid user uid=%ld\n"),
					(long)vpninfo->uid_csd);
				exit(1);
			}
			setenv("HOME", pw->pw_dir, 1);
			if (chdir(pw->pw_dir)) {
				fprintf(stderr, _("Failed to change to CSD home directory '%s': %s\n"),
					pw->pw_dir, strerror(errno));
				exit(1);
			}
		}
		if (getuid() == 0 && !vpninfo->csd_wrapper) {
			fprintf(stderr, _("Warning: you are running insecure "
					  "CSD code with root privileges\n"
					  "\t Use command line option \"--csd-user\"\n"));
		}
		/* Spurious stdout output from the CSD trojan will break both
		   the NM tool and the various cookieonly modes. */
		dup2(2, 1);
		if (vpninfo->csd_wrapper)
			csd_argv[i++] = vpninfo->csd_wrapper;
		csd_argv[i++] = fname;
		csd_argv[i++] = (char *)"-ticket";
		if (asprintf(&csd_argv[i++], "\"%s\"", vpninfo->csd_ticket) == -1)
			goto out;
		csd_argv[i++] = (char *)"-stub";
		csd_argv[i++] = (char *)"\"0\"";
		csd_argv[i++] = (char *)"-group";
		if (asprintf(&csd_argv[i++], "\"%s\"", vpninfo->authgroup?:"") == -1)
			goto out;

		openconnect_local_cert_md5(vpninfo, ccertbuf);
		scertbuf[0] = 0;
		get_cert_md5_fingerprint(vpninfo, vpninfo->peer_cert, scertbuf);
		csd_argv[i++] = (char *)"-certhash";
		if (asprintf(&csd_argv[i++], "\"%s:%s\"", scertbuf, ccertbuf) == -1)
			goto out;

		csd_argv[i++] = (char *)"-url";
		if (asprintf(&csd_argv[i++], "\"https://%s%s\"", vpninfo->hostname, vpninfo->csd_starturl) == -1)
			goto out;

		csd_argv[i++] = (char *)"-langselen";
		csd_argv[i++] = NULL;

		if (setenv("CSD_TOKEN", vpninfo->csd_token, 1))
			goto out;
		if (setenv("CSD_HOSTNAME", vpninfo->hostname, 1))
			goto out;

		execv(csd_argv[0], csd_argv);

out:
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to exec CSD script %s\n"), csd_argv[0]);
		exit(1);
	}

	free(vpninfo->csd_stuburl);
	vpninfo->csd_stuburl = NULL;
	free(vpninfo->urlpath);
	vpninfo->urlpath = strdup(vpninfo->csd_waiturl +
				  (vpninfo->csd_waiturl[0] == '/' ? 1 : 0));
	free(vpninfo->csd_waiturl);
	vpninfo->csd_waiturl = NULL;
	vpninfo->csd_scriptname = strdup(fname);

	http_add_cookie(vpninfo, "sdesktop", vpninfo->csd_token);
	return 0;
#endif /* !_WIN32 */
}