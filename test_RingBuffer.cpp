#include "RingBuffer.h"

#include <iostream>


int main(int argc, char const *argv[])
{
	using namespace std;
	static int bufferSize = 20;

	RingBuffer<int> rb(bufferSize);

	RingBufferWriter<int> &writer = *rb.getWriter();
	RingBufferReader<int> reader(&rb);

	cout << "### Enqueuing three strings" << endl;
	writer.enqueue(1);
	writer.enqueue(2);
	writer.enqueue(3);

	cout << endl << "### Dequeuing two strings" << endl;
	cout << reader.dequeue() << endl
		 << reader.dequeue() << endl;

	cout << endl << "### Enqueuing [bufferSize] strings" << endl;
	try {
		for (int i = 0; i < bufferSize; ++i) {
			writer.enqueue(36);
		}
	} catch (std::runtime_error &exception) {
		cout << exception.what() << endl;
	}

	cout << endl << "### Reading from RingBuffer into an array... ";
	int *str_array = new int[bufferSize];
	reader.read(str_array, reader.available());
	cout << "OK" << endl;
	cout << "Number of elements now in the RingBuffer: " << reader.available() << endl;

	return 0;
}
