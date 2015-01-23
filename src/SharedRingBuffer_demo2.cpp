#include "SharedRingBuffer.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;


#define BUFFER_SIZE 20
#define NUMBER_OF_WRITING_THREADS 1
#define NUMBER_OF_READING_THREADS 10
#define NUMBER_OF_ELEMENTS_TO_WRITE_PER_WRITER 2000

void writer_fn(ARingBufferWriter<int> *writer, int threadIndex);
void reader_fn(ARingBufferReader<int> *reader, int threadIndex);

//mutex cout_mutex;

int main(int argc, char const *argv[])
{
	static const int bufferSize = BUFFER_SIZE;

	cout << endl << "### Constructing SharedRingBuffer object with size " << BUFFER_SIZE << endl;

	SharedRingBuffer<int> rb(bufferSize);

	SharedRingBufferWriter<int> *writer = rb.getWriter();
	vector<SharedRingBufferReader<int>*> readers;

	vector<thread> writing_threads;
	vector<thread> reading_threads;

	cout << "### Starting " << NUMBER_OF_READING_THREADS << " reading threads" << endl;

	for (int i = 0; i < NUMBER_OF_READING_THREADS; ++i) {
        readers.push_back(new SharedRingBufferReader<int>(&rb));
		reading_threads.push_back(thread(reader_fn, readers[i], i));
	}

	cout << "### Starting " << NUMBER_OF_WRITING_THREADS << " writing threads" << endl;

	for (int i = 0; i < NUMBER_OF_WRITING_THREADS; ++i) {
		writing_threads.push_back(thread(writer_fn, writer, i));
	}

	// Await completion of thread functions
	for (int i = 0; i < NUMBER_OF_READING_THREADS; ++i) {
		reading_threads[i].join();
	}

	for (int i = 0; i < NUMBER_OF_WRITING_THREADS; ++i) {
		writing_threads[i].join();
	}

	cout << "### Done";

	return 0;
}

void writer_fn(ARingBufferWriter<int> *writer, int threadIndex)
{
	for (int i = 1; i <= NUMBER_OF_ELEMENTS_TO_WRITE_PER_WRITER; ++i) {
		writer->enqueue(i);
	}
}

void reader_fn(ARingBufferReader<int> *reader, int threadIndex)
{
	int value;

    int i = 0, progress = 0, mark = 0;

	do {
		value = reader->dequeue();

		progress = (100*i)/NUMBER_OF_ELEMENTS_TO_WRITE_PER_WRITER;
		if (progress >= mark) {
			mark += 10;
			//cout_mutex.lock();
            cout << "Reading thread #" << threadIndex << " @" << mark << "%" << endl;
            //cout_mutex.unlock();
		}
		++i;
	} while (value < NUMBER_OF_ELEMENTS_TO_WRITE_PER_WRITER);
}
