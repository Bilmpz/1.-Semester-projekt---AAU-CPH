#include "boarding.h"

int row_seat_occupied[ROW][SEAT] = {0};
int INTERFERENCE;
passenger passengers_list[PASSENGERS];

void init_passengers_random(void) {
    passenger all_seats[PASSENGERS];
    int index = 0;

    for (int r = 0; r < ROW; r++) {
        for (int s = 0; s < SEAT; s++) {
            all_seats[index].row  = r;
            all_seats[index].seat = s;
            index++;
        }
    }
    for (int i = PASSENGERS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        passenger temp = all_seats[i];
        all_seats[i] = all_seats[j];
        all_seats[j] = temp;
    }

    for (int i = 0; i < PASSENGERS; i++) {
        passengers_list[i] = all_seats[i];

        int r1 = rand() % 100;
        int r2 = rand() % 100;
        passengers_list[i].has_bag = (r1 < 50);
        passengers_list[i].is_slow = (r2 < 20);
    }
}

int compute_row_interference(int r, int s) {
    int count = 0;
    switch (s) {
        case 0: 
            if (row_seat_occupied[r][1]) 
                count++; 
            if (row_seat_occupied[r][2]) 
                count++; 
            break;
        case 1: 
            if (row_seat_occupied[r][2]) 
                count++; 
            break;
        case 4: 
            if (row_seat_occupied[r][3]) 
                count++; 
            break;
        case 5: 
            if (row_seat_occupied[r][3]) 
                count++; 
            if (row_seat_occupied[r][4]) 
                count++; 
            break;
        default: break;
    }
    return count;
}

int simulate_queue(int queue_order[], int n) {
    int time = 0;
    int current_aisle_row = -1;
    int remaining = n;

    while (remaining > 0) {
        int leader_id = queue_order[0];
        passenger leader = passengers_list[leader_id];

        int distance = leader.row - current_aisle_row;
        if (distance < 0) distance = 0;

        int base_walk = distance * WALK;
        int walk_interference = (distance > 0) ? (rand() % 3) - 1 : 0;
        if (base_walk + walk_interference < 0)
            walk_interference = -base_walk;

        time += base_walk + walk_interference;

        int new_leader = 0;
        int aisle_count = 0;
        int max_in_aisle = leader.row + 1;

        for (int i = 0; i < remaining; i++) {
            int passenger_id = queue_order[i];
            passenger p = passengers_list[passenger_id];

            if (p.row < leader.row) {
                if (aisle_count < max_in_aisle) {
                    row_seat_occupied[p.row][p.seat] = 1;
                    aisle_count++;
                } else {
                    queue_order[new_leader++] = queue_order[i];
                }
            } else {
                queue_order[new_leader++] = queue_order[i];
            }
        }
        remaining = new_leader;

        int leader_pos = -1;
        for (int i = 0; i < remaining; i++) {
            int passenger_id = queue_order[i];
            passenger p = passengers_list[passenger_id];
            if (p.row == leader.row && p.seat == leader.seat) {
                leader_pos = i;
                break;
            }
        }

        if (leader_pos == -1) {
            current_aisle_row = leader.row;
            continue;
        }

        int people_interference = compute_row_interference(leader.row, leader.seat);
        int base_seat = SEAT_TIME + people_interference * INTERFERENCE;
        if (leader.has_bag) base_seat += EXTRA_BAGGAGE_TIME;
        if (leader.is_slow) base_seat += EXTRA_SLOW_TIME;

        int seat_interference = (rand() % 3) - 1;
        if (base_seat + seat_interference < 0)
            seat_interference = -base_seat;

        time += base_seat + seat_interference;

        row_seat_occupied[leader.row][leader.seat] = 1;

        int shift_leader = 0;
        for (int i = 0; i < remaining; i++) {
            if (i == leader_pos) {
                shift_leader = 1;
                continue;
            }
            queue_order[i - shift_leader] = queue_order[i];
        }

        remaining--;
        current_aisle_row = leader.row;
    }

    return time;
}

void make_random_queue(int queue_order[]) {
    for (int i = 0; i < PASSENGERS; i++)
        queue_order[i] = i;

    for (int i = PASSENGERS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = queue_order[i];
        queue_order[i] = queue_order[j];
        queue_order[j] = temp;
    }
}

void make_wilma_queue(int queue_order[]) {
    int id = 0;

    for (int r = ROW - 1; r >= 0; r--)
        for (int i = 0; i < PASSENGERS; i++)
            if (passengers_list[i].row == r &&
                (passengers_list[i].seat == 0 || passengers_list[i].seat == 5))
                queue_order[id++] = i;

    for (int r = ROW - 1; r >= 0; r--)
        for (int i = 0; i < PASSENGERS; i++)
            if (passengers_list[i].row == r &&
                (passengers_list[i].seat == 1 || passengers_list[i].seat == 4))
                queue_order[id++] = i;

    for (int r = ROW - 1; r >= 0; r--)
        for (int i = 0; i < PASSENGERS; i++)
            if (passengers_list[i].row == r &&
                (passengers_list[i].seat == 2 || passengers_list[i].seat == 3))
                queue_order[id++] = i;
}

void make_back_to_front_queue(int queue_order[]) {
    int id = 0;
    int zone_start[3] = {20, 10, 0};
    int zone_end[3]   = {29, 19, 9};

    for (int z = 0; z < 3; z++) {
        int start = zone_start[z];
        int end   = zone_end[z];

        for (int r = end; r >= start; r--)
            for (int i = 0; i < PASSENGERS; i++)
                if (passengers_list[i].row == r)
                    queue_order[id++] = i;
    }
}

int random_time(void) {
    int queue[PASSENGERS];
    make_random_queue(queue);
    return simulate_queue(queue, PASSENGERS);
}

int wilma_time(void) {
    int queue[PASSENGERS];
    make_wilma_queue(queue);
    return simulate_queue(queue, PASSENGERS);
}

int back_to_front_time(void) {
    int queue[PASSENGERS];
    make_back_to_front_queue(queue);
    return simulate_queue(queue, PASSENGERS);
}

void run_1000_simulations(int runs) {
    long sum_w = 0, sum_r = 0, sum_b = 0;

    for (int k = 0; k < runs; k++) {
        init_passengers_random();

        memset(row_seat_occupied, 0, sizeof(row_seat_occupied));
        sum_w += wilma_time();

        memset(row_seat_occupied, 0, sizeof(row_seat_occupied));
        sum_r += random_time();

        memset(row_seat_occupied, 0, sizeof(row_seat_occupied));
        sum_b += back_to_front_time();
    }

    printf("\nAverage based on %d runs:\n", runs);
    printf("WilMA (avg): %ld sek (%.1f min)\n", sum_w / runs, (sum_w / (double)runs) / 60.0);
    printf("Random (avg): %ld sek (%.1f min)\n", sum_r / runs, (sum_r / (double)runs) / 60.0);
    printf("Back-to-front (avg): %ld sek (%.1f min)\n", sum_b / runs, (sum_b / (double)runs) / 60.0);
}


