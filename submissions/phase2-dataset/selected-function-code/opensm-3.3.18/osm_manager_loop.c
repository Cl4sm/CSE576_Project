int osm_manager_loop(osm_subn_opt_t * p_opt, osm_opensm_t * p_osm)
{
	int console_init_flag = 0;

	if (is_console_enabled(p_opt)) {
		if (!osm_console_init(p_opt, &p_osm->console, &p_osm->log))
			console_init_flag = 1;
	}

	/*
	   Sit here forever - dwell or do console i/o & cmds
	 */
	while (!osm_exit_flag) {
		if (console_init_flag) {
			if (osm_console(p_osm))
				console_init_flag = 0;
		} else
			cl_thread_suspend(10000);

		if (osm_usr1_flag) {
			osm_usr1_flag = 0;
			osm_log_reopen_file(&(p_osm->log));
		}
		if (osm_hup_flag) {
			osm_hup_flag = 0;
			/* a HUP signal should only start a new heavy sweep */
			p_osm->subn.force_heavy_sweep = TRUE;
			osm_opensm_sweep(p_osm);
		}
	}
	if (is_console_enabled(p_opt))
		osm_console_exit(&p_osm->console, &p_osm->log);
	return 0;
}