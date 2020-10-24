int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 88 "grammar.y"
{ loaded_packages = new_list();
           loaded_groups = new_list();
           loaded_annotations = new_list(); ;
    break;}
case 3:
#line 94 "grammar.y"
{ add_to_tail(loaded_packages, (void*) yyvsp[0].package); ;
    break;}
case 4:
#line 96 "grammar.y"
{ add_to_tail(loaded_groups, (void*) yyvsp[0].group); ;
    break;}
case 5:
#line 98 "grammar.y"
{ add_to_tail(loaded_annotations, (void*) yyvsp[0].annotation); ;
    break;}
case 6:
#line 102 "grammar.y"
{ yyval.package = yyvsp[-1].package;
           yyval.package->name = yyvsp[-4].list; yyval.package->arch = NULL; yyval.package->os = NULL; yyval.package->version = NULL;
           yyval.package->host = NULL; yyval.package->shell = NULL; yyval.package->requires = yyvsp[-3].list; ;
    break;}
case 7:
#line 107 "grammar.y"
{ yyval.package = yyvsp[-1].package;
           yyval.package->name = yyvsp[-5].list; yyval.package->arch = yyvsp[-4].list; yyval.package->os = NULL; yyval.package->version = NULL;
           yyval.package->host = NULL; yyval.package->shell = NULL; yyval.package->requires = yyvsp[-3].list; ;
    break;}
case 8:
#line 112 "grammar.y"
{ yyval.package = yyvsp[-1].package;
           yyval.package->name = yyvsp[-6].list; yyval.package->arch = yyvsp[-5].list; yyval.package->os = yyvsp[-4].list; yyval.package->version = NULL;
           yyval.package->host = NULL; yyval.package->shell = NULL; yyval.package->requires = yyvsp[-3].list; ;
    break;}
case 9:
#line 117 "grammar.y"
{ yyval.package = yyvsp[-1].package;
           yyval.package->name = yyvsp[-7].list; yyval.package->arch = yyvsp[-6].list; yyval.package->os = yyvsp[-5].list; yyval.package->version = yyvsp[-4].list;
           yyval.package->host = NULL; yyval.package->shell = NULL; yyval.package->requires = yyvsp[-3].list; ;
    break;}
case 10:
#line 122 "grammar.y"
{ yyval.package = yyvsp[-1].package;
           yyval.package->name = yyvsp[-8].list; yyval.package->arch = yyvsp[-7].list; yyval.package->os = yyvsp[-6].list; yyval.package->version = yyvsp[-5].list;
           yyval.package->host = yyvsp[-4].list; yyval.package->shell = NULL; yyval.package->requires = yyvsp[-3].list; ;
    break;}
case 11:
#line 127 "grammar.y"
{ yyval.package = yyvsp[-1].package;
           yyval.package->name = yyvsp[-9].list; yyval.package->arch = yyvsp[-8].list; yyval.package->os = yyvsp[-7].list; yyval.package->version = yyvsp[-6].list;
           yyval.package->host = yyvsp[-5].list; yyval.package->shell = yyvsp[-4].list; yyval.package->requires = yyvsp[-3].list; ;
    break;}
case 12:
#line 132 "grammar.y"
{ yyval.list = NULL; ;
    break;}
case 13:
#line 134 "grammar.y"
{ yyval.list = yyvsp[0].list; ;
    break;}
case 14:
#line 137 "grammar.y"
{ yyval.group = new(group_t); yyval.group->name = yyvsp[-3].string; yyval.group->packages = yyvsp[-1].list;;
    break;}
case 15:
#line 141 "grammar.y"
{ yyval.annotation = new(annotation_t);
              yyval.annotation->name = yyvsp[-3].string;
              yyval.annotation->description = yyvsp[-1].string; ;
    break;}
case 16:
#line 146 "grammar.y"
{ yyval.package = new(package_t);
            yyval.package->variables = new_list();
            yyval.package->scripts = new_list();
            add_to_tail(yyval.package->variables, (void*) yyvsp[0].variable); ;
    break;}
case 17:
#line 151 "grammar.y"
{ yyval.package = new(package_t);
            yyval.package->variables = new_list();
            yyval.package->scripts = new_list();
            add_to_tail(yyval.package->scripts, (void*) yyvsp[0].script); ;
    break;}
case 18:
#line 156 "grammar.y"
{ yyval.package = yyvsp[-2].package;
            add_to_tail(yyval.package->variables, (void*) yyvsp[0].variable); ;
    break;}
case 19:
#line 159 "grammar.y"
{ yyval.package = yyvsp[-2].package;
            add_to_tail(yyval.package->scripts, (void*) yyvsp[0].script); ;
    break;}
case 20:
#line 163 "grammar.y"
{ yyval.variable = new(variable_t);
            yyval.variable->name = yyvsp[-2].string; yyval.variable->type = VAR_LIT_SET;
            yyval.variable->literal = yyvsp[0].string; yyval.variable->pathlist = NULL; ;
    break;}
case 21:
#line 167 "grammar.y"
{ yyval.variable = new(variable_t);
            yyval.variable->name = yyvsp[-2].string; yyval.variable->type = VAR_PATH_SET;
            yyval.variable->literal = NULL; yyval.variable->pathlist = yyvsp[0].list; ;
    break;}
case 22:
#line 171 "grammar.y"
{ yyval.variable = new(variable_t); 
            yyval.variable->name = yyvsp[-2].string; yyval.variable->type = VAR_PATH_ADD;
            yyval.variable->literal = NULL; yyval.variable->pathlist = make_pathlist(yyvsp[0].string); ;
    break;}
case 23:
#line 175 "grammar.y"
{ yyval.variable = new(variable_t); 
            yyval.variable->name = yyvsp[-2].string; yyval.variable->type = VAR_PATH_ADD;
            yyval.variable->literal = NULL; yyval.variable->pathlist = yyvsp[0].list; ;
    break;}
case 24:
#line 179 "grammar.y"
{ yyval.variable = new(variable_t); 
            yyval.variable->name = yyvsp[-2].string; yyval.variable->type = VAR_PATH_TESTSET;
            yyval.variable->literal = NULL; yyval.variable->pathlist = yyvsp[0].list; ;
    break;}
case 25:
#line 183 "grammar.y"
{ yyval.variable = new(variable_t); 
            yyval.variable->name = yyvsp[-2].string; yyval.variable->type = VAR_PATH_TESTADD;
            yyval.variable->literal = NULL; yyval.variable->pathlist = make_pathlist(yyvsp[0].string); ;
    break;}
case 26:
#line 187 "grammar.y"
{ yyval.variable = new(variable_t); 
            yyval.variable->name = yyvsp[-2].string; yyval.variable->type = VAR_PATH_TESTADD;
            yyval.variable->literal = NULL; yyval.variable->pathlist = yyvsp[0].list; ;
    break;}
case 27:
#line 192 "grammar.y"
{ yyval.script = new(script_t);
	  yyval.script->text = strdup(litbuf); ;
    break;}
case 28:
#line 196 "grammar.y"
{ yyval.list = new_list(); add_to_tail(yyval.list, (void*) yyvsp[0].match); ;
    break;}
case 29:
#line 198 "grammar.y"
{ yyval.list = yyvsp[-1].list; ;
    break;}
case 30:
#line 201 "grammar.y"
{ yyval.list = new_list(); add_to_tail(yyval.list, (void*) yyvsp[0].match); ;
    break;}
case 31:
#line 203 "grammar.y"
{ add_to_tail(yyval.list = yyvsp[-2].list, (void*) yyvsp[0].match); ;
    break;}
case 32:
#line 206 "grammar.y"
{ yyval.match = new(match_t); yyval.match->type = MATCH_EXACT; yyval.match->text = yyvsp[0].string; ;
    break;}
case 33:
#line 208 "grammar.y"
{ yyval.match = new(match_t); yyval.match->type = MATCH_PREFIX; yyval.match->text = yyvsp[0].string; ;
    break;}
case 34:
#line 210 "grammar.y"
{ yyval.match = new(match_t); yyval.match->type = MATCH_WILD; yyval.match->text = NULL; ;
    break;}
case 35:
#line 213 "grammar.y"
{ yyval.list = new_list(); add_to_tail(yyval.list, (void*) yyvsp[0].string); ;
    break;}
case 36:
#line 215 "grammar.y"
{ add_to_tail(yyval.list = yyvsp[-2].list, (void*) yyvsp[0].string); ;
    break;}
case 37:
#line 218 "grammar.y"
{ yyval.list = new_list(); add_to_tail(yyval.list, (void*) yyvsp[0].string); ;
    break;}
case 38:
#line 220 "grammar.y"
{ add_to_tail(yyval.list = yyvsp[-2].list, (void*) yyvsp[0].string); ;
    break;}
case 39:
#line 223 "grammar.y"
{ yyval.string = strdup(expand(litbuf)); ;
    break;}
case 40:
#line 226 "grammar.y"
{ yyval.string = strdup(litbuf); ;
    break;}
case 41:
#line 229 "grammar.y"
{ yyval.string = strdup(litbuf); ;
    break;}
case 42:
#line 232 "grammar.y"
{ yyval.string = strdup(litbuf); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
