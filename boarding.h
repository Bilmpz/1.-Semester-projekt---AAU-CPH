#ifndef BOARDING_H
#define BOARDING_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROW 30
#define SEAT 6
#define PASSENGERS (ROW * SEAT)

#define WALK 5
#define SEAT_TIME 20

#define EXTRA_BAGGAGE_TIME 35
#define EXTRA_SLOW_TIME 25

typedef struct {
    int row;
    int seat;
    int has_bag;
    int is_slow;
} passenger;

// Globale variabler (eksternt defineret i boarding.c)
extern int row_seat_occupied[ROW][SEAT];
extern int INTERFERENCE;
extern passenger passengers_list[PASSENGERS];

// Funktioner
void init_passengers_random(void);
int compute_row_interference(int r, int s);
int simulate_queue(int queue_order[], int n);

void make_random_queue(int queue_order[]);
void make_wilma_queue(int queue_order[]);
void make_back_to_front_queue(int queue_order[]);

int random_time(void);
int wilma_time(void);
int back_to_front_time(void);

void run_1000_simulations(int runs);

#endif


