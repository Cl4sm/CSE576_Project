int osm_qos_policy_validate(osm_qos_policy_t * p_qos_policy,
			    osm_log_t *p_log)
{
	cl_list_iterator_t match_rules_list_iterator;
	cl_list_iterator_t list_iterator;
	osm_qos_port_group_t *p_port_group = NULL;
	osm_qos_match_rule_t *p_qos_match_rule = NULL;
	char *str;
	unsigned i, j;
	int res = 0;
	uint64_t pkey_64;
	ib_net16_t pkey;
	osm_prtn_t * p_prtn;

	OSM_LOG_ENTER(p_log);

	/* set default qos level */

	p_qos_policy->p_default_qos_level =
	    __qos_policy_get_qos_level_by_name(p_qos_policy, OSM_QOS_POLICY_DEFAULT_LEVEL_NAME);
	if (!p_qos_policy->p_default_qos_level) {
		/* There's no default QoS level in the usual qos-level section.
		   Check whether the 'simple' default QoS level that can be
		   defined in the qos-ulp section exists */
		if (__default_simple_qos_level.sl_set) {
			p_qos_policy->p_default_qos_level = &__default_simple_qos_level;
		}
		else {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR AC10: "
				"Default qos-level (%s) not defined.\n",
				OSM_QOS_POLICY_DEFAULT_LEVEL_NAME);
			res = 1;
			goto Exit;
		}
	}

	/* scan all the match rules, and fill the lists of pointers to
	   relevant qos levels and port groups to speed up PR matching */

	i = 1;
	match_rules_list_iterator =
	    cl_list_head(&p_qos_policy->qos_match_rules);
	while (match_rules_list_iterator !=
	       cl_list_end(&p_qos_policy->qos_match_rules)) {
		p_qos_match_rule =
		    (osm_qos_match_rule_t *)
		    cl_list_obj(match_rules_list_iterator);
		CL_ASSERT(p_qos_match_rule);

		/* find the matching qos-level for each match-rule */

		if (!p_qos_match_rule->p_qos_level)
			p_qos_match_rule->p_qos_level =
				__qos_policy_get_qos_level_by_name(p_qos_policy,
					       p_qos_match_rule->qos_level_name);

		if (!p_qos_match_rule->p_qos_level) {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR AC11: "
				"qos-match-rule num %u: qos-level '%s' not found\n",
				i, p_qos_match_rule->qos_level_name);
			res = 1;
			goto Exit;
		}

		/* find the matching port-group for element of source_list */

		if (cl_list_count(&p_qos_match_rule->source_list)) {
			list_iterator =
			    cl_list_head(&p_qos_match_rule->source_list);
			while (list_iterator !=
			       cl_list_end(&p_qos_match_rule->source_list)) {
				str = (char *)cl_list_obj(list_iterator);
				CL_ASSERT(str);

				p_port_group =
				    __qos_policy_get_port_group_by_name(p_qos_policy, str);
				if (!p_port_group) {
					OSM_LOG(p_log, OSM_LOG_ERROR, "ERR AC12: "
						"qos-match-rule num %u: source port-group '%s' not found\n",
						i, str);
					res = 1;
					goto Exit;
				}

				cl_list_insert_tail(&p_qos_match_rule->
						    source_group_list,
						    p_port_group);

				list_iterator = cl_list_next(list_iterator);
			}
		}

		/* find the matching port-group for element of destination_list */

		if (cl_list_count(&p_qos_match_rule->destination_list)) {
			list_iterator =
			    cl_list_head(&p_qos_match_rule->destination_list);
			while (list_iterator !=
			       cl_list_end(&p_qos_match_rule->
					   destination_list)) {
				str = (char *)cl_list_obj(list_iterator);
				CL_ASSERT(str);

				p_port_group =
				    __qos_policy_get_port_group_by_name(p_qos_policy,str);
				if (!p_port_group) {
					OSM_LOG(p_log, OSM_LOG_ERROR, "ERR AC13: "
						"qos-match-rule num %u: destination port-group '%s' not found\n",
						i, str);
					res = 1;
					goto Exit;
				}

				cl_list_insert_tail(&p_qos_match_rule->
						    destination_group_list,
						    p_port_group);

				list_iterator = cl_list_next(list_iterator);
			}
		}

		/*
		 * Scan all the pkeys in matching rule, and if the
		 * partition for these pkeys exists, set the SL
		 * according to the QoS Level.
		 * Warn if there's mismatch between QoS level SL
		 * and Partition SL.
		 */

		for (j = 0; j < p_qos_match_rule->pkey_range_len; j++) {
			for ( pkey_64 = p_qos_match_rule->pkey_range_arr[j][0];
			      pkey_64 <= p_qos_match_rule->pkey_range_arr[j][1];
			      pkey_64++) {
                                pkey = cl_hton16((uint16_t)(pkey_64 & 0x7fff));
				p_prtn = (osm_prtn_t *)cl_qmap_get(
					&p_qos_policy->p_subn->prtn_pkey_tbl, pkey);

				if (p_prtn == (osm_prtn_t *)cl_qmap_end(
					&p_qos_policy->p_subn->prtn_pkey_tbl))
					/* partition for this pkey not found */
					OSM_LOG(p_log, OSM_LOG_ERROR, "ERR AC14: "
						"pkey 0x%04X in match rule - "
						"partition doesn't exist\n",
						cl_ntoh16(pkey));
				else
					__qos_policy_validate_pkey(p_qos_policy,
							p_qos_match_rule,
							p_prtn);
			}
		}

		/* done with the current match-rule */

		match_rules_list_iterator =
		    cl_list_next(match_rules_list_iterator);
		i++;
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return res;
}				/* osm_qos_policy_validate() */