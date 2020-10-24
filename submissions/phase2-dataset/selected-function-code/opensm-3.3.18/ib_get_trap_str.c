const char *ib_get_trap_str(ib_net16_t trap_num)
{
	switch (cl_ntoh16(trap_num)) {
	case 64:
		return "GID in service";
	case 65:
		return "GID out of service";
	case 66:
		return "New mcast group created";
	case 67:
		return "Mcast group deleted";
	case 68:
		return "UnPath, Path no longer valid";
	case 69:
		return "RePath, Path recomputed";
	case 128:
		return "Link state change";
	case 129:
		return "Local Link integrity threshold reached";
	case 130:
		return "Excessive Buffer Overrun Threshold reached";
	case 131:
		return "Flow Control Update watchdog timer expired";
	case 144:
		return
		    "CapabilityMask, NodeDescription, Link [Width|Speed] Enabled, SM priority changed";
	case 145:
		return "System Image GUID changed";
	case 256:
		return "Bad M_Key";
	case 257:
		return "Bad P_Key";
	case 258:
		return "Bad Q_Key";
	case 259:
		return "Bad P_Key (switch external port)";
	default:
		break;
	}
	return "Unknown";
}