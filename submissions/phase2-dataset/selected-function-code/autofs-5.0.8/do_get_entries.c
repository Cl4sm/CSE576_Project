static int do_get_entries(struct ldap_search_params *sp, struct map_source *source, struct lookup_context *ctxt)
{
	struct autofs_point *ap = sp->ap;
	struct mapent_cache *mc = source->mc;
	char buf[MAX_ERR_BUF];
	struct berval **bvKey;
	struct berval **bvValues;
	LDAPMessage *e;
	char *class, *info, *entry;
	int rv, ret;
	int i, count;

	class = ctxt->schema->entry_class;
	entry = ctxt->schema->entry_attr;
	info = ctxt->schema->value_attr;

	e = ldap_first_entry(sp->ldap, sp->result);
	if (!e) {
		debug(ap->logopt,
		      MODPREFIX "query succeeded, no matches for %s",
		      sp->query);
		ret = ldap_parse_result(sp->ldap, sp->result,
					&rv, NULL, NULL, NULL, NULL, 0);
		if (ret == LDAP_SUCCESS)
			return rv;
		else
			return LDAP_OPERATIONS_ERROR;
	} else
		debug(ap->logopt, MODPREFIX "examining entries");

	while (e) {
		char *mapent = NULL;
		size_t mapent_len = 0;
		char *k_val;
		ber_len_t k_len;
		char *s_key;

		bvKey = ldap_get_values_len(sp->ldap, e, entry);
		if (!bvKey || !*bvKey) {
			e = ldap_next_entry(sp->ldap, e);
			if (!e) {
				debug(ap->logopt, MODPREFIX
				      "failed to get next entry for query %s",
				      sp->query);
				ret = ldap_parse_result(sp->ldap,
							sp->result, &rv,
							NULL, NULL, NULL, NULL, 0);
				if (ret == LDAP_SUCCESS)
					return rv;
				else
					return LDAP_OPERATIONS_ERROR;
			}
			continue;
		}

		/*
		 * By definition keys should be unique within each map entry,
		 * but as always there are exceptions.
		 */
		k_val = NULL;
		k_len = 0;

		/*
		 * Keys should be unique so, in general, there shouldn't be
		 * more than one attribute value. We make an exception for
		 * wildcard entries as people may have values for '*' or
		 * '/' for compaibility reasons. We use the '/' as the
		 * wildcard in LDAP but allow '*' as well to allow for
		 * people using older schemas that allow '*' as a key
		 * value. Another case where there can be multiple key
		 * values is when people have used the "%" hack to specify
		 * case matching ctriteria in a case insensitive attribute.
		 */
		count = ldap_count_values_len(bvKey);
		if (count > 1) {
			unsigned int i;

			/* Check for the "/" and "*" and use as "/" if found */
			for (i = 0; i < count; i++) {
				bvKey[i]->bv_val[bvKey[i]->bv_len] = '\0';

				/*
				 * If multiple entries are present they could
				 * be the result of people using the "%" hack so
				 * ignore them.
				 */
				if (strchr(bvKey[i]->bv_val, '%'))
					continue;

				/* check for wildcard */
				if (bvKey[i]->bv_len == 1 &&
				    (*bvKey[i]->bv_val == '/' ||
				     *bvKey[i]->bv_val == '*')) {
					/* always use '/' internally */
					*bvKey[i]->bv_val = '/';
					k_val = bvKey[i]->bv_val;
					k_len = 1;
					break;
				}

				/*
				 * We have a result from LDAP so this is a
				 * valid entry. Set the result to the LDAP
				 * key that isn't a wildcard and doesn't have
				 * any "%" hack values present. This should be
				 * the case insensitive match string for the
				 * nis schema, the default value.
				 */
				k_val = bvKey[i]->bv_val;
				k_len = bvKey[i]->bv_len;

				break;
			}

			if (!k_val) {
				error(ap->logopt,
				      MODPREFIX "invalid entry %.*s - ignoring",
				      bvKey[0]->bv_len, bvKey[0]->bv_val);
				goto next;
			}
		} else {
			/* Check for the "*" and use as "/" if found */
			if (bvKey[0]->bv_len == 1 && *bvKey[0]->bv_val == '*')
				*bvKey[0]->bv_val = '/';
			k_val = bvKey[0]->bv_val;
			k_len = bvKey[0]->bv_len;
		}

		/*
		 * Ignore keys beginning with '+' as plus map
		 * inclusion is only valid in file maps.
		 */
		if (*k_val == '+') {
			warn(ap->logopt,
			     MODPREFIX
			     "ignoreing '+' map entry - not in file map");
			goto next;
		}

		bvValues = ldap_get_values_len(sp->ldap, e, info);
		if (!bvValues || !*bvValues) {
			debug(ap->logopt,
			      MODPREFIX "no %s defined for %s", info, sp->query);
			goto next;
		}

		/*
		 * We expect that there will be only one value because
		 * questions of order of returned value entries but we
		 * accumulate values to support simple multi-mounts.
		 *
		 * If the ordering of a mount spec with another containing
		 * options or the actual order of entries causes problems
		 * it won't be supported. Perhaps someone can instruct us
		 * how to force an ordering.
		 */
		count = ldap_count_values_len(bvValues);
		for (i = 0; i < count; i++) {
			char *v_val = bvValues[i]->bv_val;
			ber_len_t v_len = bvValues[i]->bv_len;

			if (!mapent) {
				mapent = malloc(v_len + 1);
				if (!mapent) {
					char *estr;
					estr = strerror_r(errno, buf, sizeof(buf));
					logerr(MODPREFIX "malloc: %s", estr);
					ldap_value_free_len(bvValues);
					goto next;
				}
				strncpy(mapent, v_val, v_len);
				mapent[v_len] = '\0';
				mapent_len = v_len;
			} else {
				int new_size = mapent_len + v_len + 2;
				char *new_me;
				new_me = realloc(mapent, new_size);
				if (new_me) {
					mapent = new_me;
					strcat(mapent, " ");
					strncat(mapent, v_val, v_len);
					mapent[new_size - 1] = '\0';
					mapent_len = new_size - 1;
				} else {
					char *estr;
					estr = strerror_r(errno, buf, sizeof(buf));
					logerr(MODPREFIX "realloc: %s", estr);
				}
			}
		}
		ldap_value_free_len(bvValues);

		if (*k_val == '/' && k_len == 1) {
			if (ap->type == LKP_DIRECT)
				goto next;
			*k_val = '*';
		}

		if (strcasecmp(class, "nisObject")) {
			s_key = sanitize_path(k_val, k_len, ap->type, ap->logopt);
			if (!s_key)
				goto next;
		} else {
			char *dec_key;
			int dec_len = decode_percent_hack(k_val, &dec_key);

			if (dec_len < 0) {
				crit(ap->logopt,
				     "could not use percent hack to decode key %s",
				     k_val);
				goto next;
			}

			if (dec_len == 0)
				s_key = sanitize_path(k_val, k_len, ap->type, ap->logopt);
			else {
				s_key = sanitize_path(dec_key, dec_len, ap->type, ap->logopt);
				free(dec_key);
			}
			if (!s_key)
				goto next;
		}

		cache_writelock(mc);
		cache_update(mc, source, s_key, mapent, sp->age);
		cache_unlock(mc);

		free(s_key);
next:
		if (mapent) {
			free(mapent);
			mapent = NULL;
		}

		ldap_value_free_len(bvKey);
		e = ldap_next_entry(sp->ldap, e);
		if (!e) {
			debug(ap->logopt, MODPREFIX
			      "failed to get next entry for query %s",
			      sp->query);
			ret = ldap_parse_result(sp->ldap,
						sp->result, &rv,
						NULL, NULL, NULL, NULL, 0);
			if (ret == LDAP_SUCCESS)
				return rv;
			else
				return LDAP_OPERATIONS_ERROR;
		}
	}

	return LDAP_SUCCESS;
}