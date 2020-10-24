struct map_type_info *parse_map_type_info(const char *str)
{
	struct map_type_info *info;
	char *buf, *type, *fmt, *map, *tmp;
	char *pos;

	buf = strdup(str);
	if (!buf)
		return NULL;

	info = malloc(sizeof(struct map_type_info));
	if (!info) {
		free(buf);
		return NULL;
	}
	memset(info, 0, sizeof(struct map_type_info));

	type = fmt = map = NULL;

	tmp = strchr(buf, ':');
	if (!tmp) {
		pos = buf;
		while (*pos == ' ')
			*pos++ = '\0';
		map = pos;
	} else {
		int i, j;

		for (i = 0; i < map_type_count; i++) {
			char *m_type = map_type[i].type;
			unsigned int m_len = map_type[i].len;

			pos = buf;

			if (strncmp(m_type, pos, m_len))
				continue;

			type = pos;
			pos += m_len;

			if (*pos == ' ' || *pos == ':') {
				while (*pos == ' ')
					*pos++ = '\0';
				if (*pos != ':') {
					free(buf);
					free(info);
					return NULL;
				} else {
					*pos++ = '\0';
					while (*pos && *pos == ' ')
						*pos++ = '\0';
					map = pos;
					break;
				}
			}

			if (*pos == ',') {
				*pos++ = '\0';
				for (j = 0; j < format_type_count; j++) {
					char *f_type = format_type[j].type;
					unsigned int f_len = format_type[j].len;
				
					if (strncmp(f_type, pos, f_len))
						continue;

					fmt = pos;
					pos += f_len;

					if (*pos == ' ' || *pos == ':') {
						while (*pos == ' ')
							*pos++ = '\0';
						if (*pos != ':') {
							free(buf);
							free(info);
							return NULL;
						} else {
							*pos++ = '\0';
							while (*pos && *pos == ' ')
								*pos++ = '\0';
							map = pos;
							break;
						}
					}
				}
			}
		}

		if (!type) {
			pos = buf;
			while (*pos == ' ')
				*pos++ = '\0';
			map = pos;
		}
	}

	/* Look for space terminator - ignore local options */
	for (tmp = buf; *tmp; tmp++) {
		if (*tmp == ' ') {
			*tmp = '\0';
			break;
		}
		if (*tmp == '\\')
			tmp++;
	}

	if (type) {
		info->type = strdup(type);
		if (!info->type) {
			free(buf);
			free_map_type_info(info);
			return NULL;
		}
	}

	if (fmt) {
		info->format = strdup(fmt);
		if (!info->format) {
			free(buf);
			free_map_type_info(info);
			return NULL;
		}
	}

	if (map) {
		info->map = strdup(map);
		if (!info->map) {
			free(buf);
			free_map_type_info(info);
			return NULL;
		}
	}

	free(buf);

	return info;
}