int uevent_listen(struct udev *udev)
{
	int err;
	struct udev_monitor *monitor = NULL;
	int fd, socket_flags;
	int need_failback = 1;
	/*
	 * Queue uevents for service by dedicated thread so that the uevent
	 * listening thread does not block on multipathd locks (vecs->lock)
	 * thereby not getting to empty the socket's receive buffer queue
	 * often enough.
	 */
	if (!udev) {
		condlog(1, "no udev context");
		return 1;
	}
	udev_ref(udev);
	pthread_cleanup_push(uevq_stop, udev);

	monitor = udev_monitor_new_from_netlink(udev, "udev");
	if (!monitor) {
		condlog(2, "failed to create udev monitor");
		err = 2;
		goto out;
	}
#ifdef LIBUDEV_API_RECVBUF
	if (udev_monitor_set_receive_buffer_size(monitor, 128 * 1024 * 1024))
		condlog(2, "failed to increase buffer size");
#endif
	fd = udev_monitor_get_fd(monitor);
	if (fd < 0) {
		condlog(2, "failed to get monitor fd");
		goto out;
	}
	socket_flags = fcntl(fd, F_GETFL);
	if (socket_flags < 0) {
		condlog(2, "failed to get monitor socket flags : %s",
			strerror(errno));
		goto out;
	}
	if (fcntl(fd, F_SETFL, socket_flags & ~O_NONBLOCK) < 0) {
		condlog(2, "failed to set monitor socket flags : %s",
			strerror(errno));
		goto out;
	}
	err = udev_monitor_filter_add_match_subsystem_devtype(monitor, "block",
							      NULL);
	if (err)
		condlog(2, "failed to create filter : %s", strerror(-err));
	err = udev_monitor_enable_receiving(monitor);
	if (err) {
		condlog(2, "failed to enable receiving : %s", strerror(-err));
		goto out;
	}
	while (1) {
		int i = 0;
		char *pos, *end;
		struct uevent *uev;
		struct udev_device *dev;
                struct udev_list_entry *list_entry;

		dev = udev_monitor_receive_device(monitor);
		if (!dev) {
			condlog(0, "failed getting udev device");
			continue;
		}

		uev = alloc_uevent();
		if (!uev) {
			udev_device_unref(dev);
			condlog(1, "lost uevent, oom");
			continue;
		}
		pos = uev->buffer;
		end = pos + HOTPLUG_BUFFER_SIZE + OBJECT_SIZE - 1;
		udev_list_entry_foreach(list_entry, udev_device_get_properties_list_entry(dev)) {
			const char *name, *value;
			int bytes;

			name = udev_list_entry_get_name(list_entry);
			if (!name)
				name = "(null)";
			value = udev_list_entry_get_value(list_entry);
			if (!value)
				value = "(null)";
			bytes = snprintf(pos, end - pos, "%s=%s", name,
					value);
			if (pos + bytes >= end) {
				condlog(2, "buffer overflow for uevent");
				break;
			}
			uev->envp[i] = pos;
			pos += bytes;
			*pos = '\0';
			pos++;
			if (strcmp(name, "DEVPATH") == 0)
				uev->devpath = uev->envp[i] + 8;
			if (strcmp(name, "ACTION") == 0)
				uev->action = uev->envp[i] + 7;
			i++;
			if (i == HOTPLUG_NUM_ENVP - 1)
				break;
		}
		uev->udev = dev;
		uev->envp[i] = NULL;

		condlog(3, "uevent '%s' from '%s'", uev->action, uev->devpath);
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
	need_failback = 0;
out:
	if (monitor)
		udev_monitor_unref(monitor);
	if (need_failback)
		err = failback_listen();
	pthread_cleanup_pop(1);
	return err;
}
