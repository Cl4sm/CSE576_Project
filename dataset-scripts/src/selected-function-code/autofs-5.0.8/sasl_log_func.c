static int
sasl_log_func(void *context, int level, const char *message)
{
	switch (level) {
	case SASL_LOG_ERR:
	case SASL_LOG_FAIL:
		logerr("%s", message);
		break;
	case SASL_LOG_WARN:
		logmsg("%s", message);
		break;
	case SASL_LOG_NOTE:
		logmsg("%s", message);
		break;
	case SASL_LOG_DEBUG:
	case SASL_LOG_TRACE:
	case SASL_LOG_PASS:
		debug(LOGOPT_DEBUG, "%s", message);
		break;
	default:
		break;
	}

	return SASL_OK;
}