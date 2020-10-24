static void secondreport() {
        int i;
        static int hdrcount = 10;

        if (hdrcount >= 10) {
                printf("\tOUT\t\t\tIN\t\t\tOTHER\n");
                printf("bps\tpps\t\tbps\tpps\t\tbps\tpps\n");
                hdrcount = 0;
        }
        hdrcount++;

        for (i = 0; i < MAXCOUNTERDIR; i ++) {
                counter[i][BYTES][SMOOTHED] = smoothed(counter[i][BYTES][SMOOTHED],counter[i][BYTES][INSTANT],ALPHA);
                counter[i][PACKETS][SMOOTHED] = smoothed(counter[i][PACKETS][SMOOTHED],counter[i][PACKETS][INSTANT],ALPHA);
        }
        for (i = 0; i < 2; i++) {
                printf("%d\t%d\t\t",
                                counter[i][BYTES][SMOOTHED]*8, 
                                counter[i][PACKETS][SMOOTHED]);
                counter[i][BYTES][INSTANT] = 0;
                counter[i][PACKETS][INSTANT] = 0;
        }
        printf("%d\t%d\t\t",
                        counter[2][BYTES][INSTANT]*8,
                        counter[2][PACKETS][INSTANT]);
        counter[2][BYTES][INSTANT] = 0;
        counter[2][PACKETS][INSTANT] = 0;
        printf("\n");

        docalc=0;
}