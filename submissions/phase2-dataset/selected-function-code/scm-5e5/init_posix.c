void init_posix()
{
	init_iprocs(subr0s, tc7_subr_0);
	init_iprocs(subr1s, tc7_subr_1);
	init_iprocs(subr1os, tc7_subr_1o);
	init_iprocs(subr2s, tc7_subr_2);
	init_iprocs(subr3s, tc7_subr_3);
	add_feature("posix");
	ptobs[0x0ff & (tc16_pipe>>8)].name = s_pipe;
	ptobs[0x0ff & (tc16_pipe>>8)].fclose = pclose;
	ptobs[0x0ff & (tc16_pipe>>8)].free = pclose;
	add_feature(s_pipe);
	scm_ldstr("\n\
(define (open-input-pipe cmd) (open-pipe cmd \"r\"))\n\
(define (open-output-pipe cmd) (open-pipe cmd \"w\"))\n\
(define (system->line command . tmp)\n\
  (define line\n\
    (call-with-open-ports\n\
     read-line\n\
     (open-input-pipe command)))\n\
  (if (eof-object? line) \"\" line))\n\
");
}
