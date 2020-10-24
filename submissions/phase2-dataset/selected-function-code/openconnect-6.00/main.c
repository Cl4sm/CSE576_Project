int main(int argc, char **argv)
{
	struct openconnect_info *vpninfo;
	char *urlpath = NULL;
	char *proxy = getenv("https_proxy");
	char *vpnc_script = NULL, *ifname = NULL;
	const struct oc_ip_info *ip_info;
	int autoproxy = 0;
	int opt;
	char *pidfile = NULL;
	int use_dtls = 1;
	FILE *fp = NULL;
	char *config_arg;
	char *token_str = NULL;
	oc_token_mode_t token_mode = OC_TOKEN_MODE_NONE;
	int reconnect_timeout = 300;
	int ret;
#ifndef _WIN32
	struct sigaction sa;
	struct utsname utsbuf;
	uid_t uid = getuid();
	int use_syslog = 0;
	int script_tun = 0;
#endif

#ifdef ENABLE_NLS
	bindtextdomain("openconnect", LOCALEDIR);
	setlocale(LC_ALL, "");
#endif

	if (strcmp(openconnect_version_str, openconnect_binary_version)) {
		fprintf(stderr, _("WARNING: This version of openconnect is %s but\n"
				  "         the libopenconnect library is %s\n"),
			openconnect_binary_version, openconnect_version_str);
	}

	openconnect_init_ssl();

	vpninfo = openconnect_vpninfo_new((char *)"Open AnyConnect VPN Agent",
		validate_peer_cert, NULL, process_auth_form_cb, write_progress, NULL);
	if (!vpninfo) {
		fprintf(stderr, _("Failed to allocate vpninfo structure\n"));
		exit(1);
	}

	vpninfo->cbdata = vpninfo;
#ifdef _WIN32
	set_default_vpncscript();
#else
	if (!uname(&utsbuf)) {
		free(vpninfo->localname);
		vpninfo->localname = xstrdup(utsbuf.nodename);
	}
#endif

	while ((opt = next_option(argc, argv, &config_arg))) {

		if (opt < 0)
			break;

		switch (opt) {
#ifndef _WIN32
		case 'b':
			background = 1;
			break;
		case 'l':
			use_syslog = 1;
			break;
		case 'S':
			script_tun = 1;
			break;
		case 'U': {
			char *strend;
			uid = strtol(config_arg, &strend, 0);
			if (strend[0]) {
				struct passwd *pw = getpwnam(config_arg);
				if (!pw) {
					fprintf(stderr, _("Invalid user \"%s\"\n"),
						config_arg);
					exit(1);
				}
				uid = pw->pw_uid;
			}
			break;
		}
		case OPT_CSD_USER: {
			char *strend;
			vpninfo->uid_csd = strtol(config_arg, &strend, 0);
			if (strend[0]) {
				struct passwd *pw = getpwnam(config_arg);
				if (!pw) {
					fprintf(stderr, _("Invalid user \"%s\"\n"),
						config_arg);
					exit(1);
				}
				vpninfo->uid_csd = pw->pw_uid;
			}
			vpninfo->uid_csd_given = 1;
			break;
		}
		case OPT_CSD_WRAPPER:
			vpninfo->csd_wrapper = keep_config_arg();
			break;
#endif /* !_WIN32 */
		case OPT_CONFIGFILE:
			if (config_file) {
				fprintf(stderr, _("Cannot use 'config' option inside config file\n"));
				exit(1);
			}
			config_file = fopen(config_arg, "r");
			if (!config_file) {
				fprintf(stderr, _("Cannot open config file '%s': %s\n"),
					config_arg, strerror(errno));
				exit(1);
			}
			config_line_num = 1;
			/* The next option will come from the file... */
			break;
		case OPT_CAFILE:
			openconnect_set_cafile(vpninfo, xstrdup(config_arg));
			break;
		case OPT_PIDFILE:
			pidfile = keep_config_arg();
			break;
		case OPT_PFS:
			openconnect_set_pfs(vpninfo, 1);
			break;
		case OPT_SERVERCERT:
			openconnect_set_server_cert_sha1(vpninfo, xstrdup(config_arg));
			break;
		case OPT_NO_DTLS:
			use_dtls = 0;
			break;
		case OPT_COOKIEONLY:
			cookieonly = 1;
			break;
		case OPT_PRINTCOOKIE:
			cookieonly = 2;
			break;
		case OPT_AUTHENTICATE:
			cookieonly = 3;
			break;
		case OPT_COOKIE_ON_STDIN:
			read_stdin(&vpninfo->cookie, 0);
			/* If the cookie is empty, ignore it */
			if (!*vpninfo->cookie)
				vpninfo->cookie = NULL;
			break;
		case OPT_PASSWORD_ON_STDIN:
			read_stdin(&password, 0);
			break;
		case OPT_NO_PASSWD:
			vpninfo->nopasswd = 1;
			break;
		case OPT_NO_XMLPOST:
			openconnect_set_xmlpost(vpninfo, 0);
			break;
		case OPT_NON_INTER:
			non_inter = 1;
			break;
		case OPT_RECONNECT_TIMEOUT:
			reconnect_timeout = atoi(config_arg);
			break;
		case OPT_DTLS_CIPHERS:
			vpninfo->dtls_ciphers = keep_config_arg();
			break;
		case OPT_AUTHGROUP:
			authgroup = keep_config_arg();
			break;
		case 'C':
			vpninfo->cookie = strdup(config_arg);
			break;
		case 'c':
			vpninfo->cert = strdup(config_arg);
			break;
		case 'e':
			vpninfo->cert_expire_warning = 86400 * atoi(config_arg);
			break;
		case 'k':
			vpninfo->sslkey = strdup(config_arg);
			break;
		case 'd':
			vpninfo->deflate = 1;
			break;
		case 'D':
			vpninfo->deflate = 0;
			break;
		case 'g':
			free(urlpath);
			urlpath = strdup(config_arg);
			break;
		case 'h':
			usage();
		case 'i':
			ifname = xstrdup(config_arg);
			break;
		case 'm': {
			int mtu = atol(config_arg);
			if (mtu < 576) {
				fprintf(stderr, _("MTU %d too small\n"), mtu);
				mtu = 576;
			}
			openconnect_set_reqmtu(vpninfo, mtu);
			break;
		}
		case OPT_BASEMTU:
			vpninfo->basemtu = atol(config_arg);
			if (vpninfo->basemtu < 576) {
				fprintf(stderr, _("MTU %d too small\n"), vpninfo->basemtu);
				vpninfo->basemtu = 576;
			}
			break;
		case 'p':
			vpninfo->cert_password = strdup(config_arg);
			break;
		case 'P':
			proxy = keep_config_arg();
			autoproxy = 0;
			break;
		case OPT_PROXY_AUTH:
			openconnect_set_proxy_auth(vpninfo, xstrdup(config_arg));
			break;
		case OPT_NO_PROXY:
			autoproxy = 0;
			proxy = NULL;
			break;
		case OPT_LIBPROXY:
			autoproxy = 1;
			proxy = NULL;
			break;
		case OPT_NO_HTTP_KEEPALIVE:
			fprintf(stderr,
				_("Disabling all HTTP connection re-use due to --no-http-keepalive option.\n"
				  "If this helps, please report to <openconnect-devel@lists.infradead.org>.\n"));
			vpninfo->no_http_keepalive = 1;
			break;
		case OPT_NO_CERT_CHECK:
			nocertcheck = 1;
			break;
		case 's':
			vpnc_script = xstrdup(config_arg);
			break;
		case 'u':
			free(username);
			username = strdup(config_arg);
			break;
		case OPT_DISABLE_IPV6:
			vpninfo->disable_ipv6 = 1;
			break;
		case 'Q':
			vpninfo->max_qlen = atol(config_arg);
			if (!vpninfo->max_qlen) {
				fprintf(stderr, _("Queue length zero not permitted; using 1\n"));
				vpninfo->max_qlen = 1;
			}
			break;
		case 'q':
			verbose = PRG_ERR;
			break;
		case OPT_DUMP_HTTP:
			vpninfo->dump_http_traffic = 1;
			break;
		case 'v':
			verbose++;
			break;
		case 'V':
			printf(_("OpenConnect version %s\n"), openconnect_version_str);
			print_build_opts();
			exit(0);
		case 'x':
			vpninfo->xmlconfig = keep_config_arg();
			vpninfo->write_new_config = write_new_config;
			break;
		case OPT_KEY_PASSWORD_FROM_FSID:
			do_passphrase_from_fsid = 1;
			break;
		case OPT_USERAGENT:
			free(vpninfo->useragent);
			vpninfo->useragent = strdup(config_arg);
			break;
		case OPT_FORCE_DPD:
			openconnect_set_dpd(vpninfo, atoi(config_arg));
			break;
		case OPT_DTLS_LOCAL_PORT:
			vpninfo->dtls_local_port = atoi(config_arg);
			break;
		case OPT_TOKEN_MODE:
			if (strcasecmp(config_arg, "rsa") == 0) {
				token_mode = OC_TOKEN_MODE_STOKEN;
			} else if (strcasecmp(config_arg, "totp") == 0) {
				token_mode = OC_TOKEN_MODE_TOTP;
			} else if (strcasecmp(config_arg, "hotp") == 0) {
				token_mode = OC_TOKEN_MODE_HOTP;
			} else {
				fprintf(stderr, _("Invalid software token mode \"%s\"\n"),
					config_arg);
				exit(1);
			}
			break;
		case OPT_TOKEN_SECRET:
			token_str = keep_config_arg();
			break;
		case OPT_OS:
			if (openconnect_set_reported_os(vpninfo, config_arg)) {
				fprintf(stderr, _("Invalid OS identity \"%s\"\n"),
					config_arg);
				exit(1);
			}
			if (!strcmp(config_arg, "android") || !strcmp(config_arg, "apple-ios")) {
				/* generic defaults */
				openconnect_set_mobile_info(vpninfo,
					xstrdup("1.0"),
					xstrdup(config_arg),
					xstrdup("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
			}
			break;
		case OPT_TIMESTAMP:
			timestamp = 1;
			break;
		default:
			usage();
		}
	}

	if (optind < argc - 1) {
		fprintf(stderr, _("Too many arguments on command line\n"));
		usage();
	} else if (optind > argc - 1) {
		fprintf(stderr, _("No server specified\n"));
		usage();
	}

	if (!vpninfo->sslkey)
		vpninfo->sslkey = vpninfo->cert;

	if (vpninfo->dump_http_traffic && verbose < PRG_DEBUG)
		verbose = PRG_DEBUG;

	vpninfo->progress = write_progress;

	if (autoproxy) {
#ifdef LIBPROXY_HDR
		vpninfo->proxy_factory = px_proxy_factory_new();
#else
		fprintf(stderr, _("This version of openconnect was built without libproxy support\n"));
		exit(1);
#endif
	}

	if (token_mode != OC_TOKEN_MODE_NONE)
		init_token(vpninfo, token_mode, token_str);

	if (proxy && openconnect_set_http_proxy(vpninfo, strdup(proxy)))
		exit(1);

#ifndef _WIN32
	if (use_syslog) {
		openlog("openconnect", LOG_PID, LOG_DAEMON);
		vpninfo->progress = syslog_progress;
	}

	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = handle_signal;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGHUP, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
#endif /* !_WIN32 */

	sig_cmd_fd = openconnect_setup_cmd_pipe(vpninfo);
	if (sig_cmd_fd < 0) {
		fprintf(stderr, _("Error opening cmd pipe\n"));
		exit(1);
	}

	if (vpninfo->sslkey && do_passphrase_from_fsid)
		openconnect_passphrase_from_fsid(vpninfo);

	if (config_lookup_host(vpninfo, argv[optind]))
		exit(1);

	if (!vpninfo->hostname) {
		char *url = strdup(argv[optind]);

		if (openconnect_parse_url(vpninfo, url))
			exit(1);

		free(url);
	}

	/* Historically, the path in the URL superseded the one in the
	 * --usergroup argument, just because of the order in which they
	 * were processed. Preserve that behaviour. */
	if (urlpath && !vpninfo->urlpath) {
		vpninfo->urlpath = urlpath;
		urlpath = NULL;
	}
	free(urlpath);

	if (!vpninfo->cookie && openconnect_obtain_cookie(vpninfo)) {
		if (vpninfo->csd_scriptname) {
			unlink(vpninfo->csd_scriptname);
			vpninfo->csd_scriptname = NULL;
		}
		fprintf(stderr, _("Failed to obtain WebVPN cookie\n"));
		exit(1);
	}

	if (cookieonly == 3) {
		/* --authenticate */
		printf("COOKIE='%s'\n", vpninfo->cookie);
		printf("HOST='%s'\n", openconnect_get_hostname(vpninfo));
		if (vpninfo->peer_cert) {
			char buf[41] = {0, };
			openconnect_get_cert_sha1(vpninfo, vpninfo->peer_cert, buf);
			printf("FINGERPRINT='%s'\n", buf);
		}
		openconnect_vpninfo_free(vpninfo);
		exit(0);
	} else if (cookieonly) {
		printf("%s\n", vpninfo->cookie);
		if (cookieonly == 1) {
			/* We use cookieonly=2 for 'print it and continue' */
			openconnect_vpninfo_free(vpninfo);
			exit(0);
		}
	}
	if (openconnect_make_cstp_connection(vpninfo)) {
		fprintf(stderr, _("Creating SSL connection failed\n"));
		openconnect_vpninfo_free(vpninfo);
		exit(1);
	}

	if (!vpnc_script)
		vpnc_script = xstrdup(default_vpncscript);
#ifndef _WIN32
	if (script_tun) {
		if (openconnect_setup_tun_script(vpninfo, vpnc_script)) {
			fprintf(stderr, _("Set up tun script failed\n"));
			openconnect_vpninfo_free(vpninfo);
			exit(1);
		}
	} else
#endif
	if (openconnect_setup_tun_device(vpninfo, vpnc_script, ifname)) {
		fprintf(stderr, _("Set up tun device failed\n"));
		openconnect_vpninfo_free(vpninfo);
		exit(1);
	}

#ifndef _WIN32
	if (uid != getuid()) {
		if (setuid(uid)) {
			fprintf(stderr, _("Failed to set uid %ld\n"),
				(long)uid);
			openconnect_vpninfo_free(vpninfo);
			exit(1);
		}
	}
#endif

	if (use_dtls && openconnect_setup_dtls(vpninfo, 60))
		fprintf(stderr, _("Set up DTLS failed; using SSL instead\n"));

	openconnect_get_ip_info(vpninfo, &ip_info, NULL, NULL);
	vpn_progress(vpninfo, PRG_INFO,
		     _("Connected %s as %s%s%s, using %s\n"), openconnect_get_ifname(vpninfo),
		     ip_info->addr?:"",
		     (ip_info->netmask6 && ip_info->addr) ? " + " : "",
		     ip_info->netmask6 ? : "",
		     (vpninfo->dtls_state != DTLS_CONNECTED) ?
		     (vpninfo->deflate ? "SSL + deflate" : "SSL")
		     : "DTLS");

	if (!vpninfo->vpnc_script) {
		vpn_progress(vpninfo, PRG_INFO,
			     _("No --script argument provided; DNS and routing are not configured\n"));
		vpn_progress(vpninfo, PRG_INFO,
			     _("See http://www.infradead.org/openconnect/vpnc-script.html\n"));
	}

#ifndef _WIN32
	if (background) {
		int pid;

		/* Open the pidfile before forking, so we can report errors
		   more sanely. It's *possible* that we'll fail to write to
		   it, but very unlikely. */
		if (pidfile != NULL) {
			fp = fopen(pidfile, "w");
			if (!fp) {
				fprintf(stderr, _("Failed to open '%s' for write: %s\n"),
					pidfile, strerror(errno));
				openconnect_vpninfo_free(vpninfo);
				exit(1);
			}
		}
		if ((pid = fork())) {
			if (fp) {
				fprintf(fp, "%d\n", pid);
				fclose(fp);
			}
			vpn_progress(vpninfo, PRG_INFO,
				     _("Continuing in background; pid %d\n"),
				     pid);
			openconnect_vpninfo_free(vpninfo);
			exit(0);
		}
		if (fp)
			fclose(fp);
	}
#endif

	while (1) {
		ret = openconnect_mainloop(vpninfo, reconnect_timeout, RECONNECT_INTERVAL_MIN);
		if (ret)
			break;

		vpn_progress(vpninfo, PRG_INFO, _("User requested reconnect\n"));
	}

	if (fp)
		unlink(pidfile);

	switch (ret) {
	case -EPERM:
		vpn_progress(vpninfo, PRG_ERR, _("Cookie was rejected on reconnection; exiting.\n"));
		ret = 2;
		break;
	case -EPIPE:
		vpn_progress(vpninfo, PRG_ERR, _("Session terminated by server; exiting.\n"));
		ret = 1;
		break;
	case -EINTR:
		vpn_progress(vpninfo, PRG_INFO, _("User canceled (SIGINT); exiting.\n"));
		ret = 0;
		break;
	case -ECONNABORTED:
		vpn_progress(vpninfo, PRG_INFO, _("User detached from session (SIGHUP); exiting.\n"));
		ret = 0;
		break;
	default:
		vpn_progress(vpninfo, PRG_ERR, _("Unknown error; exiting.\n"));
		ret = 1;
		break;
	}

	openconnect_vpninfo_free(vpninfo);
	exit(ret);
}