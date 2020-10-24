void osm_log_msg_box_v2(IN osm_log_t * log, osm_log_level_t level,
			const int file_id, const char *func_name,
			const char *msg)
{
#define MSG_BOX_LENGTH 66
	char buf[MSG_BOX_LENGTH + 1];
	int i, n;

	if (!osm_log_is_active_v2(log, level, file_id))
		return;

	n = (MSG_BOX_LENGTH - strlen(msg)) / 2 - 1;
	if (n < 0)
		n = 0;
	for (i = 0; i < n; i++)
		sprintf(buf + i, "*");
	n += snprintf(buf + n, sizeof(buf) - n, " %s ", msg);
	for (i = n; i < MSG_BOX_LENGTH; i++)
		buf[i] = '*';
	buf[i] = '\0';

	osm_log_v2(log, level, file_id, "%s:\n\n\n"
		   "*********************************************"
		   "*********************\n%s\n"
		   "*********************************************"
		   "*********************\n\n\n", func_name, buf);
}