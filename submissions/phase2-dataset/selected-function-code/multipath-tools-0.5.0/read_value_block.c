vector
read_value_block(void)
{
	char *buf;
	int i;
	char *str = NULL;
	char *dup;
	vector vec = NULL;
	vector elements = vector_alloc();

	if (!elements)
		return NULL;

	buf = (char *) MALLOC(MAXBUF);

	if (!buf) {
		vector_free(elements);
		return NULL;
	}

	while (read_line(buf, MAXBUF)) {
		vec = alloc_strvec(buf);
		if (vec) {
			str = VECTOR_SLOT(vec, 0);
			if (!strcmp(str, EOB)) {
				free_strvec(vec);
				break;
			}

			for (i = 0; i < VECTOR_SIZE(vec); i++) {
				str = VECTOR_SLOT(vec, i);
				dup = (char *) MALLOC(strlen(str) + 1);
				if (!dup)
					goto out;
				memcpy(dup, str, strlen(str));

				if (!vector_alloc_slot(elements)) {
					free_strvec(vec);
					goto out1;
				}

				vector_set_slot(elements, dup);
			}
			free_strvec(vec);
		}
		memset(buf, 0, MAXBUF);
	}
	FREE(buf);
	return elements;
out1:
	FREE(dup);
out:
	FREE(buf);
	vector_free(elements);
	return NULL;
}
