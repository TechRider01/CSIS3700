#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

using namespace std;

const uint32_t
    QUEUE_SIZE = 16;
  
template <typename QueueType>
class Queue {
public:
    Queue() {
    	capacity = QUEUE_SIZE;
    	data = new QueueType[capacity];
        count = tail = 0;
        head = capacity - 1;
    }
  
    ~Queue() {delete[] data;}
  
    void clear() { count = tail = 0; head = capacity - 1; }
  
    uint32_t size() { return count; }
    bool isEmpty() { return count == 0; }
  
    void enqueue(QueueType d) {

        if (count == capacity) {
        	//Creates a temporary capacity for array size
        	uint32_t 
        		tmpCap = 2 * capacity;
        	//Creates a new array of same type but 2 times the capacity
        	QueueType
        		*tmpData = new QueueType[tmpCap];
        	
        	if(tmpData == nullptr) {
        		throw std::overflow_error("Queue is full");
        	}
        	
        	//Copy data from old array to new array
        	for (int i = 0; i < capacity; i++) {
        		tmpData[i] = data[i];
        	}
        	//Remove old array
        	delete[] data;
        	
        	//Transfer old data to new array and update capacity
        	data = tmpData;
        	capacity = tmpCap;
        	
        	tail = capacity / 2;
        	head = capacity - 1;
        	
        }
      
        data[tail++] = d;
        
        tail = tail % capacity;

        count++;
    }
  
    QueueType dequeue() {
  
        if (count == 0)
            throw underflow_error("Queue is empty");

        head = ++head % capacity;

        count--;

        return data[head];
    }

private:
    QueueType
        *data;
    	
    uint32_t
        head,tail,
        count,
        capacity;
};

#endif
