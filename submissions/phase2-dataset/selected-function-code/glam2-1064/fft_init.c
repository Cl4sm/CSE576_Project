void fft_init(fft_convolver *f, const int max_size) {
  int zero_pad, size, mem;
  assert(max_size > 0);
  zero_pad = max_size - 1;  /* padding to avoid wraparound convolution */
  assert(can_add_int(max_size, zero_pad));
  size = next_pow2(max_size + zero_pad);  /* power-of-2 size is faster??? */
  assert(can_mul_int(2, size / 2 + 1));
  mem = 2 * (size / 2 + 1);  /* padding for in-place transforms */

  f->size = size;

  f->x = fftw_xmalloc(mem * sizeof(double));
  f->y = fftw_xmalloc(mem * sizeof(double));
  f->z = fftw_xmalloc(mem * sizeof(double));

  f->x_plan = fftw_plan_dft_r2c_1d(size, f->x, (fftw_complex *)f->x,
				   FFTW_MEASURE);
  f->y_plan = fftw_plan_dft_r2c_1d(size, f->y, (fftw_complex *)f->y,
				   FFTW_MEASURE);
  f->z_plan = fftw_plan_dft_c2r_1d(size, (fftw_complex *)f->z, f->z,
				   FFTW_MEASURE);
}