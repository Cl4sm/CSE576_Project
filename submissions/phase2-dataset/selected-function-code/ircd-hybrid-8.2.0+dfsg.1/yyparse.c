yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 28:
#line 420 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = 0; }
#line 2876 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 421 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-1].number) + (yyvsp[0].number); }
#line 2882 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 422 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) + (yyvsp[0].number); }
#line 2888 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 423 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * 60 + (yyvsp[0].number); }
#line 2894 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 424 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * 60 * 60 + (yyvsp[0].number); }
#line 2900 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 425 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * 60 * 60 * 24 + (yyvsp[0].number); }
#line 2906 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 426 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * 60 * 60 * 24 * 7 + (yyvsp[0].number); }
#line 2912 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 427 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * 60 * 60 * 24 * 7 * 4 + (yyvsp[0].number); }
#line 2918 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 428 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * 60 * 60 * 24 * 365 + (yyvsp[0].number); }
#line 2924 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 431 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = 0; }
#line 2930 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 432 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-1].number) + (yyvsp[0].number); }
#line 2936 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 433 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) + (yyvsp[0].number); }
#line 2942 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 434 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * 1024 + (yyvsp[0].number); }
#line 2948 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 435 "conf_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * 1024 * 1024 + (yyvsp[0].number); }
#line 2954 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 449 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    add_conf_module(libio_basename(yylval.string));
}
#line 2963 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 455 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    mod_add_path(yylval.string);
}
#line 2972 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 485 "conf_parser.y" /* yacc.c:1646  */
    {
#ifdef HAVE_LIBCRYPTO
  if (conf_parser_ctx.pass == 2)
  {
    if (!ConfigServerInfo.rsa_private_key_file)
    {
      conf_error_report("No rsa_private_key_file specified, SSL disabled");
      break;
    }

    if (SSL_CTX_use_certificate_chain_file(ConfigServerInfo.server_ctx,
					   yylval.string) <= 0 ||
        SSL_CTX_use_certificate_chain_file(ConfigServerInfo.client_ctx,
					   yylval.string) <= 0)
    {
      report_crypto_errors();
      conf_error_report("Could not open/read certificate file");
      break;
    }

    if (SSL_CTX_use_PrivateKey_file(ConfigServerInfo.server_ctx, ConfigServerInfo.rsa_private_key_file,
                                    SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ConfigServerInfo.client_ctx, ConfigServerInfo.rsa_private_key_file,
                                    SSL_FILETYPE_PEM) <= 0)
    {
      report_crypto_errors();
      conf_error_report("Could not read RSA private key");
      break;
    }

    if (!SSL_CTX_check_private_key(ConfigServerInfo.server_ctx) ||
        !SSL_CTX_check_private_key(ConfigServerInfo.client_ctx))
    {
      report_crypto_errors();
      conf_error_report("Could not read RSA private key");
      break;
    }
  }
#endif
}
#line 3017 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 527 "conf_parser.y" /* yacc.c:1646  */
    {
#ifdef HAVE_LIBCRYPTO
  BIO *file = NULL;

  if (conf_parser_ctx.pass != 1)
    break;

  if (ConfigServerInfo.rsa_private_key)
  {
    RSA_free(ConfigServerInfo.rsa_private_key);
    ConfigServerInfo.rsa_private_key = NULL;
  }

  if (ConfigServerInfo.rsa_private_key_file)
  {
    MyFree(ConfigServerInfo.rsa_private_key_file);
    ConfigServerInfo.rsa_private_key_file = NULL;
  }

  ConfigServerInfo.rsa_private_key_file = xstrdup(yylval.string);

  if ((file = BIO_new_file(yylval.string, "r")) == NULL)
  {
    conf_error_report("File open failed, ignoring");
    break;
  }

  ConfigServerInfo.rsa_private_key = PEM_read_bio_RSAPrivateKey(file, NULL, 0, NULL);

  BIO_set_close(file, BIO_CLOSE);
  BIO_free(file);

  if (ConfigServerInfo.rsa_private_key == NULL)
  {
    conf_error_report("Couldn't extract key, ignoring");
    break;
  }

  if (!RSA_check_key(ConfigServerInfo.rsa_private_key))
  {
    RSA_free(ConfigServerInfo.rsa_private_key);
    ConfigServerInfo.rsa_private_key = NULL;

    conf_error_report("Invalid key, ignoring");
    break;
  }

  if (RSA_size(ConfigServerInfo.rsa_private_key) < 128)
  {
    RSA_free(ConfigServerInfo.rsa_private_key);
    ConfigServerInfo.rsa_private_key = NULL;

    conf_error_report("Ignoring serverinfo::rsa_private_key_file -- need at least a 1024 bit key size");
  }
#endif
}
#line 3078 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 585 "conf_parser.y" /* yacc.c:1646  */
    {
#ifdef HAVE_LIBCRYPTO
  if (conf_parser_ctx.pass == 2)
  {
    BIO *file = BIO_new_file(yylval.string, "r");

    if (file)
    {
      DH *dh = PEM_read_bio_DHparams(file, NULL, NULL, NULL);

      BIO_free(file);

      if (dh)
      {
        if (DH_size(dh) < 128)
          conf_error_report("Ignoring serverinfo::ssl_dh_param_file -- need at least a 1024 bit DH prime size");
        else
          SSL_CTX_set_tmp_dh(ConfigServerInfo.server_ctx, dh);

        DH_free(dh);
      }
    }
    else
      conf_error_report("Ignoring serverinfo::ssl_dh_param_file -- could not open/read Diffie-Hellman parameter file");
  }
#endif
}
#line 3110 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 614 "conf_parser.y" /* yacc.c:1646  */
    {
#ifdef HAVE_LIBCRYPTO
  if (conf_parser_ctx.pass == 2)
    SSL_CTX_set_cipher_list(ConfigServerInfo.server_ctx, yylval.string);
#endif
}
#line 3121 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 622 "conf_parser.y" /* yacc.c:1646  */
    {
#ifdef HAVE_LIBCRYPTO
  if (conf_parser_ctx.pass == 2)
  {
    if ((ConfigServerInfo.message_digest_algorithm = EVP_get_digestbyname(yylval.string)) == NULL)
    {
      ConfigServerInfo.message_digest_algorithm = EVP_sha256();
      conf_error_report("Ignoring serverinfo::ssl_message_digest_algorithm -- unknown message digest algorithm");
    }
  }
#endif
}
#line 3138 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 636 "conf_parser.y" /* yacc.c:1646  */
    {
#ifdef HAVE_LIBCRYPTO
#if OPENSSL_VERSION_NUMBER >= 0x1000005FL && !defined(OPENSSL_NO_ECDH)
  int nid = 0;
  EC_KEY *key = NULL;

  if (conf_parser_ctx.pass == 2)
  {
    if ((nid = OBJ_sn2nid(yylval.string)) == 0)
    {
        conf_error_report("Ignoring serverinfo::serverinfo_ssl_dh_elliptic_curve -- unknown curve name");
        break;
    }

    if ((key = EC_KEY_new_by_curve_name(nid)) == NULL)
    {
      conf_error_report("Ignoring serverinfo::serverinfo_ssl_dh_elliptic_curve -- could not create curve");
      break;
    }

    SSL_CTX_set_tmp_ecdh(ConfigServerInfo.server_ctx, key);
    EC_KEY_free(key);
}
#endif
#endif
}
#line 3169 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 664 "conf_parser.y" /* yacc.c:1646  */
    {
  /* this isn't rehashable */
  if (conf_parser_ctx.pass == 2 && !ConfigServerInfo.name)
  {
    if (valid_servname(yylval.string))
      ConfigServerInfo.name = xstrdup(yylval.string);
    else
    {
      conf_error_report("Ignoring serverinfo::name -- invalid name. Aborting.");
      exit(0);
    }
  }
}
#line 3187 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 679 "conf_parser.y" /* yacc.c:1646  */
    {
  /* this isn't rehashable */
  if (conf_parser_ctx.pass == 2 && !ConfigServerInfo.sid)
  {
    if (valid_sid(yylval.string))
      ConfigServerInfo.sid = xstrdup(yylval.string);
    else
    {
      conf_error_report("Ignoring serverinfo::sid -- invalid SID. Aborting.");
      exit(0);
    }
  }
}
#line 3205 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 694 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    MyFree(ConfigServerInfo.description);
    ConfigServerInfo.description = xstrdup(yylval.string);
    strlcpy(me.info, ConfigServerInfo.description, sizeof(me.info));
  }
}
#line 3218 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 704 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    char *p;

    if ((p = strchr(yylval.string, ' ')))
      *p = '\0';

    MyFree(ConfigServerInfo.network_name);
    ConfigServerInfo.network_name = xstrdup(yylval.string);
  }
}
#line 3235 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 718 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  MyFree(ConfigServerInfo.network_desc);
  ConfigServerInfo.network_desc = xstrdup(yylval.string);
}
#line 3247 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 727 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2 && *yylval.string != '*')
  {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE | AI_NUMERICHOST;

    if (getaddrinfo(yylval.string, NULL, &hints, &res))
      ilog(LOG_TYPE_IRCD, "Invalid netmask for server vhost(%s)", yylval.string);
    else
    {
      assert(res);

      memcpy(&ConfigServerInfo.ip, res->ai_addr, res->ai_addrlen);
      ConfigServerInfo.ip.ss.ss_family = res->ai_family;
      ConfigServerInfo.ip.ss_len = res->ai_addrlen;
      freeaddrinfo(res);

      ConfigServerInfo.specific_ipv4_vhost = 1;
    }
  }
}
#line 3278 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 755 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2 && *yylval.string != '*')
  {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE | AI_NUMERICHOST;

    if (getaddrinfo(yylval.string, NULL, &hints, &res))
      ilog(LOG_TYPE_IRCD, "Invalid netmask for server vhost6(%s)", yylval.string);
    else
    {
      assert(res);

      memcpy(&ConfigServerInfo.ip6, res->ai_addr, res->ai_addrlen);
      ConfigServerInfo.ip6.ss.ss_family = res->ai_family;
      ConfigServerInfo.ip6.ss_len = res->ai_addrlen;
      freeaddrinfo(res);

      ConfigServerInfo.specific_ipv6_vhost = 1;
    }
  }
}
#line 3309 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 783 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if ((yyvsp[-1].number) < MAXCLIENTS_MIN)
  {
    char buf[IRCD_BUFSIZE] = "";

    snprintf(buf, sizeof(buf), "MAXCLIENTS too low, setting to %d", MAXCLIENTS_MIN);
    conf_error_report(buf);
    ConfigServerInfo.max_clients = MAXCLIENTS_MIN;
  }
  else if ((yyvsp[-1].number) > MAXCLIENTS_MAX)
  {
    char buf[IRCD_BUFSIZE] = "";

    snprintf(buf, sizeof(buf), "MAXCLIENTS too high, setting to %d", MAXCLIENTS_MAX);
    conf_error_report(buf);
    ConfigServerInfo.max_clients = MAXCLIENTS_MAX;
  }
  else
    ConfigServerInfo.max_clients = (yyvsp[-1].number);
}
#line 3337 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 808 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if ((yyvsp[-1].number) < 9)
  {
    conf_error_report("max_nick_length too low, setting to 9");
    ConfigServerInfo.max_nick_length = 9;
  }
  else if ((yyvsp[-1].number) > NICKLEN)
  {
    char buf[IRCD_BUFSIZE] = "";

    snprintf(buf, sizeof(buf), "max_nick_length too high, setting to %d", NICKLEN);
    conf_error_report(buf);
    ConfigServerInfo.max_nick_length = NICKLEN;
  }
  else
    ConfigServerInfo.max_nick_length = (yyvsp[-1].number);
}
#line 3362 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 830 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if ((yyvsp[-1].number) < 80)
  {
    conf_error_report("max_topic_length too low, setting to 80");
    ConfigServerInfo.max_topic_length = 80;
  }
  else if ((yyvsp[-1].number) > TOPICLEN)
  {
    char buf[IRCD_BUFSIZE] = "";

    snprintf(buf, sizeof(buf), "max_topic_length too high, setting to %d", TOPICLEN);
    conf_error_report(buf);
    ConfigServerInfo.max_topic_length = TOPICLEN;
  }
  else
    ConfigServerInfo.max_topic_length = (yyvsp[-1].number);
}
#line 3387 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 852 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigServerInfo.hub = yylval.number;
}
#line 3396 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 869 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  MyFree(ConfigAdminInfo.name);
  ConfigAdminInfo.name = xstrdup(yylval.string);
}
#line 3408 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 878 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  MyFree(ConfigAdminInfo.email);
  ConfigAdminInfo.email = xstrdup(yylval.string);
}
#line 3420 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 887 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  MyFree(ConfigAdminInfo.description);
  ConfigAdminInfo.description = xstrdup(yylval.string);
}
#line 3432 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 899 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    reset_block_state();
}
#line 3441 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 903 "conf_parser.y" /* yacc.c:1646  */
    {
  dlink_node *ptr = NULL;

  if (conf_parser_ctx.pass != 2)
    break;

  if (!block_state.file.buf[0])
    break;

  DLINK_FOREACH(ptr, block_state.mask.list.head)
    motd_add(ptr->data, block_state.file.buf);
}
#line 3458 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 920 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    dlinkAdd(xstrdup(yylval.string), make_dlink_node(), &block_state.mask.list);
}
#line 3467 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 926 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.file.buf, yylval.string, sizeof(block_state.file.buf));
}
#line 3476 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 935 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    reset_block_state();
}
#line 3485 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 939 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if (!block_state.command.buf[0] ||
      !block_state.name.buf[0] ||
      !block_state.nick.buf[0] ||
      !block_state.host.buf[0])
    break;

  pseudo_register(block_state.name.buf, block_state.nick.buf, block_state.host.buf,
                  block_state.prepend.buf, block_state.command.buf);
}
#line 3503 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 957 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.command.buf, yylval.string, sizeof(block_state.command.buf));
}
#line 3512 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 963 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
}
#line 3521 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 969 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.prepend.buf, yylval.string, sizeof(block_state.prepend.buf));
}
#line 3530 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 975 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    struct split_nuh_item nuh;

    nuh.nuhmask  = yylval.string;
    nuh.nickptr  = NULL;
    nuh.userptr  = block_state.user.buf;
    nuh.hostptr  = block_state.host.buf;
    nuh.nicksize = 0;
    nuh.usersize = sizeof(block_state.nick.buf);
    nuh.hostsize = sizeof(block_state.host.buf);
    split_nuh(&nuh);

    strlcpy(block_state.nick.buf, nuh.userptr, sizeof(block_state.nick.buf));
    strlcpy(block_state.host.buf, nuh.hostptr, sizeof(block_state.host.buf));
  }
}
#line 3553 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1004 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigLog.use_logging = yylval.number;
}
#line 3562 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1010 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    reset_block_state();
}
#line 3571 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1014 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if (block_state.type.value && block_state.file.buf[0])
    log_set_file(block_state.type.value, block_state.size.value,
                 block_state.file.buf);
}
#line 3584 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1030 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  strlcpy(block_state.file.buf, yylval.string, sizeof(block_state.file.buf));
}
#line 3595 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1038 "conf_parser.y" /* yacc.c:1646  */
    {
  block_state.size.value = (yyvsp[-1].number);
}
#line 3603 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1041 "conf_parser.y" /* yacc.c:1646  */
    {
  block_state.size.value = 0;
}
#line 3611 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 1046 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = 0;
}
#line 3620 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1053 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_USER;
}
#line 3629 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1057 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_OPER;
}
#line 3638 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1061 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_GLINE;
}
#line 3647 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1065 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_XLINE;
}
#line 3656 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 1069 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_RESV;
}
#line 3665 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 1073 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_DLINE;
}
#line 3674 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 1077 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_KLINE;
}
#line 3683 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 1081 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_KILL;
}
#line 3692 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 1085 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.type.value = LOG_TYPE_DEBUG;
}
#line 3701 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 1095 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  reset_block_state();
  block_state.flags.value |= CONF_FLAGS_ENCRYPTED;
}
#line 3713 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 1102 "conf_parser.y" /* yacc.c:1646  */
    {
  dlink_node *ptr = NULL;

  if (conf_parser_ctx.pass != 2)
    break;

  if (!block_state.name.buf[0])
    break;
#ifdef HAVE_LIBCRYPTO
  if (!block_state.file.buf[0] &&
      !block_state.rpass.buf[0])
    break;
#else
  if (!block_state.rpass.buf[0])
    break;
#endif

  DLINK_FOREACH(ptr, block_state.mask.list.head)
  {
    struct MaskItem *conf = NULL;
    struct split_nuh_item nuh;

    nuh.nuhmask  = ptr->data;
    nuh.nickptr  = NULL;
    nuh.userptr  = block_state.user.buf;
    nuh.hostptr  = block_state.host.buf;
    nuh.nicksize = 0;
    nuh.usersize = sizeof(block_state.user.buf);
    nuh.hostsize = sizeof(block_state.host.buf);
    split_nuh(&nuh);

    conf         = conf_make(CONF_OPER);
    conf->name   = xstrdup(block_state.name.buf);
    conf->user   = xstrdup(block_state.user.buf);
    conf->host   = xstrdup(block_state.host.buf);

    if (block_state.cert.buf[0])
      conf->certfp = xstrdup(block_state.cert.buf);

    if (block_state.rpass.buf[0])
      conf->passwd = xstrdup(block_state.rpass.buf);

    conf->flags = block_state.flags.value;
    conf->modes = block_state.modes.value;
    conf->port  = block_state.port.value;
    conf->htype = parse_netmask(conf->host, &conf->addr, &conf->bits);

    conf_add_class_to_conf(conf, block_state.class.buf);

#ifdef HAVE_LIBCRYPTO
    if (block_state.file.buf[0])
    {
      BIO *file = NULL;
      RSA *pkey = NULL;

      if ((file = BIO_new_file(block_state.file.buf, "r")) == NULL)
      {
        ilog(LOG_TYPE_IRCD, "Ignoring rsa_public_key_file -- file doesn't exist");
        break;
      }

      if ((pkey = PEM_read_bio_RSA_PUBKEY(file, NULL, 0, NULL)) == NULL)
        ilog(LOG_TYPE_IRCD, "Ignoring rsa_public_key_file -- key invalid; check key syntax");
      else
      {
        if (RSA_size(pkey) > 128)
          ilog(LOG_TYPE_IRCD, "Ignoring rsa_public_key_file -- key size must be 1024 or below");
        else
          conf->rsa_public_key = pkey;
      }

      BIO_set_close(file, BIO_CLOSE);
      BIO_free(file);
    }
#endif /* HAVE_LIBCRYPTO */
  }
}
#line 3795 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 1194 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
}
#line 3804 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 1200 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    dlinkAdd(xstrdup(yylval.string), make_dlink_node(), &block_state.mask.list);
}
#line 3813 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 1206 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.rpass.buf, yylval.string, sizeof(block_state.rpass.buf));
}
#line 3822 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 1212 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if (yylval.number)
    block_state.flags.value |= CONF_FLAGS_ENCRYPTED;
  else
    block_state.flags.value &= ~CONF_FLAGS_ENCRYPTED;
}
#line 3836 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 1223 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.file.buf, yylval.string, sizeof(block_state.file.buf));
}
#line 3845 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 1229 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.cert.buf, yylval.string, sizeof(block_state.cert.buf));
}
#line 3854 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 1235 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if (yylval.number)
    block_state.flags.value |= CONF_FLAGS_SSL;
  else
    block_state.flags.value &= ~CONF_FLAGS_SSL;
}
#line 3868 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 1246 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.class.buf, yylval.string, sizeof(block_state.class.buf));
}
#line 3877 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 1252 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value = 0;
}
#line 3886 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 1259 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_BOTS;
}
#line 3895 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 1263 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_CCONN;
}
#line 3904 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 1267 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_DEAF;
}
#line 3913 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 1271 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_DEBUG;
}
#line 3922 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 1275 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_FULL;
}
#line 3931 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 1279 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_HIDDEN;
}
#line 3940 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 1283 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_HIDECHANS;
}
#line 3949 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 1287 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_HIDEIDLE;
}
#line 3958 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 1291 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_SKILL;
}
#line 3967 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 1295 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_NCHANGE;
}
#line 3976 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 1299 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_REJ;
}
#line 3985 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 1303 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_UNAUTH;
}
#line 3994 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 1307 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_SPY;
}
#line 4003 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 1311 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_EXTERNAL;
}
#line 4012 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 1315 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_SERVNOTICE;
}
#line 4021 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 1319 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_INVISIBLE;
}
#line 4030 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 1323 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_WALLOP;
}
#line 4039 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 1327 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_SOFTCALLERID;
}
#line 4048 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 1331 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_CALLERID;
}
#line 4057 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 1335 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_LOCOPS;
}
#line 4066 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 1339 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_REGONLY;
}
#line 4075 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 1343 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.modes.value |= UMODE_FARCONNECT;
}
#line 4084 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 1349 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value = 0;
}
#line 4093 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 1356 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_KILL_REMOTE;
}
#line 4102 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 1360 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_KILL;
}
#line 4111 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 1364 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_CONNECT_REMOTE;
}
#line 4120 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 1368 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_CONNECT;
}
#line 4129 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 1372 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_SQUIT_REMOTE;
}
#line 4138 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 1376 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_SQUIT;
}
#line 4147 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 1380 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_KLINE;
}
#line 4156 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 1384 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_UNKLINE;
}
#line 4165 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 1388 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_DLINE;
}
#line 4174 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1392 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_UNDLINE;
}
#line 4183 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1396 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_XLINE;
}
#line 4192 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1400 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_UNXLINE;
}
#line 4201 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1404 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_GLINE;
}
#line 4210 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1408 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_DIE;
}
#line 4219 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1412 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_RESTART;
}
#line 4228 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1416 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_REHASH;
}
#line 4237 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1420 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_ADMIN;
}
#line 4246 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1424 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_GLOBOPS;
}
#line 4255 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1428 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_WALLOPS;
}
#line 4264 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1432 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_LOCOPS;
}
#line 4273 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1436 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_REMOTEBAN;
}
#line 4282 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1440 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_SET;
}
#line 4291 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1444 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value |= OPER_FLAG_MODULE;
}
#line 4300 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1454 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 1)
    break;

  reset_block_state();

  block_state.ping_freq.value = DEFAULT_PINGFREQUENCY;
  block_state.con_freq.value  = DEFAULT_CONNECTFREQUENCY;
  block_state.max_total.value = MAXIMUM_LINKS_DEFAULT;
  block_state.max_sendq.value = DEFAULT_SENDQ;
  block_state.max_recvq.value = DEFAULT_RECVQ;
}
#line 4317 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1466 "conf_parser.y" /* yacc.c:1646  */
    {
  struct ClassItem *class = NULL;

  if (conf_parser_ctx.pass != 1)
    break;

  if (!block_state.class.buf[0])
    break;

  if (!(class = class_find(block_state.class.buf, 0)))
    class = class_make();

  class->active = 1;
  MyFree(class->name);
  class->name = xstrdup(block_state.class.buf);
  class->ping_freq = block_state.ping_freq.value;
  class->max_perip = block_state.max_perip.value;
  class->con_freq = block_state.con_freq.value;
  class->max_total = block_state.max_total.value;
  class->max_global = block_state.max_global.value;
  class->max_local = block_state.max_local.value;
  class->max_ident = block_state.max_ident.value;
  class->max_sendq = block_state.max_sendq.value;
  class->max_recvq = block_state.max_recvq.value;
  class->max_channels = block_state.max_channels.value;

  if (block_state.min_idle.value > block_state.max_idle.value)
  {
    block_state.min_idle.value = 0;
    block_state.max_idle.value = 0;
    block_state.flags.value &= ~CLASS_FLAGS_FAKE_IDLE;
  }

  class->flags = block_state.flags.value;
  class->min_idle = block_state.min_idle.value;
  class->max_idle = block_state.max_idle.value;

  if (class->number_per_cidr && block_state.number_per_cidr.value)
    if ((class->cidr_bitlen_ipv4 && block_state.cidr_bitlen_ipv4.value) ||
        (class->cidr_bitlen_ipv6 && block_state.cidr_bitlen_ipv6.value))
      if ((class->cidr_bitlen_ipv4 != block_state.cidr_bitlen_ipv4.value) ||
          (class->cidr_bitlen_ipv6 != block_state.cidr_bitlen_ipv6.value))
        rebuild_cidr_list(class);

  class->cidr_bitlen_ipv4 = block_state.cidr_bitlen_ipv4.value;
  class->cidr_bitlen_ipv6 = block_state.cidr_bitlen_ipv6.value;
  class->number_per_cidr = block_state.number_per_cidr.value;
}
#line 4370 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1535 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    strlcpy(block_state.class.buf, yylval.string, sizeof(block_state.class.buf));
}
#line 4379 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1541 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.ping_freq.value = (yyvsp[-1].number);
}
#line 4388 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1547 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.max_perip.value = (yyvsp[-1].number);
}
#line 4397 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1553 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.con_freq.value = (yyvsp[-1].number);
}
#line 4406 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1559 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.max_channels.value = (yyvsp[-1].number);
}
#line 4415 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1565 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.max_total.value = (yyvsp[-1].number);
}
#line 4424 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1571 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.max_global.value = (yyvsp[-1].number);
}
#line 4433 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1577 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.max_local.value = (yyvsp[-1].number);
}
#line 4442 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1583 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.max_ident.value = (yyvsp[-1].number);
}
#line 4451 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1589 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.max_sendq.value = (yyvsp[-1].number);
}
#line 4460 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1595 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    if ((yyvsp[-1].number) >= CLIENT_FLOOD_MIN && (yyvsp[-1].number) <= CLIENT_FLOOD_MAX)
      block_state.max_recvq.value = (yyvsp[-1].number);
}
#line 4470 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1602 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.cidr_bitlen_ipv4.value = (yyvsp[-1].number) > 32 ? 32 : (yyvsp[-1].number);
}
#line 4479 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1608 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.cidr_bitlen_ipv6.value = (yyvsp[-1].number) > 128 ? 128 : (yyvsp[-1].number);
}
#line 4488 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1614 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.number_per_cidr.value = (yyvsp[-1].number);
}
#line 4497 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1620 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 1)
    break;

  block_state.min_idle.value = (yyvsp[-1].number);
  block_state.flags.value |= CLASS_FLAGS_FAKE_IDLE;
}
#line 4509 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1629 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 1)
    break;

  block_state.max_idle.value = (yyvsp[-1].number);
  block_state.flags.value |= CLASS_FLAGS_FAKE_IDLE;
}
#line 4521 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1638 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.flags.value &= CLASS_FLAGS_FAKE_IDLE;
}
#line 4530 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1645 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.flags.value |= CLASS_FLAGS_RANDOM_IDLE;
}
#line 4539 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1649 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 1)
    block_state.flags.value |= CLASS_FLAGS_HIDE_IDLE_FROM_OPERS;
}
#line 4548 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1659 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    reset_block_state();
}
#line 4557 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1665 "conf_parser.y" /* yacc.c:1646  */
    {
  block_state.flags.value = 0;
}
#line 4565 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1671 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= LISTENER_SSL;
}
#line 4574 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1675 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= LISTENER_HIDDEN;
}
#line 4583 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1679 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
   block_state.flags.value |= LISTENER_SERVER;
}
#line 4592 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1687 "conf_parser.y" /* yacc.c:1646  */
    { block_state.flags.value = 0; }
#line 4598 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1692 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
#ifndef HAVE_LIBCRYPTO
    if (block_state.flags.value & LISTENER_SSL)
    {
      conf_error_report("SSL not available - port closed");
      break;
    }
#endif
    add_listener((yyvsp[0].number), block_state.addr.buf, block_state.flags.value);
  }
}
#line 4616 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1705 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
#ifndef HAVE_LIBCRYPTO
    if (block_state.flags.value & LISTENER_SSL)
    {
      conf_error_report("SSL not available - port closed");
      break;
    }
#endif

    for (int i = (yyvsp[-2].number); i <= (yyvsp[0].number); ++i)
      add_listener(i, block_state.addr.buf, block_state.flags.value);
  }
}
#line 4636 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1722 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.addr.buf, yylval.string, sizeof(block_state.addr.buf));
}
#line 4645 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1728 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.addr.buf, yylval.string, sizeof(block_state.addr.buf));
}
#line 4654 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1737 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    reset_block_state();
}
#line 4663 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1741 "conf_parser.y" /* yacc.c:1646  */
    {
  dlink_node *ptr = NULL;

  if (conf_parser_ctx.pass != 2)
    break;

  DLINK_FOREACH(ptr, block_state.mask.list.head)
  {
    struct MaskItem *conf = NULL;
    struct split_nuh_item nuh;

    nuh.nuhmask  = ptr->data;
    nuh.nickptr  = NULL;
    nuh.userptr  = block_state.user.buf;
    nuh.hostptr  = block_state.host.buf;
    nuh.nicksize = 0;
    nuh.usersize = sizeof(block_state.user.buf);
    nuh.hostsize = sizeof(block_state.host.buf);
    split_nuh(&nuh);

    conf        = conf_make(CONF_CLIENT);
    conf->user  = xstrdup(block_state.user.buf);
    conf->host  = xstrdup(block_state.host.buf);

    if (block_state.rpass.buf[0])
      conf->passwd = xstrdup(block_state.rpass.buf);
    if (block_state.name.buf[0])
      conf->name = xstrdup(block_state.name.buf);

    conf->flags = block_state.flags.value;
    conf->port  = block_state.port.value;

    conf_add_class_to_conf(conf, block_state.class.buf);
    add_conf_by_address(CONF_CLIENT, conf);
  }
}
#line 4704 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1790 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    dlinkAdd(xstrdup(yylval.string), make_dlink_node(), &block_state.mask.list);
}
#line 4713 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1796 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.rpass.buf, yylval.string, sizeof(block_state.rpass.buf));
}
#line 4722 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1802 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.class.buf, yylval.string, sizeof(block_state.class.buf));
}
#line 4731 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1808 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    if (yylval.number)
      block_state.flags.value |= CONF_FLAGS_ENCRYPTED;
    else
      block_state.flags.value &= ~CONF_FLAGS_ENCRYPTED;
  }
}
#line 4745 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1819 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value &= (CONF_FLAGS_ENCRYPTED | CONF_FLAGS_SPOOF_IP);
}
#line 4754 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1826 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_SPOOF_NOTICE;
}
#line 4763 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1830 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_NOLIMIT;
}
#line 4772 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1834 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_EXEMPTKLINE;
}
#line 4781 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1838 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_NEED_IDENTD;
}
#line 4790 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1842 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_CAN_FLOOD;
}
#line 4799 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1846 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_NO_TILDE;
}
#line 4808 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1850 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_EXEMPTGLINE;
}
#line 4817 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1854 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_EXEMPTRESV;
}
#line 4826 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1858 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_WEBIRC;
}
#line 4835 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1862 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_NEED_PASSWORD;
}
#line 4844 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1868 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if (valid_hostname(yylval.string))
  {
    strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
    block_state.flags.value |= CONF_FLAGS_SPOOF_IP;
  }
  else
    ilog(LOG_TYPE_IRCD, "Spoof either is too long or contains invalid characters. Ignoring it.");
}
#line 4861 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1882 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
  block_state.flags.value |= CONF_FLAGS_REDIR;
}
#line 4873 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1891 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  block_state.flags.value |= CONF_FLAGS_REDIR;
  block_state.port.value = (yyvsp[-1].number);
}
#line 4885 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1904 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  reset_block_state();
  strlcpy(block_state.rpass.buf, CONF_NOREASON, sizeof(block_state.rpass.buf));
}
#line 4897 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1911 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  create_resv(block_state.name.buf, block_state.rpass.buf, &block_state.mask.list);
}
#line 4908 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1922 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
}
#line 4917 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 340:
#line 1928 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.rpass.buf, yylval.string, sizeof(block_state.rpass.buf));
}
#line 4926 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1934 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    dlinkAdd(xstrdup(yylval.string), make_dlink_node(), &block_state.mask.list);
}
#line 4935 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1949 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if (valid_servname(yylval.string))
  {
    struct MaskItem *conf = conf_make(CONF_SERVICE);
    conf->name = xstrdup(yylval.string);
  }
}
#line 4950 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1964 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  reset_block_state();

  strlcpy(block_state.name.buf, "*", sizeof(block_state.name.buf));
  strlcpy(block_state.user.buf, "*", sizeof(block_state.user.buf));
  strlcpy(block_state.host.buf, "*", sizeof(block_state.host.buf));
  block_state.flags.value = SHARED_ALL;
}
#line 4966 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1975 "conf_parser.y" /* yacc.c:1646  */
    {
  struct MaskItem *conf = NULL;

  if (conf_parser_ctx.pass != 2)
    break;

  conf = conf_make(CONF_ULINE);
  conf->flags = block_state.flags.value;
  conf->name = xstrdup(block_state.name.buf);
  conf->user = xstrdup(block_state.user.buf);
  conf->host = xstrdup(block_state.host.buf);
}
#line 4983 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1992 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
}
#line 4992 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1998 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    struct split_nuh_item nuh;

    nuh.nuhmask  = yylval.string;
    nuh.nickptr  = NULL;
    nuh.userptr  = block_state.user.buf;
    nuh.hostptr  = block_state.host.buf;

    nuh.nicksize = 0;
    nuh.usersize = sizeof(block_state.user.buf);
    nuh.hostsize = sizeof(block_state.host.buf);

    split_nuh(&nuh);
  }
}
#line 5014 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 358:
#line 2017 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value = 0;
}
#line 5023 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 362:
#line 2024 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_KLINE;
}
#line 5032 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 363:
#line 2028 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_UNKLINE;
}
#line 5041 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 364:
#line 2032 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_DLINE;
}
#line 5050 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 365:
#line 2036 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_UNDLINE;
}
#line 5059 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 366:
#line 2040 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_XLINE;
}
#line 5068 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 367:
#line 2044 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_UNXLINE;
}
#line 5077 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 368:
#line 2048 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_RESV;
}
#line 5086 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 369:
#line 2052 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_UNRESV;
}
#line 5095 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 370:
#line 2056 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_LOCOPS;
}
#line 5104 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 371:
#line 2060 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value = SHARED_ALL;
}
#line 5113 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 372:
#line 2069 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  reset_block_state();

  strlcpy(block_state.name.buf, "*", sizeof(block_state.name.buf));
  block_state.flags.value = SHARED_ALL;
}
#line 5127 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 373:
#line 2078 "conf_parser.y" /* yacc.c:1646  */
    {
  struct MaskItem *conf = NULL;

  if (conf_parser_ctx.pass != 2)
    break;

  conf = conf_make(CONF_CLUSTER);
  conf->flags = block_state.flags.value;
  conf->name = xstrdup(block_state.name.buf);
}
#line 5142 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 379:
#line 2093 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
}
#line 5151 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 380:
#line 2099 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value = 0;
}
#line 5160 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 384:
#line 2106 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_KLINE;
}
#line 5169 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 385:
#line 2110 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_UNKLINE;
}
#line 5178 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 386:
#line 2114 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_DLINE;
}
#line 5187 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 387:
#line 2118 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_UNDLINE;
}
#line 5196 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 388:
#line 2122 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_XLINE;
}
#line 5205 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 389:
#line 2126 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_UNXLINE;
}
#line 5214 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 390:
#line 2130 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_RESV;
}
#line 5223 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 391:
#line 2134 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_UNRESV;
}
#line 5232 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 392:
#line 2138 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= SHARED_LOCOPS;
}
#line 5241 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 393:
#line 2142 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value = SHARED_ALL;
}
#line 5250 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 394:
#line 2151 "conf_parser.y" /* yacc.c:1646  */
    {

  if (conf_parser_ctx.pass != 2)
    break;

  reset_block_state();
  block_state.aftype.value = AF_INET;
  block_state.port.value = PORTNUM;
}
#line 5264 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 395:
#line 2160 "conf_parser.y" /* yacc.c:1646  */
    {
  struct MaskItem *conf = NULL;
  struct addrinfo hints, *res;

  if (conf_parser_ctx.pass != 2)
    break;

  if (!block_state.name.buf[0] ||
      !block_state.host.buf[0])
    break;

  if (!block_state.rpass.buf[0] ||
      !block_state.spass.buf[0])
    break;

  if (has_wildcards(block_state.name.buf) ||
      has_wildcards(block_state.host.buf))
    break;

  conf = conf_make(CONF_SERVER);
  conf->port = block_state.port.value;
  conf->flags = block_state.flags.value;
  conf->aftype = block_state.aftype.value;
  conf->host = xstrdup(block_state.host.buf);
  conf->name = xstrdup(block_state.name.buf);
  conf->passwd = xstrdup(block_state.rpass.buf);
  conf->spasswd = xstrdup(block_state.spass.buf);

  if (block_state.cert.buf[0])
    conf->certfp = xstrdup(block_state.cert.buf);

  if (block_state.ciph.buf[0])
    conf->cipher_list = xstrdup(block_state.ciph.buf);

  dlinkMoveList(&block_state.leaf.list, &conf->leaf_list);
  dlinkMoveList(&block_state.hub.list, &conf->hub_list);

  if (block_state.bind.buf[0])
  {
    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE | AI_NUMERICHOST;

    if (getaddrinfo(block_state.bind.buf, NULL, &hints, &res))
      ilog(LOG_TYPE_IRCD, "Invalid netmask for server vhost(%s)", block_state.bind.buf);
    else
    {
      assert(res);

      memcpy(&conf->bind, res->ai_addr, res->ai_addrlen);
      conf->bind.ss.ss_family = res->ai_family;
      conf->bind.ss_len = res->ai_addrlen;
      freeaddrinfo(res);
    }
  }

  conf_add_class_to_conf(conf, block_state.class.buf);
  lookup_confhost(conf);
}
#line 5330 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 413:
#line 2240 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
}
#line 5339 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 414:
#line 2246 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.host.buf, yylval.string, sizeof(block_state.host.buf));
}
#line 5348 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 415:
#line 2252 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.bind.buf, yylval.string, sizeof(block_state.bind.buf));
}
#line 5357 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 416:
#line 2258 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if ((yyvsp[-1].string)[0] == ':')
    conf_error_report("Server passwords cannot begin with a colon");
  else if (strchr((yyvsp[-1].string), ' '))
    conf_error_report("Server passwords cannot contain spaces");
  else
    strlcpy(block_state.spass.buf, yylval.string, sizeof(block_state.spass.buf));
}
#line 5373 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 417:
#line 2271 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass != 2)
    break;

  if ((yyvsp[-1].string)[0] == ':')
    conf_error_report("Server passwords cannot begin with a colon");
  else if (strchr((yyvsp[-1].string), ' '))
    conf_error_report("Server passwords cannot contain spaces");
  else
    strlcpy(block_state.rpass.buf, yylval.string, sizeof(block_state.rpass.buf));
}
#line 5389 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 418:
#line 2284 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.cert.buf, yylval.string, sizeof(block_state.cert.buf));
}
#line 5398 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 419:
#line 2290 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.port.value = (yyvsp[-1].number);
}
#line 5407 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 420:
#line 2296 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.aftype.value = AF_INET;
}
#line 5416 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 421:
#line 2300 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.aftype.value = AF_INET6;
}
#line 5425 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 422:
#line 2306 "conf_parser.y" /* yacc.c:1646  */
    {
  block_state.flags.value &= CONF_FLAGS_ENCRYPTED;
}
#line 5433 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 426:
#line 2312 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_ALLOW_AUTO_CONN;
}
#line 5442 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 427:
#line 2316 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    block_state.flags.value |= CONF_FLAGS_SSL;
}
#line 5451 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 428:
#line 2322 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    if (yylval.number)
      block_state.flags.value |= CONF_FLAGS_ENCRYPTED;
    else
      block_state.flags.value &= ~CONF_FLAGS_ENCRYPTED;
  }
}
#line 5465 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 429:
#line 2333 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    dlinkAdd(xstrdup(yylval.string), make_dlink_node(), &block_state.hub.list);
}
#line 5474 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 430:
#line 2339 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    dlinkAdd(xstrdup(yylval.string), make_dlink_node(), &block_state.leaf.list);
}
#line 5483 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 431:
#line 2345 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.class.buf, yylval.string, sizeof(block_state.class.buf));
}
#line 5492 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 432:
#line 2351 "conf_parser.y" /* yacc.c:1646  */
    {
#ifdef HAVE_LIBCRYPTO
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.ciph.buf, yylval.string, sizeof(block_state.ciph.buf));
#else
  if (conf_parser_ctx.pass == 2)
    conf_error_report("Ignoring connect::ciphers -- no OpenSSL support");
#endif
}
#line 5506 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 433:
#line 2366 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    reset_block_state();
}
#line 5515 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 434:
#line 2370 "conf_parser.y" /* yacc.c:1646  */
    {
  struct MaskItem *conf = NULL;

  if (conf_parser_ctx.pass != 2)
    break;

  if (!block_state.user.buf[0] ||
      !block_state.host.buf[0])
    break;

  conf = conf_make(CONF_KLINE);
  conf->user = xstrdup(block_state.user.buf);
  conf->host = xstrdup(block_state.host.buf);

  if (block_state.rpass.buf[0])
    conf->reason = xstrdup(block_state.rpass.buf);
  else
    conf->reason = xstrdup(CONF_NOREASON);
  add_conf_by_address(CONF_KLINE, conf);
}
#line 5540 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 440:
#line 2395 "conf_parser.y" /* yacc.c:1646  */
    {

  if (conf_parser_ctx.pass == 2)
  {
    struct split_nuh_item nuh;

    nuh.nuhmask  = yylval.string;
    nuh.nickptr  = NULL;
    nuh.userptr  = block_state.user.buf;
    nuh.hostptr  = block_state.host.buf;

    nuh.nicksize = 0;
    nuh.usersize = sizeof(block_state.user.buf);
    nuh.hostsize = sizeof(block_state.host.buf);

    split_nuh(&nuh);
  }
}
#line 5563 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 441:
#line 2415 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.rpass.buf, yylval.string, sizeof(block_state.rpass.buf));
}
#line 5572 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 442:
#line 2424 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    reset_block_state();
}
#line 5581 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 443:
#line 2428 "conf_parser.y" /* yacc.c:1646  */
    {
  struct MaskItem *conf = NULL;

  if (conf_parser_ctx.pass != 2)
    break;

  if (!block_state.addr.buf[0])
    break;

  if (parse_netmask(block_state.addr.buf, NULL, NULL) != HM_HOST)
  {
    conf = conf_make(CONF_DLINE);
    conf->host = xstrdup(block_state.addr.buf);

    if (block_state.rpass.buf[0])
      conf->reason = xstrdup(block_state.rpass.buf);
    else
      conf->reason = xstrdup(CONF_NOREASON);
    add_conf_by_address(CONF_DLINE, conf);
  }
}
#line 5607 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 449:
#line 2454 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.addr.buf, yylval.string, sizeof(block_state.addr.buf));
}
#line 5616 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 450:
#line 2460 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.rpass.buf, yylval.string, sizeof(block_state.rpass.buf));
}
#line 5625 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 456:
#line 2474 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    if (yylval.string[0] && parse_netmask(yylval.string, NULL, NULL) != HM_HOST)
    {
      struct MaskItem *conf = conf_make(CONF_EXEMPT);
      conf->host = xstrdup(yylval.string);

      add_conf_by_address(CONF_EXEMPT, conf);
    }
  }
}
#line 5642 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 457:
#line 2491 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    reset_block_state();
}
#line 5651 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 458:
#line 2495 "conf_parser.y" /* yacc.c:1646  */
    {
  struct MaskItem *conf = NULL;

  if (conf_parser_ctx.pass != 2)
    break;

  if (!block_state.name.buf[0])
    break;

  conf = conf_make(CONF_XLINE);
  conf->name = xstrdup(block_state.name.buf);

  if (block_state.rpass.buf[0])
    conf->reason = xstrdup(block_state.rpass.buf);
  else
    conf->reason = xstrdup(CONF_NOREASON);
}
#line 5673 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 464:
#line 2517 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.name.buf, yylval.string, sizeof(block_state.name.buf));
}
#line 5682 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 465:
#line 2523 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    strlcpy(block_state.rpass.buf, yylval.string, sizeof(block_state.rpass.buf));
}
#line 5691 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 519:
#line 2588 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.away_count = (yyvsp[-1].number);
}
#line 5699 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 520:
#line 2593 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.away_time = (yyvsp[-1].number);
}
#line 5707 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 521:
#line 2598 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.max_watch = (yyvsp[-1].number);
}
#line 5715 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 522:
#line 2603 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigGeneral.cycle_on_host_change = yylval.number;
}
#line 5724 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 523:
#line 2609 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigGeneral.glines = yylval.number;
}
#line 5733 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 524:
#line 2615 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigGeneral.gline_time = (yyvsp[-1].number);
}
#line 5742 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 525:
#line 2621 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigGeneral.gline_request_time = (yyvsp[-1].number);
}
#line 5751 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 526:
#line 2627 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.gline_min_cidr = (yyvsp[-1].number);
}
#line 5759 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 527:
#line 2632 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.gline_min_cidr6 = (yyvsp[-1].number);
}
#line 5767 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 528:
#line 2637 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.tkline_expire_notices = yylval.number;
}
#line 5775 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 529:
#line 2642 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.kill_chase_time_limit = (yyvsp[-1].number);
}
#line 5783 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 530:
#line 2647 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.hide_spoof_ips = yylval.number;
}
#line 5791 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 531:
#line 2652 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.ignore_bogus_ts = yylval.number;
}
#line 5799 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 532:
#line 2657 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.failed_oper_notice = yylval.number;
}
#line 5807 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 533:
#line 2662 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.anti_nick_flood = yylval.number;
}
#line 5815 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 534:
#line 2667 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.max_nick_time = (yyvsp[-1].number);
}
#line 5823 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 535:
#line 2672 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.max_nick_changes = (yyvsp[-1].number);
}
#line 5831 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 536:
#line 2677 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.max_accept = (yyvsp[-1].number);
}
#line 5839 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 537:
#line 2682 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.anti_spam_exit_message_time = (yyvsp[-1].number);
}
#line 5847 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 538:
#line 2687 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.ts_warn_delta = (yyvsp[-1].number);
}
#line 5855 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 539:
#line 2692 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigGeneral.ts_max_delta = (yyvsp[-1].number);
}
#line 5864 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 540:
#line 2698 "conf_parser.y" /* yacc.c:1646  */
    {
  if (((yyvsp[-1].number) > 0) && conf_parser_ctx.pass == 1)
  {
    ilog(LOG_TYPE_IRCD, "You haven't read your config file properly.");
    ilog(LOG_TYPE_IRCD, "There is a line in the example conf that will kill your server if not removed.");
    ilog(LOG_TYPE_IRCD, "Consider actually reading/editing the conf file, and removing this line.");
    exit(0);
  }
}
#line 5878 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 541:
#line 2709 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.invisible_on_connect = yylval.number;
}
#line 5886 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 542:
#line 2714 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.warn_no_connect_block = yylval.number;
}
#line 5894 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 543:
#line 2719 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.stats_e_disabled = yylval.number;
}
#line 5902 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 544:
#line 2724 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.stats_o_oper_only = yylval.number;
}
#line 5910 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 545:
#line 2729 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.stats_P_oper_only = yylval.number;
}
#line 5918 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 546:
#line 2734 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.stats_u_oper_only = yylval.number;
}
#line 5926 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 547:
#line 2739 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.stats_k_oper_only = 2 * yylval.number;
}
#line 5934 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 548:
#line 2742 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.stats_k_oper_only = 1;
}
#line 5942 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 549:
#line 2747 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.stats_i_oper_only = 2 * yylval.number;
}
#line 5950 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 550:
#line 2750 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.stats_i_oper_only = 1;
}
#line 5958 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 551:
#line 2755 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.pace_wait = (yyvsp[-1].number);
}
#line 5966 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 552:
#line 2760 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.caller_id_wait = (yyvsp[-1].number);
}
#line 5974 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 553:
#line 2765 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.opers_bypass_callerid = yylval.number;
}
#line 5982 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 554:
#line 2770 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.pace_wait_simple = (yyvsp[-1].number);
}
#line 5990 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 555:
#line 2775 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.short_motd = yylval.number;
}
#line 5998 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 556:
#line 2780 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.no_oper_flood = yylval.number;
}
#line 6006 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 557:
#line 2785 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.true_no_oper_flood = yylval.number;
}
#line 6014 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 558:
#line 2790 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_pass_resv = yylval.number;
}
#line 6022 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 559:
#line 2795 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.dots_in_ident = (yyvsp[-1].number);
}
#line 6030 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 560:
#line 2800 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.max_targets = (yyvsp[-1].number);
}
#line 6038 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 561:
#line 2805 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.ping_cookie = yylval.number;
}
#line 6046 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 562:
#line 2810 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.disable_auth = yylval.number;
}
#line 6054 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 563:
#line 2815 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.throttle_count = (yyvsp[-1].number);
}
#line 6062 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 564:
#line 2820 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.throttle_time = (yyvsp[-1].number);
}
#line 6070 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 565:
#line 2825 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes = 0;
}
#line 6078 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 569:
#line 2831 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_BOTS;
}
#line 6086 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 570:
#line 2834 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_CCONN;
}
#line 6094 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 571:
#line 2837 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_DEAF;
}
#line 6102 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 572:
#line 2840 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_DEBUG;
}
#line 6110 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 573:
#line 2843 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_FULL;
}
#line 6118 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 574:
#line 2846 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_HIDDEN;
}
#line 6126 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 575:
#line 2849 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_HIDECHANS;
}
#line 6134 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 576:
#line 2852 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_HIDEIDLE;
}
#line 6142 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 577:
#line 2855 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_SKILL;
}
#line 6150 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 578:
#line 2858 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_NCHANGE;
}
#line 6158 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 579:
#line 2861 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_REJ;
}
#line 6166 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 580:
#line 2864 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_UNAUTH;
}
#line 6174 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 581:
#line 2867 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_SPY;
}
#line 6182 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 582:
#line 2870 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_EXTERNAL;
}
#line 6190 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 583:
#line 2873 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_SERVNOTICE;
}
#line 6198 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 584:
#line 2876 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_INVISIBLE;
}
#line 6206 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 585:
#line 2879 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_WALLOP;
}
#line 6214 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 586:
#line 2882 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_SOFTCALLERID;
}
#line 6222 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 587:
#line 2885 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_CALLERID;
}
#line 6230 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 588:
#line 2888 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_LOCOPS;
}
#line 6238 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 589:
#line 2891 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_REGONLY;
}
#line 6246 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 590:
#line 2894 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_umodes |= UMODE_FARCONNECT;
}
#line 6254 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 591:
#line 2899 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes = 0;
}
#line 6262 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 595:
#line 2905 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_BOTS;
}
#line 6270 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 596:
#line 2908 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_CCONN;
}
#line 6278 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 597:
#line 2911 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_DEAF;
}
#line 6286 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 598:
#line 2914 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_DEBUG;
}
#line 6294 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 599:
#line 2917 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_FULL;
}
#line 6302 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 600:
#line 2920 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_SKILL;
}
#line 6310 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 601:
#line 2923 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_HIDDEN;
}
#line 6318 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 602:
#line 2926 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_NCHANGE;
}
#line 6326 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 603:
#line 2929 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_REJ;
}
#line 6334 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 604:
#line 2932 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_UNAUTH;
}
#line 6342 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 605:
#line 2935 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_SPY;
}
#line 6350 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 606:
#line 2938 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_EXTERNAL;
}
#line 6358 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 607:
#line 2941 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_SERVNOTICE;
}
#line 6366 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 608:
#line 2944 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_INVISIBLE;
}
#line 6374 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 609:
#line 2947 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_WALLOP;
}
#line 6382 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 610:
#line 2950 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_SOFTCALLERID;
}
#line 6390 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 611:
#line 2953 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_CALLERID;
}
#line 6398 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 612:
#line 2956 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_LOCOPS;
}
#line 6406 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 613:
#line 2959 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_REGONLY;
}
#line 6414 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 614:
#line 2962 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.oper_only_umodes |= UMODE_FARCONNECT;
}
#line 6422 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 615:
#line 2967 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.min_nonwildcard = (yyvsp[-1].number);
}
#line 6430 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 616:
#line 2972 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.min_nonwildcard_simple = (yyvsp[-1].number);
}
#line 6438 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 617:
#line 2977 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigGeneral.default_floodcount = (yyvsp[-1].number);
}
#line 6446 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 636:
#line 3006 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.disable_fake_channels = yylval.number;
}
#line 6454 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 637:
#line 3011 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.invite_client_count = (yyvsp[-1].number);
}
#line 6462 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 638:
#line 3016 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.invite_client_time = (yyvsp[-1].number);
}
#line 6470 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 639:
#line 3021 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.knock_client_count = (yyvsp[-1].number);
}
#line 6478 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 640:
#line 3026 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.knock_client_time = (yyvsp[-1].number);
}
#line 6486 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 641:
#line 3031 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.knock_delay_channel = (yyvsp[-1].number);
}
#line 6494 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 642:
#line 3036 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.max_channels = (yyvsp[-1].number);
}
#line 6502 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 643:
#line 3041 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.max_bans = (yyvsp[-1].number);
}
#line 6510 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 644:
#line 3046 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.default_split_user_count = (yyvsp[-1].number);
}
#line 6518 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 645:
#line 3051 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.default_split_server_count = (yyvsp[-1].number);
}
#line 6526 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 646:
#line 3056 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.no_create_on_split = yylval.number;
}
#line 6534 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 647:
#line 3061 "conf_parser.y" /* yacc.c:1646  */
    {
  ConfigChannel.no_join_on_split = yylval.number;
}
#line 6542 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 648:
#line 3066 "conf_parser.y" /* yacc.c:1646  */
    {
  GlobalSetOptions.joinfloodcount = yylval.number;
}
#line 6550 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 649:
#line 3071 "conf_parser.y" /* yacc.c:1646  */
    {
  GlobalSetOptions.joinfloodtime = (yyvsp[-1].number);
}
#line 6558 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 662:
#line 3093 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigServerHide.flatten_links = yylval.number;
}
#line 6567 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 663:
#line 3099 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigServerHide.disable_remote_commands = yylval.number;
}
#line 6576 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 664:
#line 3105 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigServerHide.hide_servers = yylval.number;
}
#line 6585 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 665:
#line 3111 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigServerHide.hide_services = yylval.number;
}
#line 6594 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 666:
#line 3117 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    MyFree(ConfigServerHide.hidden_name);
    ConfigServerHide.hidden_name = xstrdup(yylval.string);
  }
}
#line 6606 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 667:
#line 3126 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
  {
    if (((yyvsp[-1].number) > 0) && ConfigServerHide.links_disabled == 1)
    {
      event_write_links_file.when = (yyvsp[-1].number);
      event_addish(&event_write_links_file, NULL);
      ConfigServerHide.links_disabled = 0;
    }

    ConfigServerHide.links_delay = (yyvsp[-1].number);
  }
}
#line 6624 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 668:
#line 3141 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigServerHide.hidden = yylval.number;
}
#line 6633 "conf_parser.c" /* yacc.c:1646  */
    break;

  case 669:
#line 3147 "conf_parser.y" /* yacc.c:1646  */
    {
  if (conf_parser_ctx.pass == 2)
    ConfigServerHide.hide_server_ips = yylval.number;
}
#line 6642 "conf_parser.c" /* yacc.c:1646  */
    break;


#line 6646 "conf_parser.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
