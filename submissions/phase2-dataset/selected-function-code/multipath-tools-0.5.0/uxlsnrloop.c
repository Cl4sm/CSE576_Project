static void *
uxlsnrloop (void * ap)
{
	if (cli_init())
		return NULL;

	set_handler_callback(LIST+PATHS, cli_list_paths);
	set_handler_callback(LIST+PATHS+FMT, cli_list_paths_fmt);
	set_handler_callback(LIST+MAPS, cli_list_maps);
	set_handler_callback(LIST+STATUS, cli_list_status);
	set_handler_callback(LIST+DAEMON, cli_list_daemon);
	set_handler_callback(LIST+MAPS+STATUS, cli_list_maps_status);
	set_handler_callback(LIST+MAPS+STATS, cli_list_maps_stats);
	set_handler_callback(LIST+MAPS+FMT, cli_list_maps_fmt);
	set_handler_callback(LIST+MAPS+TOPOLOGY, cli_list_maps_topology);
	set_handler_callback(LIST+TOPOLOGY, cli_list_maps_topology);
	set_handler_callback(LIST+MAP+TOPOLOGY, cli_list_map_topology);
	set_handler_callback(LIST+CONFIG, cli_list_config);
	set_handler_callback(LIST+BLACKLIST, cli_list_blacklist);
	set_handler_callback(LIST+DEVICES, cli_list_devices);
	set_handler_callback(LIST+WILDCARDS, cli_list_wildcards);
	set_handler_callback(ADD+PATH, cli_add_path);
	set_handler_callback(DEL+PATH, cli_del_path);
	set_handler_callback(ADD+MAP, cli_add_map);
	set_handler_callback(DEL+MAP, cli_del_map);
	set_handler_callback(SWITCH+MAP+GROUP, cli_switch_group);
	set_handler_callback(RECONFIGURE, cli_reconfigure);
	set_handler_callback(SUSPEND+MAP, cli_suspend);
	set_handler_callback(RESUME+MAP, cli_resume);
	set_handler_callback(RESIZE+MAP, cli_resize);
	set_handler_callback(RELOAD+MAP, cli_reload);
	set_handler_callback(RESET+MAP, cli_reassign);
	set_handler_callback(REINSTATE+PATH, cli_reinstate);
	set_handler_callback(FAIL+PATH, cli_fail);
	set_handler_callback(DISABLEQ+MAP, cli_disable_queueing);
	set_handler_callback(RESTOREQ+MAP, cli_restore_queueing);
	set_handler_callback(DISABLEQ+MAPS, cli_disable_all_queueing);
	set_handler_callback(RESTOREQ+MAPS, cli_restore_all_queueing);
	set_handler_callback(QUIT, cli_quit);
	set_handler_callback(SHUTDOWN, cli_shutdown);
	set_handler_callback(GETPRSTATUS+MAP, cli_getprstatus);
	set_handler_callback(SETPRSTATUS+MAP, cli_setprstatus);
	set_handler_callback(UNSETPRSTATUS+MAP, cli_unsetprstatus);
	set_handler_callback(FORCEQ+DAEMON, cli_force_no_daemon_q);
	set_handler_callback(RESTOREQ+DAEMON, cli_restore_no_daemon_q);

	umask(077);
	uxsock_listen(&uxsock_trigger, ap);

	return NULL;
}
