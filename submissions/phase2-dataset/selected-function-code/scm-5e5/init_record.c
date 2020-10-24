{
  SCM i_name = CAR(sysintern("name", UNDEFINED));
  SCM i_fields = CAR(sysintern("fields", UNDEFINED));
  SCM i_printer = CAR(sysintern("printer", UNDEFINED));
  SCM the_rtd, rtd_name = makfrom0str("record-type");
  SCM rtd_fields = cons2(i_name, i_fields, cons(i_printer, EOL));
  tc16_record = newsmob(&recsmob);
  f_rtdprin1 = make_subr(" rtdprin1", tc7_subr_3, rec_rtdprin1);
  DEFER_INTS;
  the_rtd = must_malloc_cell(4L * sizeof(SCM),
		 MAKE_NUMDIGS(4L, tc16_record), s_record);
  REC_RTD(the_rtd) = the_rtd;
  RTD_NAME(the_rtd) = rtd_name;
  RTD_FIELDS(the_rtd) = rtd_fields;
  RTD_PRINTER(the_rtd) = f_rtdprin1;
  ALLOW_INTS;
  the_rtd_rtd = the_rtd;	/* Protected by make-record-type */
  f_rec_pred1 = make_subr(" record-predicate-procedure", tc7_subr_2, rec_pred1);
  f_rec_constr1 = make_subr(s_rec_constr1, tc7_lsubr, rec_constr1);
  f_rec_accessor1 = make_subr(s_rec_accessor1, tc7_subr_2, rec_accessor1);
  f_rec_modifier1 = make_subr(s_rec_modifier1, tc7_subr_3, rec_modifier1);
  make_subr(s_rec_constr, tc7_subr_2o, rec_constr);
  init_iprocs(subr1s, tc7_subr_1);
  init_iprocs(subr2s, tc7_subr_2);
  sysintern("record-type-descriptor?", rec_pred(the_rtd_rtd));
  sysintern("record-type-name", rec_accessor(the_rtd_rtd, i_name));
  sysintern("record-type-field-names", rec_accessor(the_rtd_rtd, i_fields));
  loc_makrtd = &CDR(sysintern("RTD:make",
			      rec_constr(the_rtd_rtd, cons2(i_name, i_fields, EOL))));
  add_feature(s_record);
}
