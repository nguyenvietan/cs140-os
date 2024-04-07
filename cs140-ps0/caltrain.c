#include "pintos_thread.h"

struct station {
	struct condition *cond_train_arrived;
	struct condition *cond_all_passengers_seated;
	struct lock *lck;
	int available_seats;
	int waiting_passengers;
};

void
station_init(struct station *station)
{
	station->cond_train_arrived = malloc(sizeof(struct condition));
	station->cond_all_passengers_seated = malloc(sizeof(struct condition));
	station->lck = malloc(sizeof(struct lock));
	cond_init(station->cond_train_arrived);
	cond_init(station->cond_all_passengers_seated);
	lock_init(station->lck);
	station->available_seats = 0;
	station->waiting_passengers = 0;
}

void
station_load_train(struct station *station, int count)
{
	lock_acquire(station->lck);
	station->available_seats = count;

	while (station->available_seats > 0 && station->waiting_passengers > 0) {
		cond_broadcast(station->cond_train_arrived, station->lck);
		cond_wait(station->cond_all_passengers_seated, station->lck);
	}
	printf("station->available_seats = %d, station->waiting_passengers = %d\n",
		station->available_seats, station->waiting_passengers);

	station->available_seats = 0;

	lock_release(station->lck);
}

void
station_wait_for_train(struct station *station)
{
	lock_acquire(station->lck);
	station->waiting_passengers++;

	while (station->available_seats == 0) {
		cond_wait(station->cond_train_arrived, station->lck);
	}

	station->waiting_passengers--;
	station->available_seats--;

	lock_release(station->lck);
}

void
station_on_board(struct station *station)
{
	lock_acquire(station->lck);

	if (station->available_seats == 0 || station->waiting_passengers == 0) {
		cond_signal(station->cond_all_passengers_seated, station->lck);
	}

	lock_release(station->lck);
}
