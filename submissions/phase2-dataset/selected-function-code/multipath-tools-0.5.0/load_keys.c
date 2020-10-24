int
load_keys (void)
{
	int r = 0;
	keys = vector_alloc();

	if (!keys)
		return 1;

	r += add_key(keys, "list", LIST, 0);
	r += add_key(keys, "show", LIST, 0);
	r += add_key(keys, "add", ADD, 0);
	r += add_key(keys, "remove", DEL, 0);
	r += add_key(keys, "del", DEL, 0);
	r += add_key(keys, "switch", SWITCH, 0);
	r += add_key(keys, "switchgroup", SWITCH, 0);
	r += add_key(keys, "suspend", SUSPEND, 0);
	r += add_key(keys, "resume", RESUME, 0);
	r += add_key(keys, "reinstate", REINSTATE, 0);
	r += add_key(keys, "fail", FAIL, 0);
	r += add_key(keys, "resize", RESIZE, 0);
	r += add_key(keys, "reset", RESET, 0);
	r += add_key(keys, "reload", RELOAD, 0);
	r += add_key(keys, "forcequeueing", FORCEQ, 0);
	r += add_key(keys, "disablequeueing", DISABLEQ, 0);
	r += add_key(keys, "restorequeueing", RESTOREQ, 0);
	r += add_key(keys, "paths", PATHS, 0);
	r += add_key(keys, "maps", MAPS, 0);
	r += add_key(keys, "multipaths", MAPS, 0);
	r += add_key(keys, "path", PATH, 1);
	r += add_key(keys, "map", MAP, 1);
	r += add_key(keys, "multipath", MAP, 1);
	r += add_key(keys, "group", GROUP, 1);
	r += add_key(keys, "reconfigure", RECONFIGURE, 0);
	r += add_key(keys, "daemon", DAEMON, 0);
	r += add_key(keys, "status", STATUS, 0);
	r += add_key(keys, "stats", STATS, 0);
	r += add_key(keys, "topology", TOPOLOGY, 0);
	r += add_key(keys, "config", CONFIG, 0);
	r += add_key(keys, "blacklist", BLACKLIST, 0);
	r += add_key(keys, "devices", DEVICES, 0);
	r += add_key(keys, "format", FMT, 1);
	r += add_key(keys, "wildcards", WILDCARDS, 0);
	r += add_key(keys, "quit", QUIT, 0);
	r += add_key(keys, "exit", QUIT, 0);
	r += add_key(keys, "shutdown", SHUTDOWN, 0);
	r += add_key(keys, "getprstatus", GETPRSTATUS, 0);
	r += add_key(keys, "setprstatus", SETPRSTATUS, 0);
	r += add_key(keys, "unsetprstatus", UNSETPRSTATUS, 0);

	if (r) {
		free_keys(keys);
		keys = NULL;
		return 1;
	}
	return 0;
}
