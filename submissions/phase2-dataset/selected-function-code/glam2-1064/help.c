static void help(void) {
  die("\
Usage: glam2 [options] alphabet my_seqs.fa\n\
Alphabets: p = proteins, n = nucleotides, other = alphabet file\n\
Options (default settings):\n\
-h: show all options and their default settings\n\
-o: output file (stdout)\n\
-r: number of alignment runs (%d)\n\
-n: end each run after this many iterations without improvement (%d)\n\
-2: examine both strands - forward and reverse complement\n\
-z: minimum number of sequences in the alignment (%d)\n\
-a: minimum number of aligned columns (%d)\n\
-b: maximum number of aligned columns (%d)\n\
-w: initial number of aligned columns (%d)\n\
-d: Dirichlet mixture file\n\
-D: deletion pseudocount (%g)\n\
-E: no-deletion pseudocount (%.1f)\n\
-I: insertion pseudocount (%g)\n\
-J: no-insertion pseudocount (%.1f)\n\
-q: weight for generic versus sequence-set-specific residue abundances (%g)\n\
-t: initial temperature (%g)\n\
-c: cooling factor per n iterations (%g)\n\
-u: temperature lower bound (%g)\n\
-p: print progress information at each iteration\n\
-m: column-sampling moves per site-sampling move (%.1f)\n\
-x: site sampling algorithm: 0=FAST 1=SLOW 2=FFT (%d)\n\
-s: seed for pseudo-random numbers (%u)\n\
", r_def, n_def, z_def, a_def, b_def, w_def, D_def, E_def, I_def, J_def, q_def,
   t_def, c_def, u_def, m_def, x_def, s_def);
}