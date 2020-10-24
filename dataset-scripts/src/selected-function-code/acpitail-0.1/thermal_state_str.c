char * thermal_state_str(thermal_state_t ts)
{
	switch(ts)
	{
		case T_CRIT:
			return "critical temperature, will switch to S4";

		case T_HOT:
			return "high temperature, will shutdown immediately";

		case T_PASS:
			return "passive cooling";

		case T_ACT:
			return "active cooling";

		case T_OK:
			return "ok";

		case T_ERR:
			return "error";
	}

	return "?";
}