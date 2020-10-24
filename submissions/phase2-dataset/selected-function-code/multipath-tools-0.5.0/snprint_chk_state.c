static int
snprint_chk_state (char * buff, size_t len, struct path * pp)
{
	switch (pp->state) {
	case PATH_UP:
		return snprintf(buff, len, "ready");
	case PATH_DOWN:
		return snprintf(buff, len, "faulty");
	case PATH_SHAKY:
		return snprintf(buff, len, "shaky");
	case PATH_GHOST:
		return snprintf(buff, len, "ghost");
	case PATH_PENDING:
		return snprintf(buff, len, "i/o pending");
	case PATH_TIMEOUT:
		return snprintf(buff, len, "i/o timeout");
	default:
		return snprintf(buff, len, "undef");
	}
}
