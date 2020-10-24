#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 17:

/* Line 1455 of yacc.c  */
#line 365 "osm_qos_parser_y.y"
    {
                        __parser_port_group_start();
                    }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 370 "osm_qos_parser_y.y"
    {
                        if ( __parser_port_group_end() )
                            return 1;
                    }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 433 "osm_qos_parser_y.y"
    {
                        __parser_vlarb_scope_start();
                    }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 438 "osm_qos_parser_y.y"
    {
                        if ( __parser_vlarb_scope_end() )
                            return 1;
                    }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 479 "osm_qos_parser_y.y"
    {
                        __parser_sl2vl_scope_start();
                    }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 484 "osm_qos_parser_y.y"
    {
                        if ( __parser_sl2vl_scope_end() )
                            return 1;
                    }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 550 "osm_qos_parser_y.y"
    {
                        __parser_qos_level_start();
                    }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 555 "osm_qos_parser_y.y"
    {
                        if ( __parser_qos_level_end() )
                            return 1;
                    }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 604 "osm_qos_parser_y.y"
    {
                        __parser_match_rule_start();
                    }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 609 "osm_qos_parser_y.y"
    {
                        if ( __parser_match_rule_end() )
                            return 1;
                    }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 649 "osm_qos_parser_y.y"
    {
                        /* parsing default ulp rule: "default: num" */
                        cl_list_iterator_t    list_iterator;
                        uint64_t            * p_tmp_num;

                        list_iterator = cl_list_head(&tmp_parser_struct.num_list);
                        p_tmp_num = (uint64_t*)cl_list_obj(list_iterator);
                        if (*p_tmp_num > 15)
                        {
                            yyerror("illegal SL value");
                            return 1;
                        }
                        __default_simple_qos_level.sl = (uint8_t)(*p_tmp_num);
                        __default_simple_qos_level.sl_set = TRUE;
                        free(p_tmp_num);
                        cl_list_remove_all(&tmp_parser_struct.num_list);
                    }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 667 "osm_qos_parser_y.y"
    {
                        /* "any, service-id ... : sl" - one instance of list of ranges */
                        uint64_t ** range_arr;
                        unsigned    range_len;

                        if (!cl_list_count(&tmp_parser_struct.num_pair_list))
                        {
                            yyerror("ULP rule doesn't have service ids");
                            return 1;
                        }

                        /* get all the service id ranges */
                        __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );

                        p_current_qos_match_rule->service_id_range_arr = range_arr;
                        p_current_qos_match_rule->service_id_range_len = range_len;

                    }
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 688 "osm_qos_parser_y.y"
    {
                        /* "any, pkey ... : sl" - one instance of list of ranges */
                        uint64_t ** range_arr;
                        unsigned    range_len;

                        if (!cl_list_count(&tmp_parser_struct.num_pair_list))
                        {
                            yyerror("ULP rule doesn't have pkeys");
                            return 1;
                        }

                        /* get all the pkey ranges */
                        __pkey_rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );

                        p_current_qos_match_rule->pkey_range_arr = range_arr;
                        p_current_qos_match_rule->pkey_range_len = range_len;

                    }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 709 "osm_qos_parser_y.y"
    {
                        /* any, target-port-guid ... : sl */
                        uint64_t ** range_arr;
                        unsigned    range_len;

                        if (!cl_list_count(&tmp_parser_struct.num_pair_list))
                        {
                            yyerror("ULP rule doesn't have port guids");
                            return 1;
                        }

                        /* create a new port group with these ports */
                        __parser_port_group_start();

                        p_current_port_group->name = strdup("_ULP_Targets_");
                        p_current_port_group->use = strdup("Generated from ULP rules");

                        __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );

                        __parser_add_guid_range_to_port_map(
                                              &p_current_port_group->port_map,
                                              range_arr,
                                              range_len);

                        /* add this port group to the destination
                           groups of the current match rule */
                        cl_list_insert_tail(&p_current_qos_match_rule->destination_group_list,
                                            p_current_port_group);

                        __parser_port_group_end();

                    }
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 744 "osm_qos_parser_y.y"
    {
			/* any, source-port-guid ... : sl */
			uint64_t ** range_arr;
			unsigned    range_len;

			if (!cl_list_count(&tmp_parser_struct.num_pair_list))
			{
				yyerror("ULP rule doesn't have port guids");
				return 1;
			}

                        /* create a new port group with these ports */
                        __parser_port_group_start();

                        p_current_port_group->name = strdup("_ULP_Sources_");
                        p_current_port_group->use = strdup("Generated from ULP rules");

                        __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );

                        __parser_add_guid_range_to_port_map(
                                              &p_current_port_group->port_map,
                                              range_arr,
                                              range_len);

                        /* add this port group to the source
                           groups of the current match rule */
                        cl_list_insert_tail(&p_current_qos_match_rule->source_group_list,
                                            p_current_port_group);

                        __parser_port_group_end();

		    }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 779 "osm_qos_parser_y.y"
    {
			/* any, source-target-port-guid ... : sl */
			uint64_t ** range_arr;
			unsigned    range_len;

			if (!cl_list_count(&tmp_parser_struct.num_pair_list))
			{
				yyerror("ULP rule doesn't have port guids");
				return 1;
			}

                        /* create a new port group with these ports */
                        __parser_port_group_start();

                        p_current_port_group->name = strdup("_ULP_Sources_Targets_");
                        p_current_port_group->use = strdup("Generated from ULP rules");

                        __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );

                        __parser_add_guid_range_to_port_map(
                                              &p_current_port_group->port_map,
                                              range_arr,
                                              range_len);

                        /* add this port group to the source and destination
                           groups of the current match rule */
                        cl_list_insert_tail(&p_current_qos_match_rule->source_group_list,
                                            p_current_port_group);

                        cl_list_insert_tail(&p_current_qos_match_rule->destination_group_list,
                                            p_current_port_group);

                        __parser_port_group_end();

		    }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 817 "osm_qos_parser_y.y"
    {
                        /* "sdp : sl" - default SL for SDP */
                        uint64_t ** range_arr =
                               (uint64_t **)malloc(sizeof(uint64_t *));
                        range_arr[0] = (uint64_t *)malloc(2*sizeof(uint64_t));
                        range_arr[0][0] = OSM_QOS_POLICY_ULP_SDP_SERVICE_ID;
                        range_arr[0][1] = OSM_QOS_POLICY_ULP_SDP_SERVICE_ID + 0xFFFF;

                        p_current_qos_match_rule->service_id_range_arr = range_arr;
                        p_current_qos_match_rule->service_id_range_len = 1;

                    }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 830 "osm_qos_parser_y.y"
    {
                        /* sdp with port numbers */
                        uint64_t ** range_arr;
                        unsigned    range_len;
                        unsigned    i;

                        if (!cl_list_count(&tmp_parser_struct.num_pair_list))
                        {
                            yyerror("SDP ULP rule doesn't have port numbers");
                            return 1;
                        }

                        /* get all the port ranges */
                        __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );
                        /* now translate these port numbers into service ids */
                        for (i = 0; i < range_len; i++)
                        {
                            if (range_arr[i][0] > 0xFFFF || range_arr[i][1] > 0xFFFF)
                            {
                                yyerror("SDP port number out of range");
				free(range_arr);
                                return 1;
                            }
                            range_arr[i][0] += OSM_QOS_POLICY_ULP_SDP_SERVICE_ID;
                            range_arr[i][1] += OSM_QOS_POLICY_ULP_SDP_SERVICE_ID;
                        }

                        p_current_qos_match_rule->service_id_range_arr = range_arr;
                        p_current_qos_match_rule->service_id_range_len = range_len;

                    }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 864 "osm_qos_parser_y.y"
    {
                        /* "rds : sl" - default SL for RDS */
                        uint64_t ** range_arr =
                               (uint64_t **)malloc(sizeof(uint64_t *));
                        range_arr[0] = (uint64_t *)malloc(2*sizeof(uint64_t));
                        range_arr[0][0] = range_arr[0][1] =
                           OSM_QOS_POLICY_ULP_RDS_SERVICE_ID + OSM_QOS_POLICY_ULP_RDS_PORT;

                        p_current_qos_match_rule->service_id_range_arr = range_arr;
                        p_current_qos_match_rule->service_id_range_len = 1;

                    }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 877 "osm_qos_parser_y.y"
    {
                        /* rds with port numbers */
                        uint64_t ** range_arr;
                        unsigned    range_len;
                        unsigned    i;

                        if (!cl_list_count(&tmp_parser_struct.num_pair_list))
                        {
                            yyerror("RDS ULP rule doesn't have port numbers");
                            return 1;
                        }

                        /* get all the port ranges */
                        __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );
                        /* now translate these port numbers into service ids */
                        for (i = 0; i < range_len; i++)
                        {
                            if (range_arr[i][0] > 0xFFFF || range_arr[i][1] > 0xFFFF)
                            {
                                yyerror("SDP port number out of range");
				free(range_arr);
                                return 1;
                            }
                            range_arr[i][0] += OSM_QOS_POLICY_ULP_RDS_SERVICE_ID;
                            range_arr[i][1] += OSM_QOS_POLICY_ULP_RDS_SERVICE_ID;
                        }

                        p_current_qos_match_rule->service_id_range_arr = range_arr;
                        p_current_qos_match_rule->service_id_range_len = range_len;

                    }
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 911 "osm_qos_parser_y.y"
    {
                        /* "iSER : sl" - default SL for iSER */
                        uint64_t ** range_arr =
                               (uint64_t **)malloc(sizeof(uint64_t *));
                        range_arr[0] = (uint64_t *)malloc(2*sizeof(uint64_t));
                        range_arr[0][0] = range_arr[0][1] =
                           OSM_QOS_POLICY_ULP_ISER_SERVICE_ID + OSM_QOS_POLICY_ULP_ISER_PORT;

                        p_current_qos_match_rule->service_id_range_arr = range_arr;
                        p_current_qos_match_rule->service_id_range_len = 1;

                    }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 924 "osm_qos_parser_y.y"
    {
                        /* iser with port numbers */
                        uint64_t ** range_arr;
                        unsigned    range_len;
                        unsigned    i;

                        if (!cl_list_count(&tmp_parser_struct.num_pair_list))
                        {
                            yyerror("iSER ULP rule doesn't have port numbers");
                            return 1;
                        }

                        /* get all the port ranges */
                        __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );
                        /* now translate these port numbers into service ids */
                        for (i = 0; i < range_len; i++)
                        {
                            if (range_arr[i][0] > 0xFFFF || range_arr[i][1] > 0xFFFF)
                            {
                                yyerror("SDP port number out of range");
				free(range_arr);
                                return 1;
                            }
                            range_arr[i][0] += OSM_QOS_POLICY_ULP_ISER_SERVICE_ID;
                            range_arr[i][1] += OSM_QOS_POLICY_ULP_ISER_SERVICE_ID;
                        }

                        p_current_qos_match_rule->service_id_range_arr = range_arr;
                        p_current_qos_match_rule->service_id_range_len = range_len;

                    }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 958 "osm_qos_parser_y.y"
    {
                        /* srp with target guids - this rule is similar
                           to writing 'any' ulp with target port guids */
                        uint64_t ** range_arr;
                        unsigned    range_len;

                        if (!cl_list_count(&tmp_parser_struct.num_pair_list))
                        {
                            yyerror("SRP ULP rule doesn't have port guids");
                            return 1;
                        }

                        /* create a new port group with these ports */
                        __parser_port_group_start();

                        p_current_port_group->name = strdup("_SRP_Targets_");
                        p_current_port_group->use = strdup("Generated from ULP rules");

                        __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );

                        __parser_add_guid_range_to_port_map(
                                              &p_current_port_group->port_map,
                                              range_arr,
                                              range_len);

                        /* add this port group to the destination
                           groups of the current match rule */
                        cl_list_insert_tail(&p_current_qos_match_rule->destination_group_list,
                                            p_current_port_group);

                        __parser_port_group_end();

                    }
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 994 "osm_qos_parser_y.y"
    {
                        /* ipoib w/o any pkeys (default pkey) */
                        uint64_t ** range_arr =
                               (uint64_t **)malloc(sizeof(uint64_t *));
                        range_arr[0] = (uint64_t *)malloc(2*sizeof(uint64_t));
                        range_arr[0][0] = range_arr[0][1] = 0x7fff;

                        /*
                         * Although we know that the default partition exists,
                         * we still need to validate it by checking that it has
                         * at least two full members. Otherwise IPoIB won't work.
                         */
                        if (__validate_pkeys(range_arr, 1, TRUE))
                            return 1;

                        p_current_qos_match_rule->pkey_range_arr = range_arr;
                        p_current_qos_match_rule->pkey_range_len = 1;

                    }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1014 "osm_qos_parser_y.y"
    {
                        /* ipoib with pkeys */
                        uint64_t ** range_arr;
                        unsigned    range_len;

                        if (!cl_list_count(&tmp_parser_struct.num_pair_list))
                        {
                            yyerror("IPoIB ULP rule doesn't have pkeys");
                            return 1;
                        }

                        /* get all the pkey ranges */
                        __pkey_rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                              &range_arr,
                                              &range_len );

                        /*
                         * Validate pkeys.
                         * For IPoIB pkeys the validation is strict.
                         * If some problem would be found, parsing will
                         * be aborted with a proper error messages.
                         */
			if (__validate_pkeys(range_arr, range_len, TRUE)) {
			    free(range_arr);
                            return 1;
			}

                        p_current_qos_match_rule->pkey_range_arr = range_arr;
                        p_current_qos_match_rule->pkey_range_len = range_len;

                    }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1048 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1051 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1054 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1057 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1060 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1063 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1066 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1069 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1072 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1075 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1078 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1081 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1084 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1087 "osm_qos_parser_y.y"
    { __parser_ulp_match_rule_start(); }
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1090 "osm_qos_parser_y.y"
    {
                        /* get the SL for ULP rules */
                        cl_list_iterator_t  list_iterator;
                        uint64_t          * p_tmp_num;
                        uint8_t             sl;

                        list_iterator = cl_list_head(&tmp_parser_struct.num_list);
                        p_tmp_num = (uint64_t*)cl_list_obj(list_iterator);
                        if (*p_tmp_num > 15)
                        {
                            yyerror("illegal SL value");
                            return 1;
                        }

                        sl = (uint8_t)(*p_tmp_num);
                        free(p_tmp_num);
                        cl_list_remove_all(&tmp_parser_struct.num_list);

                        p_current_qos_match_rule->p_qos_level =
                                 &osm_qos_policy_simple_qos_levels[sl];
                        p_current_qos_match_rule->qos_level_name =
                                 strdup(osm_qos_policy_simple_qos_levels[sl].name);

                        if (__parser_ulp_match_rule_end())
                            return 1;
                    }
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1129 "osm_qos_parser_y.y"
    {
                            /* 'name' of 'port-group' - one instance */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            if (p_current_port_group->name)
                            {
                                yyerror("port-group has multiple 'name' tags");
                                cl_list_remove_all(&tmp_parser_struct.str_list);
                                return 1;
                            }

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            if ( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    p_current_port_group->name = tmp_str;
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1152 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1157 "osm_qos_parser_y.y"
    {
                            /* 'use' of 'port-group' - one instance */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            if (p_current_port_group->use)
                            {
                                yyerror("port-group has multiple 'use' tags");
                                cl_list_remove_all(&tmp_parser_struct.str_list);
                                return 1;
                            }

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            if ( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    p_current_port_group->use = tmp_str;
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1180 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1185 "osm_qos_parser_y.y"
    {
                            /* 'port-name' in 'port-group' - any num of instances */
                            cl_list_iterator_t list_iterator;
                            osm_node_t * p_node;
                            osm_physp_t * p_physp;
                            unsigned port_num;
                            char * tmp_str;
                            char * port_str;

                            /* parsing port name strings */
                            for (list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                                 list_iterator != cl_list_end(&tmp_parser_struct.str_list);
                                 list_iterator = cl_list_next(list_iterator))
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                {
                                    /* last slash in port name string is a separator
                                       between node name and port number */
                                    port_str = strrchr(tmp_str, '/');
                                    if (!port_str || (strlen(port_str) < 3) ||
                                        (port_str[1] != 'p' && port_str[1] != 'P')) {
                                        yyerror("'%s' - illegal port name",
                                                           tmp_str);
                                        free(tmp_str);
                                        cl_list_remove_all(&tmp_parser_struct.str_list);
                                        return 1;
                                    }

                                    if (!(port_num = strtoul(&port_str[2],NULL,0))) {
                                        yyerror(
                                               "'%s' - illegal port number in port name",
                                               tmp_str);
                                        free(tmp_str);
                                        cl_list_remove_all(&tmp_parser_struct.str_list);
                                        return 1;
                                    }

                                    /* separate node name from port number */
                                    port_str[0] = '\0';

                                    if (st_lookup(p_qos_policy->p_node_hash,
                                                  (st_data_t)tmp_str,
                                                  (void *)&p_node))
                                    {
                                        /* we found the node, now get the right port */
                                        p_physp = osm_node_get_physp_ptr(p_node, port_num);
                                        if (!p_physp) {
                                            yyerror(
                                                   "'%s' - port number out of range in port name",
                                                   tmp_str);
                                            free(tmp_str);
                                            cl_list_remove_all(&tmp_parser_struct.str_list);
                                            return 1;
                                        }
                                        /* we found the port, now add it to guid table */
                                        __parser_add_port_to_port_map(&p_current_port_group->port_map,
                                                                      p_physp);
                                    }
                                    free(tmp_str);
                                }
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1251 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1256 "osm_qos_parser_y.y"
    {
                            /* 'port-guid' in 'port-group' - any num of instances */
                            /* list of guid ranges */
                            if (cl_list_count(&tmp_parser_struct.num_pair_list))
                            {
                                uint64_t ** range_arr;
                                unsigned range_len;

                                __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                                      &range_arr,
                                                      &range_len );

                                __parser_add_guid_range_to_port_map(
                                                      &p_current_port_group->port_map,
                                                      range_arr,
                                                      range_len);
                            }
                        }
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1276 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1281 "osm_qos_parser_y.y"
    {
                            /* 'pkey' in 'port-group' - any num of instances */
                            /* list of pkey ranges */
                            if (cl_list_count(&tmp_parser_struct.num_pair_list))
                            {
                                uint64_t ** range_arr;
                                unsigned range_len;

                                __pkey_rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                                      &range_arr,
                                                      &range_len );

                                __parser_add_pkey_range_to_port_map(
                                                      &p_current_port_group->port_map,
                                                      range_arr,
                                                      range_len);
                            }
                        }
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1301 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1306 "osm_qos_parser_y.y"
    {
                            /* 'partition' in 'port-group' - any num of instances */
                            __parser_add_partition_list_to_port_map(
                                               &p_current_port_group->port_map,
                                               &tmp_parser_struct.str_list);
                        }
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1314 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1319 "osm_qos_parser_y.y"
    {
                            /* 'node-type' in 'port-group' - any num of instances */
                        }
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1324 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1340 "osm_qos_parser_y.y"
    {
                            p_current_port_group->node_types |=
                               OSM_QOS_POLICY_NODE_TYPE_CA;
                        }
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1346 "osm_qos_parser_y.y"
    {
                            p_current_port_group->node_types |=
                               OSM_QOS_POLICY_NODE_TYPE_SWITCH;
                        }
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1352 "osm_qos_parser_y.y"
    {
                            p_current_port_group->node_types |=
                               OSM_QOS_POLICY_NODE_TYPE_ROUTER;
                        }
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1358 "osm_qos_parser_y.y"
    {
                            p_current_port_group->node_types |=
                               (OSM_QOS_POLICY_NODE_TYPE_CA |
                                OSM_QOS_POLICY_NODE_TYPE_SWITCH |
                                OSM_QOS_POLICY_NODE_TYPE_ROUTER);
                        }
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1366 "osm_qos_parser_y.y"
    {
                            osm_port_t * p_osm_port =
                                osm_get_port_by_guid(p_qos_policy->p_subn,
                                     p_qos_policy->p_subn->sm_port_guid);
                            if (p_osm_port)
                                __parser_add_port_to_port_map(
                                   &p_current_port_group->port_map,
                                   p_osm_port->p_physp);
                        }
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1388 "osm_qos_parser_y.y"
    {
                            /* 'group' in 'vlarb-scope' - any num of instances */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    cl_list_insert_tail(&p_current_vlarb_scope->group_list,tmp_str);
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1405 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1410 "osm_qos_parser_y.y"
    {
                            /* 'across' in 'vlarb-scope' - any num of instances */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    cl_list_insert_tail(&p_current_vlarb_scope->across_list,tmp_str);
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1427 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1432 "osm_qos_parser_y.y"
    {
                            /* 'vl-high-limit' in 'vlarb-scope' - one instance of one number */
                            cl_list_iterator_t    list_iterator;
                            uint64_t            * p_tmp_num;

                            list_iterator = cl_list_head(&tmp_parser_struct.num_list);
                            p_tmp_num = (uint64_t*)cl_list_obj(list_iterator);
                            if (p_tmp_num)
                            {
                                p_current_vlarb_scope->vl_high_limit = (uint32_t)(*p_tmp_num);
                                p_current_vlarb_scope->vl_high_limit_set = TRUE;
                                free(p_tmp_num);
                            }

                            cl_list_remove_all(&tmp_parser_struct.num_list);
                        }
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1450 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1455 "osm_qos_parser_y.y"
    {
                            /* 'vlarb-high' in 'vlarb-scope' - list of pairs of numbers with ':' and ',' */
                            cl_list_iterator_t    list_iterator;
                            uint64_t            * num_pair;

                            list_iterator = cl_list_head(&tmp_parser_struct.num_pair_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.num_pair_list) )
                            {
                                num_pair = (uint64_t*)cl_list_obj(list_iterator);
                                if (num_pair)
                                    cl_list_insert_tail(&p_current_vlarb_scope->vlarb_high_list,num_pair);
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.num_pair_list);
                        }
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1472 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1477 "osm_qos_parser_y.y"
    {
                            /* 'vlarb-low' in 'vlarb-scope' - list of pairs of numbers with ':' and ',' */
                            cl_list_iterator_t    list_iterator;
                            uint64_t            * num_pair;

                            list_iterator = cl_list_head(&tmp_parser_struct.num_pair_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.num_pair_list) )
                            {
                                num_pair = (uint64_t*)cl_list_obj(list_iterator);
                                if (num_pair)
                                    cl_list_insert_tail(&p_current_vlarb_scope->vlarb_low_list,num_pair);
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.num_pair_list);
                        }
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1494 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1510 "osm_qos_parser_y.y"
    {
                            /* 'group' in 'sl2vl-scope' - any num of instances */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    cl_list_insert_tail(&p_current_sl2vl_scope->group_list,tmp_str);
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1527 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1532 "osm_qos_parser_y.y"
    {
                            /* 'across' in 'sl2vl-scope' - any num of instances */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str) {
                                    cl_list_insert_tail(&p_current_sl2vl_scope->across_from_list,tmp_str);
                                    cl_list_insert_tail(&p_current_sl2vl_scope->across_to_list,strdup(tmp_str));
                                }
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1551 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1556 "osm_qos_parser_y.y"
    {
                            /* 'across-from' in 'sl2vl-scope' - any num of instances */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    cl_list_insert_tail(&p_current_sl2vl_scope->across_from_list,tmp_str);
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1573 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1578 "osm_qos_parser_y.y"
    {
                            /* 'across-to' in 'sl2vl-scope' - any num of instances */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str) {
                                    cl_list_insert_tail(&p_current_sl2vl_scope->across_to_list,tmp_str);
                                }
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1596 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1601 "osm_qos_parser_y.y"
    {
                            /* 'from' in 'sl2vl-scope' - any num of instances */
                        }
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1606 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1611 "osm_qos_parser_y.y"
    {
                            /* 'to' in 'sl2vl-scope' - any num of instances */
                        }
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1616 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1625 "osm_qos_parser_y.y"
    {
                            int i;
                            for (i = 0; i < OSM_QOS_POLICY_MAX_PORTS_ON_SWITCH; i++)
                                p_current_sl2vl_scope->from[i] = TRUE;
                        }
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1636 "osm_qos_parser_y.y"
    {
                            int i;
                            for (i = 0; i < OSM_QOS_POLICY_MAX_PORTS_ON_SWITCH; i++)
                                p_current_sl2vl_scope->to[i] = TRUE;
                        }
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1643 "osm_qos_parser_y.y"
    {
                            int i;
                            cl_list_iterator_t    list_iterator;
                            uint64_t            * num_pair;
                            uint8_t               num1, num2;

                            list_iterator = cl_list_head(&tmp_parser_struct.num_pair_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.num_pair_list) )
                            {
                                num_pair = (uint64_t*)cl_list_obj(list_iterator);
                                if (num_pair)
                                {
                                    if ( num_pair[1] >= OSM_QOS_POLICY_MAX_PORTS_ON_SWITCH )
                                    {
                                        yyerror("port number out of range 'from' list");
                                        free(num_pair);
                                        cl_list_remove_all(&tmp_parser_struct.num_pair_list);
                                        return 1;
                                    }
                                    num1 = (uint8_t)num_pair[0];
                                    num2 = (uint8_t)num_pair[1];
                                    free(num_pair);
                                    for (i = num1; i <= num2; i++)
                                        p_current_sl2vl_scope->from[i] = TRUE;
                                }
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.num_pair_list);
                        }
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1674 "osm_qos_parser_y.y"
    {
                            int i;
                            cl_list_iterator_t    list_iterator;
                            uint64_t            * num_pair;
                            uint8_t               num1, num2;

                            list_iterator = cl_list_head(&tmp_parser_struct.num_pair_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.num_pair_list) )
                            {
                                num_pair = (uint64_t*)cl_list_obj(list_iterator);
                                if (num_pair)
                                {
                                    if ( num_pair[1] >= OSM_QOS_POLICY_MAX_PORTS_ON_SWITCH )
                                    {
                                        yyerror("port number out of range 'to' list");
                                        free(num_pair);
                                        cl_list_remove_all(&tmp_parser_struct.num_pair_list);
                                        return 1;
                                    }
                                    num1 = (uint8_t)num_pair[0];
                                    num2 = (uint8_t)num_pair[1];
                                    free(num_pair);
                                    for (i = num1; i <= num2; i++)
                                        p_current_sl2vl_scope->to[i] = TRUE;
                                }
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.num_pair_list);
                        }
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1706 "osm_qos_parser_y.y"
    {
                            /* 'sl2vl-table' - one instance of exactly
                               OSM_QOS_POLICY_SL2VL_TABLE_LEN numbers */
                            cl_list_iterator_t    list_iterator;
                            uint64_t              num;
                            uint64_t            * p_num;
                            int                   i = 0;

                            if (p_current_sl2vl_scope->sl2vl_table_set)
                            {
                                yyerror("sl2vl-scope has more than one sl2vl-table");
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }

                            if (cl_list_count(&tmp_parser_struct.num_list) != OSM_QOS_POLICY_SL2VL_TABLE_LEN)
                            {
                                yyerror("wrong number of values in 'sl2vl-table' (should be 16)");
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }

                            list_iterator = cl_list_head(&tmp_parser_struct.num_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.num_list) )
                            {
                                p_num = (uint64_t*)cl_list_obj(list_iterator);
                                num = *p_num;
                                free(p_num);
                                if (num >= OSM_QOS_POLICY_MAX_VL_NUM)
                                {
                                    yyerror("wrong VL value in 'sl2vl-table' (should be 0 to 15)");
                                    cl_list_remove_all(&tmp_parser_struct.num_list);
                                    return 1;
                                }

                                p_current_sl2vl_scope->sl2vl_table[i++] = (uint8_t)num;
                                list_iterator = cl_list_next(list_iterator);
                            }
                            p_current_sl2vl_scope->sl2vl_table_set = TRUE;
                            cl_list_remove_all(&tmp_parser_struct.num_list);
                        }
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1749 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1766 "osm_qos_parser_y.y"
    {
                            /* 'name' of 'qos-level' - one instance */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            if (p_current_qos_level->name)
                            {
                                yyerror("qos-level has multiple 'name' tags");
                                cl_list_remove_all(&tmp_parser_struct.str_list);
                                return 1;
                            }

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            if ( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    p_current_qos_level->name = tmp_str;
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1789 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1794 "osm_qos_parser_y.y"
    {
                            /* 'use' of 'qos-level' - one instance */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            if (p_current_qos_level->use)
                            {
                                yyerror("qos-level has multiple 'use' tags");
                                cl_list_remove_all(&tmp_parser_struct.str_list);
                                return 1;
                            }

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            if ( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    p_current_qos_level->use = tmp_str;
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1817 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1822 "osm_qos_parser_y.y"
    {
                            /* 'sl' in 'qos-level' - one instance */
                            cl_list_iterator_t   list_iterator;
                            uint64_t           * p_num;

                            if (p_current_qos_level->sl_set)
                            {
                                yyerror("'qos-level' has multiple 'sl' tags");
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }
                            list_iterator = cl_list_head(&tmp_parser_struct.num_list);
                            p_num = (uint64_t*)cl_list_obj(list_iterator);
                            p_current_qos_level->sl = (uint8_t)(*p_num);
                            free(p_num);
                            p_current_qos_level->sl_set = TRUE;
                            cl_list_remove_all(&tmp_parser_struct.num_list);
                        }
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1842 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1847 "osm_qos_parser_y.y"
    {
                            /* 'mtu-limit' in 'qos-level' - one instance */
                            cl_list_iterator_t   list_iterator;
                            uint64_t           * p_num;

                            if (p_current_qos_level->mtu_limit_set)
                            {
                                yyerror("'qos-level' has multiple 'mtu-limit' tags");
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }
                            list_iterator = cl_list_head(&tmp_parser_struct.num_list);
                            p_num = (uint64_t*)cl_list_obj(list_iterator);
                            if (*p_num > OSM_QOS_POLICY_MAX_MTU || *p_num < OSM_QOS_POLICY_MIN_MTU)
                            {
                                yyerror("mtu limit is out of range, value: %d", *p_num);
                                free(p_num);
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }
                            p_current_qos_level->mtu_limit = (uint8_t)(*p_num);
                            free(p_num);
                            p_current_qos_level->mtu_limit_set = TRUE;
                            cl_list_remove_all(&tmp_parser_struct.num_list);
                        }
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1874 "osm_qos_parser_y.y"
    {
                            /* 'mtu-limit' in 'qos-level' - one instance */
                            RESET_BUFFER;
                        }
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1880 "osm_qos_parser_y.y"
    {
                            /* 'rate-limit' in 'qos-level' - one instance */
                            cl_list_iterator_t   list_iterator;
                            uint64_t           * p_num;

                            if (p_current_qos_level->rate_limit_set)
                            {
                                yyerror("'qos-level' has multiple 'rate-limit' tags");
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }
                            list_iterator = cl_list_head(&tmp_parser_struct.num_list);
                            p_num = (uint64_t*)cl_list_obj(list_iterator);
                            if (*p_num > OSM_QOS_POLICY_MAX_RATE || *p_num < OSM_QOS_POLICY_MIN_RATE)
                            {
                                yyerror("rate limit is out of range, value: %d", *p_num);
                                free(p_num);
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }
                            p_current_qos_level->rate_limit = (uint8_t)(*p_num);
                            free(p_num);
                            p_current_qos_level->rate_limit_set = TRUE;
                            cl_list_remove_all(&tmp_parser_struct.num_list);
                        }
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1907 "osm_qos_parser_y.y"
    {
                            /* 'rate-limit' in 'qos-level' - one instance */
                            RESET_BUFFER;
                        }
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1913 "osm_qos_parser_y.y"
    {
                            /* 'packet-life' in 'qos-level' - one instance */
                            cl_list_iterator_t   list_iterator;
                            uint64_t           * p_num;

                            if (p_current_qos_level->pkt_life_set)
                            {
                                yyerror("'qos-level' has multiple 'packet-life' tags");
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }
                            list_iterator = cl_list_head(&tmp_parser_struct.num_list);
                            p_num = (uint64_t*)cl_list_obj(list_iterator);
                            p_current_qos_level->pkt_life = (uint8_t)(*p_num);
                            free(p_num);
                            p_current_qos_level->pkt_life_set= TRUE;
                            cl_list_remove_all(&tmp_parser_struct.num_list);
                        }
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1933 "osm_qos_parser_y.y"
    {
                            /* 'packet-life' in 'qos-level' - one instance */
                            RESET_BUFFER;
                        }
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1939 "osm_qos_parser_y.y"
    {
                            /* 'path-bits' in 'qos-level' - any num of instances */
                            /* list of path bit ranges */

                            if (cl_list_count(&tmp_parser_struct.num_pair_list))
                            {
                                uint64_t ** range_arr;
                                unsigned range_len;

                                __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                                      &range_arr,
                                                      &range_len );

                                if ( !p_current_qos_level->path_bits_range_len )
                                {
                                    p_current_qos_level->path_bits_range_arr = range_arr;
                                    p_current_qos_level->path_bits_range_len = range_len;
                                }
                                else
                                {
                                    uint64_t ** new_range_arr;
                                    unsigned new_range_len;
                                    __merge_rangearr( p_current_qos_level->path_bits_range_arr,
                                                      p_current_qos_level->path_bits_range_len,
                                                      range_arr,
                                                      range_len,
                                                      &new_range_arr,
                                                      &new_range_len );
                                    p_current_qos_level->path_bits_range_arr = new_range_arr;
                                    p_current_qos_level->path_bits_range_len = new_range_len;
                                }
                            }
                        }
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1974 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1979 "osm_qos_parser_y.y"
    {
                            /* 'pkey' in 'qos-level' - num of instances of list of ranges */
                            if (cl_list_count(&tmp_parser_struct.num_pair_list))
                            {
                                uint64_t ** range_arr;
                                unsigned range_len;

                                __pkey_rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                                      &range_arr,
                                                      &range_len );

                                if ( !p_current_qos_level->pkey_range_len )
                                {
                                    p_current_qos_level->pkey_range_arr = range_arr;
                                    p_current_qos_level->pkey_range_len = range_len;
                                }
                                else
                                {
                                    uint64_t ** new_range_arr;
                                    unsigned new_range_len;
                                    __merge_rangearr( p_current_qos_level->pkey_range_arr,
                                                      p_current_qos_level->pkey_range_len,
                                                      range_arr,
                                                      range_len,
                                                      &new_range_arr,
                                                      &new_range_len );
                                    p_current_qos_level->pkey_range_arr = new_range_arr;
                                    p_current_qos_level->pkey_range_len = new_range_len;
                                }
                            }
                        }
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 2012 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 2029 "osm_qos_parser_y.y"
    {
                            /* 'use' of 'qos-match-rule' - one instance */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            if (p_current_qos_match_rule->use)
                            {
                                yyerror("'qos-match-rule' has multiple 'use' tags");
                                cl_list_remove_all(&tmp_parser_struct.str_list);
                                return 1;
                            }

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            if ( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    p_current_qos_match_rule->use = tmp_str;
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 2052 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 2057 "osm_qos_parser_y.y"
    {
                            /* 'qos-class' in 'qos-match-rule' - num of instances of list of ranges */
                            /* list of class ranges (QoS Class is 12-bit value) */
                            if (cl_list_count(&tmp_parser_struct.num_pair_list))
                            {
                                uint64_t ** range_arr;
                                unsigned range_len;

                                __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                                      &range_arr,
                                                      &range_len );

                                if ( !p_current_qos_match_rule->qos_class_range_len )
                                {
                                    p_current_qos_match_rule->qos_class_range_arr = range_arr;
                                    p_current_qos_match_rule->qos_class_range_len = range_len;
                                }
                                else
                                {
                                    uint64_t ** new_range_arr;
                                    unsigned new_range_len;
                                    __merge_rangearr( p_current_qos_match_rule->qos_class_range_arr,
                                                      p_current_qos_match_rule->qos_class_range_len,
                                                      range_arr,
                                                      range_len,
                                                      &new_range_arr,
                                                      &new_range_len );
                                    p_current_qos_match_rule->qos_class_range_arr = new_range_arr;
                                    p_current_qos_match_rule->qos_class_range_len = new_range_len;
                                }
                            }
                        }
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 2091 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 2096 "osm_qos_parser_y.y"
    {
                            /* 'source' in 'qos-match-rule' - text */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    cl_list_insert_tail(&p_current_qos_match_rule->source_list,tmp_str);
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 2113 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 2118 "osm_qos_parser_y.y"
    {
                            /* 'destination' in 'qos-match-rule' - text */
                            cl_list_iterator_t    list_iterator;
                            char                * tmp_str;

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            while( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    cl_list_insert_tail(&p_current_qos_match_rule->destination_list,tmp_str);
                                list_iterator = cl_list_next(list_iterator);
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 2135 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 2140 "osm_qos_parser_y.y"
    {
                            /* 'qos-level-name' in 'qos-match-rule' - single string */
                            cl_list_iterator_t   list_iterator;
                            char               * tmp_str;

                            if (p_current_qos_match_rule->qos_level_name)
                            {
                                yyerror("qos-match-rule has multiple 'qos-level-name' tags");
                                cl_list_remove_all(&tmp_parser_struct.num_list);
                                return 1;
                            }

                            list_iterator = cl_list_head(&tmp_parser_struct.str_list);
                            if ( list_iterator != cl_list_end(&tmp_parser_struct.str_list) )
                            {
                                tmp_str = (char*)cl_list_obj(list_iterator);
                                if (tmp_str)
                                    p_current_qos_match_rule->qos_level_name = tmp_str;
                            }
                            cl_list_remove_all(&tmp_parser_struct.str_list);
                        }
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 2163 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 2168 "osm_qos_parser_y.y"
    {
                            /* 'service-id' in 'qos-match-rule' - num of instances of list of ranges */
                            if (cl_list_count(&tmp_parser_struct.num_pair_list))
                            {
                                uint64_t ** range_arr;
                                unsigned range_len;

                                __rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                                      &range_arr,
                                                      &range_len );

                                if ( !p_current_qos_match_rule->service_id_range_len )
                                {
                                    p_current_qos_match_rule->service_id_range_arr = range_arr;
                                    p_current_qos_match_rule->service_id_range_len = range_len;
                                }
                                else
                                {
                                    uint64_t ** new_range_arr;
                                    unsigned new_range_len;
                                    __merge_rangearr( p_current_qos_match_rule->service_id_range_arr,
                                                      p_current_qos_match_rule->service_id_range_len,
                                                      range_arr,
                                                      range_len,
                                                      &new_range_arr,
                                                      &new_range_len );
                                    p_current_qos_match_rule->service_id_range_arr = new_range_arr;
                                    p_current_qos_match_rule->service_id_range_len = new_range_len;
                                }
                            }
                        }
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 2201 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 2206 "osm_qos_parser_y.y"
    {
                            /* 'pkey' in 'qos-match-rule' - num of instances of list of ranges */
                            if (cl_list_count(&tmp_parser_struct.num_pair_list))
                            {
                                uint64_t ** range_arr;
                                unsigned range_len;

                                __pkey_rangelist2rangearr( &tmp_parser_struct.num_pair_list,
                                                      &range_arr,
                                                      &range_len );

                                if ( !p_current_qos_match_rule->pkey_range_len )
                                {
                                    p_current_qos_match_rule->pkey_range_arr = range_arr;
                                    p_current_qos_match_rule->pkey_range_len = range_len;
                                }
                                else
                                {
                                    uint64_t ** new_range_arr;
                                    unsigned new_range_len;
                                    __merge_rangearr( p_current_qos_match_rule->pkey_range_arr,
                                                      p_current_qos_match_rule->pkey_range_len,
                                                      range_arr,
                                                      range_len,
                                                      &new_range_arr,
                                                      &new_range_len );
                                    p_current_qos_match_rule->pkey_range_arr = new_range_arr;
                                    p_current_qos_match_rule->pkey_range_len = new_range_len;
                                }
                            }
                        }
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 2239 "osm_qos_parser_y.y"
    {
                            RESET_BUFFER;
                        }
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 2250 "osm_qos_parser_y.y"
    {
                        cl_list_insert_tail(&tmp_parser_struct.str_list,
                                            strdup(__parser_strip_white(tmp_parser_struct.str)));
                        tmp_parser_struct.str[0] = '\0';
                    }
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 2261 "osm_qos_parser_y.y"
    {
                        strcat(tmp_parser_struct.str,(yyvsp[(1) - (1)]));
                        free((yyvsp[(1) - (1)]));
                    }
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 2281 "osm_qos_parser_y.y"
    {
                        uint64_t * p_num = (uint64_t*)malloc(sizeof(uint64_t));
                        __parser_str2uint64(p_num,(yyvsp[(1) - (1)]));
                        free((yyvsp[(1) - (1)]));
                        cl_list_insert_tail(&tmp_parser_struct.num_list, p_num);
                    }
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 2289 "osm_qos_parser_y.y"
    {
                        uint64_t * num_pair = (uint64_t*)malloc(sizeof(uint64_t)*2);
                        num_pair[0] = tmp_parser_struct.num_pair[0];
                        num_pair[1] = tmp_parser_struct.num_pair[1];
                        cl_list_insert_tail(&tmp_parser_struct.num_pair_list, num_pair);
                    }
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 2295 "osm_qos_parser_y.y"
    {
                        uint64_t * num_pair = (uint64_t*)malloc(sizeof(uint64_t)*2);
                        num_pair[0] = tmp_parser_struct.num_pair[0];
                        num_pair[1] = tmp_parser_struct.num_pair[1];
                        cl_list_insert_tail(&tmp_parser_struct.num_pair_list, num_pair);
                    }
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 2303 "osm_qos_parser_y.y"
    {
                        __parser_str2uint64(&tmp_parser_struct.num_pair[0],(yyvsp[(1) - (1)]));
                        free((yyvsp[(1) - (1)]));
                    }
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 2309 "osm_qos_parser_y.y"
    {
                        __parser_str2uint64(&tmp_parser_struct.num_pair[1],(yyvsp[(1) - (1)]));
                        free((yyvsp[(1) - (1)]));
                    }
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 2318 "osm_qos_parser_y.y"
    {
                        uint64_t * num_pair = (uint64_t*)malloc(sizeof(uint64_t)*2);
                        num_pair[0] = tmp_parser_struct.num_pair[0];
                        num_pair[1] = tmp_parser_struct.num_pair[1];
                        cl_list_insert_tail(&tmp_parser_struct.num_pair_list, num_pair);
                    }
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 2324 "osm_qos_parser_y.y"
    {
                        uint64_t * num_pair = (uint64_t*)malloc(sizeof(uint64_t)*2);
                        if (tmp_parser_struct.num_pair[0] <= tmp_parser_struct.num_pair[1]) {
                            num_pair[0] = tmp_parser_struct.num_pair[0];
                            num_pair[1] = tmp_parser_struct.num_pair[1];
                        }
                        else {
                            num_pair[1] = tmp_parser_struct.num_pair[0];
                            num_pair[0] = tmp_parser_struct.num_pair[1];
                        }
                        cl_list_insert_tail(&tmp_parser_struct.num_pair_list, num_pair);
                    }
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 2336 "osm_qos_parser_y.y"
    {
                        uint64_t * num_pair = (uint64_t*)malloc(sizeof(uint64_t)*2);
                        if (tmp_parser_struct.num_pair[0] <= tmp_parser_struct.num_pair[1]) {
                            num_pair[0] = tmp_parser_struct.num_pair[0];
                            num_pair[1] = tmp_parser_struct.num_pair[1];
                        }
                        else {
                            num_pair[1] = tmp_parser_struct.num_pair[0];
                            num_pair[0] = tmp_parser_struct.num_pair[1];
                        }
                        cl_list_insert_tail(&tmp_parser_struct.num_pair_list, num_pair);
                    }
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 2348 "osm_qos_parser_y.y"
    {
                        uint64_t * num_pair = (uint64_t*)malloc(sizeof(uint64_t)*2);
                        num_pair[0] = tmp_parser_struct.num_pair[0];
                        num_pair[1] = tmp_parser_struct.num_pair[1];
                        cl_list_insert_tail(&tmp_parser_struct.num_pair_list, num_pair);
                    }
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 2356 "osm_qos_parser_y.y"
    {
                        __parser_str2uint64(&tmp_parser_struct.num_pair[0],(yyvsp[(1) - (1)]));
                        __parser_str2uint64(&tmp_parser_struct.num_pair[1],(yyvsp[(1) - (1)]));
                        free((yyvsp[(1) - (1)]));
                    }
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 2363 "osm_qos_parser_y.y"
    {
                        __parser_str2uint64(&tmp_parser_struct.num_pair[0],(yyvsp[(1) - (1)]));
                        free((yyvsp[(1) - (1)]));
                    }
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 2369 "osm_qos_parser_y.y"
    {
                        __parser_str2uint64(&tmp_parser_struct.num_pair[1],(yyvsp[(1) - (1)]));
                        free((yyvsp[(1) - (1)]));
                    }
    break;



/* Line 1455 of yacc.c  */
#line 4305 "osm_qos_parser_y.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */