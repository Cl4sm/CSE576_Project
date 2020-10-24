{
	int sock;
	struct sockaddr_nl snl;
	struct sockaddr_un sun;
	socklen_t addrlen;
	int retval;
	int rcvbufsz = 128*1024;
	int rcvsz = 0;
	int rcvszsz = sizeof(rcvsz);
	unsigned int *prcvszsz = (unsigned int *)&rcvszsz;
	const int feature_on = 1;
	/*
	 * First check whether we have a udev socket
	 */
	memset(&sun, 0x00, sizeof(struct sockaddr_un));
	sun.sun_family = AF_LOCAL;
	strcpy(&sun.sun_path[1], "/org/kernel/dm/multipath_event");
	addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(sun.sun_path+1) + 1;

	sock = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if (sock >= 0) {

		condlog(3, "reading events from udev socket.");

		/* the bind takes care of ensuring only one copy running */
		retval = bind(sock, (struct sockaddr *) &sun, addrlen);
		if (retval < 0) {
			condlog(0, "bind failed, exit");
			goto exit;
		}

		/* enable receiving of the sender credentials */
		setsockopt(sock, SOL_SOCKET, SO_PASSCRED,
			   &feature_on, sizeof(feature_on));

	} else {
		/* Fallback to read kernel netlink events */
		memset(&snl, 0x00, sizeof(struct sockaddr_nl));
		snl.nl_family = AF_NETLINK;
		snl.nl_pid = getpid();
		snl.nl_groups = 0x01;

		sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
		if (sock == -1) {
			condlog(0, "error getting socket, exit");
			return 1;
		}

		condlog(3, "reading events from kernel.");

		/*
		 * try to avoid dropping uevents, even so, this is not a guarantee,
		 * but it does help to change the netlink uevent socket's
		 * receive buffer threshold from the default value of 106,496 to
		 * the maximum value of 262,142.
		 */
		retval = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcvbufsz,
				    sizeof(rcvbufsz));

		if (retval < 0) {
			condlog(0, "error setting receive buffer size for socket, exit");
			exit(1);
		}
		retval = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcvsz, prcvszsz);
		if (retval < 0) {
			condlog(0, "error setting receive buffer size for socket, exit");
			exit(1);
		}
		condlog(3, "receive buffer size for socket is %u.", rcvsz);

		/* enable receiving of the sender credentials */
		setsockopt(sock, SOL_SOCKET, SO_PASSCRED,
			   &feature_on, sizeof(feature_on));

		retval = bind(sock, (struct sockaddr *) &snl,
			      sizeof(struct sockaddr_nl));
		if (retval < 0) {
			condlog(0, "bind failed, exit");
			goto exit;
		}
	}

	while (1) {
		int i;
		char *pos;
		size_t bufpos;
		ssize_t buflen;
		struct uevent *uev;
		char *buffer;
		struct msghdr smsg;
		struct iovec iov;
		char cred_msg[CMSG_SPACE(sizeof(struct ucred))];
		struct cmsghdr *cmsg;
		struct ucred *cred;
		static char buf[HOTPLUG_BUFFER_SIZE + OBJECT_SIZE];

		memset(buf, 0x00, sizeof(buf));
		iov.iov_base = &buf;
		iov.iov_len = sizeof(buf);
		memset (&smsg, 0x00, sizeof(struct msghdr));
		smsg.msg_iov = &iov;
		smsg.msg_iovlen = 1;
		smsg.msg_control = cred_msg;
		smsg.msg_controllen = sizeof(cred_msg);

		buflen = recvmsg(sock, &smsg, 0);
		if (buflen < 0) {
			if (errno != EINTR)
				condlog(0, "error receiving message, errno %d", errno);
			continue;
		}

		cmsg = CMSG_FIRSTHDR(&smsg);
		if (cmsg == NULL || cmsg->cmsg_type != SCM_CREDENTIALS) {
			condlog(3, "no sender credentials received, message ignored");
			continue;
		}

		cred = (struct ucred *)CMSG_DATA(cmsg);
		if (cred->uid != 0) {
			condlog(3, "sender uid=%d, message ignored", cred->uid);
			continue;
		}

		/* skip header */
		bufpos = strlen(buf) + 1;
		if (bufpos < sizeof("a@/d") || bufpos >= sizeof(buf)) {
			condlog(3, "invalid message length");
			continue;
		}

		/* check message header */
		if (strstr(buf, "@/") == NULL) {
			condlog(3, "unrecognized message header");
			continue;
		}
		if ((size_t)buflen > sizeof(buf)-1) {
			condlog(2, "buffer overflow for received uevent");
			buflen = sizeof(buf)-1;
		}

		uev = alloc_uevent();

		if (!uev) {
			condlog(1, "lost uevent, oom");
			continue;
		}

		if ((size_t)buflen > sizeof(buf)-1)
			buflen = sizeof(buf)-1;

		/*
		 * Copy the shared receive buffer contents to buffer private
		 * to this uevent so we can immediately reuse the shared buffer.
		 */
		memcpy(uev->buffer, buf, HOTPLUG_BUFFER_SIZE + OBJECT_SIZE);
		buffer = uev->buffer;
		buffer[buflen] = '\0';

		/* save start of payload */
		bufpos = strlen(buffer) + 1;

		/* action string */
		uev->action = buffer;
		pos = strchr(buffer, '@');
		if (!pos) {
			condlog(3, "bad action string '%s'", buffer);
			continue;
		}
		pos[0] = '\0';

		/* sysfs path */
		uev->devpath = &pos[1];

		/* hotplug events have the environment attached - reconstruct envp[] */
		for (i = 0; (bufpos < (size_t)buflen) && (i < HOTPLUG_NUM_ENVP-1); i++) {
			int keylen;
			char *key;

			key = &buffer[bufpos];
			keylen = strlen(key);
			uev->envp[i] = key;
			/* Filter out sequence number */
			if (strncmp(key, "SEQNUM=", 7) == 0) {
				char *eptr;

				uev->seqnum = strtoul(key + 7, &eptr, 10);
				if (eptr == key + 7)
					uev->seqnum = -1;
			}
			bufpos += keylen + 1;
		}
		uev->envp[i] = NULL;

		condlog(3, "uevent %ld '%s' from '%s'", uev->seqnum,
			uev->action, uev->devpath);
		uev->kernel = strrchr(uev->devpath, '/');
		if (uev->kernel)
			uev->kernel++;

		/* print payload environment */
		for (i = 0; uev->envp[i] != NULL; i++)
			condlog(5, "%s", uev->envp[i]);

		/*
		 * Queue uevent and poke service pthread.
		 */
		pthread_mutex_lock(uevq_lockp);
		list_add_tail(&uev->node, &uevq);
		pthread_cond_signal(uev_condp);
		pthread_mutex_unlock(uevq_lockp);
	}

exit:
	close(sock);
	return 1;
}
