#include "RingBuffer.h"
#include <iostream>

using namespace std;


#define BUFFER_SIZE 20


int main(int argc, char const *argv[])
{
	cout << endl << "### Constructing RingBuffer object with size " << BUFFER_SIZE << endl;

	RingBuffer<int> rb(BUFFER_SIZE);

	RingBufferWriter<int> &writer = *rb.getWriter();
	RingBufferReader<int> reader(&rb);

	cout << "### Enqueuing four integers" << endl;

	writer.enqueue(1);
	writer.enqueue(2);
	writer.enqueue(3);
	writer.enqueue(4);

	cout << endl << "### Dequeuing two integers" << endl;

	cout << reader.dequeue() << endl
		 << reader.dequeue() << endl;

	cout << endl << "### Enqueuing " << BUFFER_SIZE << " integers" << endl;

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        try {
            writer.enqueue(36);
        } catch (std::runtime_error &exception) {
            cout << exception.what() << endl;
        }
    }

	cout << endl << "### Reading everything from RingBuffer into an array... ";

	int *str_array = new int[BUFFER_SIZE];
	reader.read(str_array, reader.available());

	cout << "OK" << endl
		 << "Number of elements now in the RingBuffer: " << reader.available() << endl;

	delete[] str_array;

	return 0;
}
