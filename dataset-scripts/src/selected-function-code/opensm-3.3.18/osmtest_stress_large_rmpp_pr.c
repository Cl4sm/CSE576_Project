static ib_api_status_t osmtest_stress_large_rmpp_pr(IN osmtest_t * const p_osmt)
{
	ib_api_status_t status = IB_SUCCESS;
	uint64_t num_recs = 0;
	uint64_t num_queries = 0;
	uint32_t delta_recs;
	uint32_t delta_queries;
	uint32_t print_freq = 0;
	struct timeval start_tv, end_tv;
	long sec_diff, usec_diff;
	float ratio;

	OSM_LOG_ENTER(&p_osmt->log);
	gettimeofday(&start_tv, NULL);
	printf("-I- Start time is : %09ld:%06ld [sec:usec]\n", start_tv.tv_sec,
	       (long)start_tv.tv_usec);

	while (num_queries < STRESS_LARGE_PR_RMPP_THR) {
		delta_recs = 0;
		delta_queries = 0;

		status = osmtest_stress_path_recs_by_guid(p_osmt, &delta_recs,
							  &delta_queries);
		if (status != IB_SUCCESS)
			goto Exit;

		num_recs += delta_recs;
		num_queries += delta_queries;

		print_freq += delta_recs;
		if (print_freq > 10000) {
			gettimeofday(&end_tv, NULL);
			if (end_tv.tv_usec > start_tv.tv_usec) {
				sec_diff = end_tv.tv_sec - start_tv.tv_sec;
				usec_diff = end_tv.tv_usec - start_tv.tv_usec;
			} else {
				sec_diff = end_tv.tv_sec - start_tv.tv_sec - 1;
				usec_diff =
				    1000000 - (start_tv.tv_usec -
					       end_tv.tv_usec);
			}
			printf("-I- End time is : %09ld:%06ld [sec:usec]\n",
			       end_tv.tv_sec, (long)end_tv.tv_usec);
			printf("-I- Querying %" PRId64
			       " Path Record queries CA to CA (rmpp)\n\ttook %04ld:%06ld [sec:usec]\n",
			       num_queries, sec_diff, usec_diff);
			if (num_recs == 0)
				ratio = 0;
			else
				ratio = ((float)num_queries / (float)num_recs);
			printf("-I- Queries to Record Ratio is %" PRIu64
			       " records, %" PRIu64 " queries : %.2f \n",
			       num_recs, num_queries, ratio);
			print_freq = 0;
		}
	}

Exit:
	gettimeofday(&end_tv, NULL);
	printf("-I- End time is : %09ld:%06ld [sec:usec]\n",
	       end_tv.tv_sec, (long)end_tv.tv_usec);
	if (end_tv.tv_usec > start_tv.tv_usec) {
		sec_diff = end_tv.tv_sec - start_tv.tv_sec;
		usec_diff = end_tv.tv_usec - start_tv.tv_usec;
	} else {
		sec_diff = end_tv.tv_sec - start_tv.tv_sec - 1;
		usec_diff = 1000000 - (start_tv.tv_usec - end_tv.tv_usec);
	}

	printf("-I- Querying %" PRId64
	       " Path Record queries (rmpp) took %04ld:%06ld [sec:usec]\n",
	       num_queries, sec_diff, usec_diff);

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}