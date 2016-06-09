// Kirt Stark Stark_lab_2.cpp - Part 1


#include "stdafx.h"
#include <time.h>
#include <thread>
#include <vector>
#include <algorithm>

// The globals.
int BUFFER_SIZE = 25;
int itemCount;
int LOOPX;
int *buffer;
int added = 0;
int removed = 0;
int proLooped = 0;
int conLooped = 0;
int turn_producer = 0;
int turn_consumer = 0;

// Pointers for producer and consumer index places
int removeIt = 0;
int addIt = 0;

static void putItemIntoBuffer(int item, int thread_num)
{
	if (turn_producer != thread_num)
		return;
	printf("Added to buffer: %d \n", item);
	added++;
	buffer[addIt] = item;
	addIt = (addIt + 1) % BUFFER_SIZE;
}

static int removeItemFromBuffer(int thread_num)
{
	if (turn_consumer != thread_num)
		return -1;

	int item = buffer[removeIt];
	printf("\t\t\tRemoved from buffer: %d \n", item);
	removed++;
	buffer[removeIt] = 0;
	removeIt = (removeIt + 1) % BUFFER_SIZE;
	return (int)item;
}

/**
* Thread job for the producers
* Has NO SYNCHRONIZATION
*/
static void producer(int thread_num){
	while (added < LOOPX)
	{
		if (itemCount < BUFFER_SIZE)
		{
			turn_producer = thread_num;
			putItemIntoBuffer(rand() % 100, thread_num);
			itemCount = itemCount + 1;
			proLooped++;
		}
	}

	return;
}

/**
* Thread job for the consumers
* Has NO SYNCHRONIZATION
*/
static void consumer(int thread_num) {
	while (removed < LOOPX)
	{
		if (itemCount > 0)
		{
			turn_consumer = thread_num;
			removeItemFromBuffer(thread_num);
			itemCount = itemCount - 1;
			conLooped++;
		}
	}

	return;
}

/**
* Main entry point to the program
*/
int main(int argc, char **argv) {

	LOOPX = 100;   // Number of times each thing will run.
	int num_threads = 4;
	itemCount = 0;
	buffer = new int[BUFFER_SIZE];
	std::vector<std::thread> producerTs;
	std::vector<std::thread> consumerTs;

	srand(time(NULL)); // See our random number generator

	// Makin threads
	for (unsigned i = 1; i <= num_threads; ++i)
	{
		// spawn threads
		producerTs.push_back(std::thread(producer, i));
		consumerTs.push_back(std::thread(consumer, i));
	}
	//std::thread producerT(producer);
	//std::thread consumerT(consumer);

	// Waiting for stuff
	for_each(producerTs.begin(), producerTs.end(), std::mem_fn(&(std::thread::join)));
	for_each(consumerTs.begin(), consumerTs.end(), std::mem_fn(&(std::thread::join)));
	//producerT.join();
	//consumerT.join();

	// Everything finished, so blat out the buffer
	int i;
	printf("In buffer:\n");
	for (i = 0; i < BUFFER_SIZE; i++) {
		printf("%d\n", buffer[i]);
	}

	printf("Loops:  Producer = %d, Consumer = %d\n", proLooped, conLooped);
	printf("Added: %d  Removed: %d\n", added, removed);

	system("pause");

	return 1;
}


