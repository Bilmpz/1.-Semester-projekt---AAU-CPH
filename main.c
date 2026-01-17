#include "boarding.h"

int main(void) {
    srand((unsigned int)time(NULL));
    INTERFERENCE = 15 + rand() % 16;

    init_passengers_random();

    int first_w = wilma_time();
    int first_r = random_time();
    int first_b = back_to_front_time();

    printf("First simulation:\n");
    printf("WilMA: %d sek (%.1f min)\n", first_w, first_w / 60.0);
    printf("Random: %d sek (%.1f min)\n", first_r, first_r / 60.0);
    printf("Back-to-front: %d sek (%.1f min)\n\n", first_b, first_b / 60.0);

    run_1000_simulations(1000);
    return 0;
}

