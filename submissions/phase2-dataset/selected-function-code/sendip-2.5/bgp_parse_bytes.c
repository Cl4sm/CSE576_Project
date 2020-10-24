static u_int32_t bgp_parse_bytes (u_int8_t   *buf,
                                  char       *arg,
                                  char      **new_arg,
                                  u_int32_t   limit,
                                  int         base,
                                  char        stopc)
{
	u_int8_t *ptr = buf;
	char     *arg_ptr = arg;
	
	while (*arg_ptr != '\0' && *arg_ptr != stopc && limit > 0) {
		*ptr++ = (u_int8_t)strtoul(arg_ptr, &arg_ptr, base);
		if (*arg_ptr != '\0' && *arg_ptr != stopc) {
			arg_ptr++;
		}
		limit--;
	}
	
	if (new_arg != NULL) {
		*new_arg = arg_ptr;
	}
	
	return (ptr - buf);
}
