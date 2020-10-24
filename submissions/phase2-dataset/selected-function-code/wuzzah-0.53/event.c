	char *status_msg=NULL, *wr_msg=NULL, *extra_cmd=NULL;

	if(u->verified==B_LOGGED_IN || u->verified==B_LOGGED_OUT){
		if(b->conf) {
			status_msg=bud_expand_fmt(u, b->conf->status_msg);
			if(b->conf->write_users && write != -1) 
				wr_msg=bud_expand_fmt(u, b->conf->buddy_msg);
			extra_cmd=bud_expand_fmt(u, b->conf->eventcmd);
		} else {
			status_msg=bud_expand_fmt(u, g_config.status_msg);
			if(write > 0) wr_msg=bud_expand_fmt(u, g_config.buddy_msg);
			extra_cmd=bud_expand_fmt(u, g_config.eventcmd);
		}
	}

	if(status_msg) {
		fprintf(stdout, "%s", status_msg);
		free(status_msg);
	}
	if(wr_msg) {
		if(u->verified==B_LOGGED_IN)
			wri_do_write(u->u->ut_user, u->u->ut_line, wr_msg);
		free(wr_msg);
	}
	if(extra_cmd) {
		if(u->verified==B_LOGGED_IN)
			fork_and_exec(extra_cmd);
		free(extra_cmd);
	}
}
