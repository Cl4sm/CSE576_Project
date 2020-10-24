void cl_log_event(IN const char *const name, IN const cl_log_type_t type,
		  IN const char *const message,
		  IN const void *const p_data OPTIONAL,
		  IN const uint32_t data_len)
{
	int priority, i;
	char data[CL_LOG_DATA_SIZE];
	char *p_buf;
	uint8_t *p_int_data = (uint8_t *) p_data;

	CL_ASSERT(name);
	CL_ASSERT(message);

	openlog(name, LOG_NDELAY | LOG_PID, LOG_USER);
	switch (type) {
	case CL_LOG_ERROR:
		priority = LOG_ERR;
		break;

	case CL_LOG_WARN:
		priority = LOG_WARNING;
		break;

	case CL_LOG_INFO:
	default:
		priority = LOG_INFO;
		break;
	}

	if (p_data) {
		CL_ASSERT(data_len);
		if (data_len < CL_MAX_LOG_DATA) {
			p_buf = data;
			/* Format the data into ASCII. */
			for (i = 0; i < data_len; i++) {
				sprintf(p_buf, "%02x", *p_int_data++);
				p_buf += 2;

				/* Add line break after 8 DWORDS. */
				if (i % 32) {
					sprintf(p_buf++, "\n");
					continue;
				}

				/* Add a space between DWORDS. */
				if (i % 4)
					sprintf(p_buf++, " ");
			}
			syslog(priority, "%s data:\n%s\n", message, p_buf);
		} else {
			/* The data portion is too large to log. */
			cl_msg_out
			    ("cl_log() - WARNING: data too large to log.\n");
			syslog(priority, "%s\n", message);
		}
	} else {
		syslog(priority, "%s\n", message);
	}
	closelog();
}