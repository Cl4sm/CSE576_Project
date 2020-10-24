static void __create_mgrp(struct part_conf *conf, struct precreate_mgroup *group)
{
	unsigned int scope;

	if (!group->flags.scope_mask) {
		osm_prtn_add_mcgroup(conf->p_log, conf->p_subn, conf->p_prtn,
				     (uint8_t) group->flags.rate,
				     (uint8_t) group->flags.mtu,
				     group->flags.sl,
				     0,
				     group->flags.Q_Key,
				     group->flags.TClass,
				     group->flags.FlowLabel,
				     &group->mgid);
	} else {
		for (scope = 0; scope < 16; scope++) {
			if (((1<<scope) & group->flags.scope_mask) == 0)
				continue;

			osm_prtn_add_mcgroup(conf->p_log, conf->p_subn, conf->p_prtn,
					     (uint8_t)group->flags.rate,
					     (uint8_t)group->flags.mtu,
					     (uint8_t)group->flags.sl,
					     (uint8_t)scope,
					     group->flags.Q_Key,
					     group->flags.TClass,
					     group->flags.FlowLabel,
					     &group->mgid);
		}
	}
}