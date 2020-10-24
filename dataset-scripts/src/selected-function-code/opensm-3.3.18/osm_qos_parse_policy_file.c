 ***************************************************/

int osm_qos_parse_policy_file(IN osm_subn_t * p_subn)
{
    int res = 0;
    static boolean_t first_time = TRUE;
    p_qos_parser_osm_log = &p_subn->p_osm->log;

    OSM_LOG_ENTER(p_qos_parser_osm_log);

    osm_qos_policy_destroy(p_subn->p_qos_policy);
    p_subn->p_qos_policy = NULL;

    yyin = fopen (p_subn->opt.qos_policy_file, "r");
    if (!yyin)
    {
        if (strcmp(p_subn->opt.qos_policy_file,OSM_DEFAULT_QOS_POLICY_FILE)) {
            OSM_LOG(p_qos_parser_osm_log, OSM_LOG_ERROR, "ERR AC01: "
                    "Failed opening QoS policy file %s - %s\n",
                    p_subn->opt.qos_policy_file, strerror(errno));
            res = 1;
        }
        else
            OSM_LOG(p_qos_parser_osm_log, OSM_LOG_VERBOSE,
                    "QoS policy file not found (%s)\n",
                    p_subn->opt.qos_policy_file);

        goto Exit;
    }

    if (first_time)
    {
        first_time = FALSE;
        __setup_simple_qos_levels();
        __setup_ulp_match_rules();
        OSM_LOG(p_qos_parser_osm_log, OSM_LOG_INFO,
		"Loading QoS policy file (%s)\n",
                p_subn->opt.qos_policy_file);
    }
    else
        /*
         * ULP match rules list was emptied at the end of
         * previous parsing iteration.
         * What's left is to clear simple QoS levels.
         */
        __clear_simple_qos_levels();

    column_num = 1;
    line_num = 1;

    p_subn->p_qos_policy = osm_qos_policy_create(p_subn);

    __parser_tmp_struct_init();
    p_qos_policy = p_subn->p_qos_policy;

    res = yyparse();

    __parser_tmp_struct_destroy();

    if (res != 0)
    {
        OSM_LOG(p_qos_parser_osm_log, OSM_LOG_ERROR, "ERR AC03: "
                "Failed parsing QoS policy file (%s)\n",
                p_subn->opt.qos_policy_file);
        osm_qos_policy_destroy(p_subn->p_qos_policy);
        p_subn->p_qos_policy = NULL;
        res = 1;
        goto Exit;
    }

    /* add generated ULP match rules to the usual match rules */
    __process_ulp_match_rules();

    if (osm_qos_policy_validate(p_subn->p_qos_policy,p_qos_parser_osm_log))
    {
        OSM_LOG(p_qos_parser_osm_log, OSM_LOG_ERROR, "ERR AC04: "
                "Error(s) in QoS policy file (%s)\n",
                p_subn->opt.qos_policy_file);
        fprintf(stderr, "Error(s) in QoS policy file (%s)\n",
                p_subn->opt.qos_policy_file);
        osm_qos_policy_destroy(p_subn->p_qos_policy);
        p_subn->p_qos_policy = NULL;
        res = 1;
        goto Exit;
    }

  Exit:
    if (yyin)
    {
        yyrestart(yyin);
        fclose(yyin);
    }
    OSM_LOG_EXIT(p_qos_parser_osm_log);