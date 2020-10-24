{
  init_iprocs(subr1s, tc7_subr_1);
  init_iprocs(subr2os, tc7_subr_2o);
  init_iprocs(lsubr2s, tc7_lsubr_2);
  make_subr(s_bytes, tc7_lsubr, scm_bytes);
  make_subr(s_read_byte, tc7_subr_1o, scm_read_byte);
  make_subr(s_bt_ref, tc7_subr_2, scm_byte_ref);
  make_subr(s_bt_set, tc7_subr_3, scm_byte_set);
  add_feature("byte");
  scm_ldstr("\n\
(define bytes-length string-length)\n\
(define bytes-copy string-copy)\n\
(define bytes-append string-append)\n\
(define subbytes substring)\n\
(define bytes->string cr)\n\
(define string->bytes cr)\n\
(define (bytes-reverse bytes)\n\
  (bytes-reverse! (bytes-copy bytes)))\n\
(define (read-bytes n . port)\n\
  (let* ((len (abs n))\n\
	 (byts (make-bytes len))\n\
	 (cnt (if (positive? n)\n\
		  (apply subbytes-read! byts 0 n port)\n\
		  (apply subbytes-read! byts (- n) 0 port))))\n\
    (if (= cnt len)\n\
	byts\n\
	(if (positive? n)\n\
	    (substring byts 0 cnt)\n\
	    (substring byts (- len cnt) len)))))\n\
(define (write-bytes bytes n . port)\n\
  (if (positive? n)\n\
      (apply subbytes-write bytes 0 n port)\n\
      (apply subbytes-write bytes (- n) 0 port)))\n\
(define substring-read! subbytes-read!)\n\
(define substring-write subbytes-write)\n\
");
}
