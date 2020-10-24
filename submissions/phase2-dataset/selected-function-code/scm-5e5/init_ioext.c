void init_ioext()
{
	init_iprocs(subr1os, tc7_subr_1o);
	init_iprocs(subr1s, tc7_subr_1);
	init_iprocs(subr2os, tc7_subr_2o);
	init_iprocs(subr2s, tc7_subr_2);
#ifdef O_EXCL
	make_subr(s_try_create_file, tc7_lsubr_2, scm_try_create_file);
#endif
	make_subr(s_reopen_file, tc7_subr_3, reopen_file);
#ifndef THINK_C
# ifndef MCH_AMIGA
#  ifndef __MWERKS__
	make_subr("getpid", tc7_subr_0, l_getpid);
#  endif
	make_subr("getcwd", tc7_subr_0, l_getcwd);
#  ifndef vms
#   ifndef _WIN32
	make_subr(s_utime, tc7_subr_3, l_utime);
#   endif
	tc16_dir = newsmob(&dir_smob);
#  endif
# endif
#endif
#ifndef __IBMC__
# ifndef macintosh
#  ifndef __WATCOMC__
#   ifndef _Windows
	make_subr(s_execv, tc7_subr_2, lexecv);
	make_subr(s_execvp, tc7_subr_2, lexecvp);
	make_subr("execl", tc7_lsubr_2, lexec);
	make_subr("execlp", tc7_lsubr_2, lexecp);
	make_subr(s_putenv, tc7_subr_1, l_putenv);
#   endif
#  endif
# endif
#endif
	add_feature("i/o-extensions");
	add_feature("line-i/o");
	scm_ldstr("\n\
(define (file-exists? path) (access path \"r\"))\n\
(define (make-directory path)\n\
  (define umsk (umask 18))\n\
  (umask umsk)\n\
  (mkdir path (logxor #o777 umsk)))\n\
(define current-directory getcwd)\n\
(define (directory-for-each proc dirname . args)\n\
  (define dir (opendir (if (symbol? dirname)\n\
			   (symbol->string dirname)\n\
			   dirname)))\n\
  (if dir\n\
      (let ((selector\n\
	     (cond ((null? args) identity)\n\
		   ((> (length args) 1)\n\
		    (slib:error 'directory-for-each\n\
				'too-many-arguments\n\
				(cdr args)))\n\
		   ((procedure? (car args)) (car args))\n\
		   ((string? (car args))\n\
		    (require 'filename)\n\
		    (filename:match?? (car args)))\n\
		   (else (slib:error 'directory-for-each\n\
				     'unknown-selector-type\n\
				     (car args))))))\n\
	(do ((filename (readdir dir) (readdir dir)))\n\
	    ((not filename) (closedir dir))\n\
	  (and (selector filename) (proc filename))))))\n\
(define (system->line command . tmp)\n\
  (require 'filename)\n\
  (cond ((null? tmp)\n\
         (call-with-tmpnam\n\
          (lambda (tmp) (system->line command tmp))))\n\
        (else\n\
         (set! tmp (car tmp))\n\
         (and (zero? (system (string-append command \" > \" tmp)))\n\
              (file-exists? tmp)\n\
              (let ((line (call-with-input-file tmp read-line)))\n\
                (if (eof-object? line) \"\" line))))))\n\
");
	add_feature("directory");
}
