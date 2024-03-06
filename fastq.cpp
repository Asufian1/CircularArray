// CMSC 341 - Spring 2024 - Project 1
/*****************************************
** File:    fastq.cpp
** Project: CMSC 341 Project 1
** Author:  Abdullah sufian
** Date:    3/5/24
** E-mail:  asufian1@umbc.edu 
** Description: This code defines two classes, ArrayBuffer and ListBuffer, 
which are used for buffering integers in a dynamic array and a circular 
linked list structure, respectively.
** 
**
**
***********************************************/

#include "fastq.h"


// Name: ArrayBuffer(int capacity) - Constructor
// Desc: Initializes the ArrayBuffer with a given capacity. Allocates memory for the buffer.
// Preconditions: A non-negative integer for capacity.
// Postconditions: An ArrayBuffer object is created with the specified capacity.
ArrayBuffer::ArrayBuffer(int capacity) {
    if (capacity < 1) {
        m_capacity = 0;
        m_buffer = nullptr;
    } else {
        m_capacity = capacity;
        m_buffer = new int[m_capacity];
    }
    m_count = 0;
    m_start = 0;
    m_end = 0;
    m_next = nullptr;
}

// Name: ~ArrayBuffer() - Destructor
// Desc: Deallocates the memory used by the ArrayBuffer.
// Preconditions: An existing ArrayBuffer object.
// Postconditions: The memory allocated for the buffer is freed.
ArrayBuffer::~ArrayBuffer() {
    delete[] m_buffer;
    m_buffer = nullptr;
}

// Name: clear() - Clear Method
// Desc: Clears the contents of the ArrayBuffer and resets its attributes.
// Preconditions: An existing ArrayBuffer object.
// Postconditions: The ArrayBuffer is emptied, and its attributes are reset.
void ArrayBuffer::clear() {
    delete[] m_buffer;
    m_buffer = nullptr;
    m_capacity = 0;
    m_count = 0;
    m_start = 0;
    m_end = 0;
}

// Name: ArrayBuffer(const ArrayBuffer &rhs) - Copy Constructor
// Desc: Creates a new ArrayBuffer as a deep copy of another ArrayBuffer.
// Preconditions: An existing ArrayBuffer object to copy from.
// Postconditions: A deep copy of the ArrayBuffer is created.
ArrayBuffer::ArrayBuffer(const ArrayBuffer &rhs) {
    m_capacity = rhs.m_capacity;
    m_count = rhs.m_count;
    m_start = rhs.m_start;
    m_end = rhs.m_end;
    m_buffer = new int[m_capacity];

    // Manually copying the elements
    for (int i = 0; i < m_capacity; ++i) {
        m_buffer[i] = rhs.m_buffer[i];
    }

    m_next = nullptr; // m_next is managed by ListBuffer
}


// Name: operator=(const ArrayBuffer &rhs) - Overloaded Assignment Operator
// Desc: Assigns one ArrayBuffer to another, using deep copy.
// Preconditions: An existing ArrayBuffer object to assign from.
// Postconditions: The current ArrayBuffer becomes a deep copy of the provided ArrayBuffer.
const ArrayBuffer & ArrayBuffer::operator=(const ArrayBuffer &rhs) {
    if (this != &rhs) {
        clear();

        m_capacity = rhs.m_capacity;
        m_count = rhs.m_count;
        m_start = rhs.m_start;
        m_end = rhs.m_end;
        m_buffer = new int[m_capacity];

        // Manually copying the elements
        for (int i = 0; i < m_capacity; ++i) {
            m_buffer[i] = rhs.m_buffer[i];
        }
    }
    return *this;
}


// Name: enqueue(int data) - Enqueue Method
// Desc: Adds an integer to the buffer. Throws overflow_error if the buffer is full.
// Preconditions: An integer to add to the buffer.
// Postconditions: The integer is added to the buffer, or an overflow_error is thrown.
void ArrayBuffer::enqueue(int data) {
    if (m_count >= m_capacity) {
        throw overflow_error("Buffer overflow");
    }
    m_buffer[m_end] = data;
    m_end = (m_end + 1) % m_capacity;
    m_count++;
}

// Name: dequeue() - Dequeue Method
// Desc: Removes and returns the first integer from the buffer. Throws underflow_error if the buffer is empty.
// Preconditions: A non-empty ArrayBuffer.
// Postconditions: The first integer is removed and returned, or an underflow_error is thrown.
int ArrayBuffer::dequeue() {
    if (m_count <= 0) {
        throw std::underflow_error("Buffer underflow");
    }
    int data = m_buffer[m_start];
    m_start = (m_start + 1) % m_capacity;
    m_count--;
    return data;
}

// Name: empty() const - Empty Check Method
// Desc: Checks if the ArrayBuffer is empty.
// Preconditions: An existing ArrayBuffer object.
// Postconditions: Returns true if the buffer is empty; false otherwise.
bool ArrayBuffer::empty() const {
    return m_count == 0;
}



void ArrayBuffer::dump(){
    int start = m_start;
    //int end = m_end;
    int counter = 0;
    cout << "ArrayBuffer size: " << m_capacity << " : ";
    if (!empty()){
        while(counter < m_count){
            cout << m_buffer[start] << "[" << start << "]" << " ";
            start = (start + 1) % m_capacity;
            counter++;
        }
        cout << endl;
    }
    else cout << "ArrayBuffer is empty!" << endl;
}


//**** ListBuffer ****

// Name: ListBuffer(int minBufCapacity) - Constructor
// Desc: Initializes the ListBuffer with a given minimum buffer capacity.
// Preconditions: A non-negative integer for minimum buffer capacity.
// Postconditions: A ListBuffer object is created with the specified minimum capacity.
ListBuffer::ListBuffer(int minBufCapacity) {
    if (minBufCapacity < 1) {
        minBufCapacity = DEFAULT_MIN_CAPACITY;
    }
    m_minBufCapacity = minBufCapacity;
    m_cursor = new ArrayBuffer(m_minBufCapacity);
    m_cursor->m_next = m_cursor; // circular linked list
    m_listSize = 1;
}


// Name: ~ListBuffer() - Destructor
// Desc: Deallocates all the ArrayBuffers within the ListBuffer.
// Preconditions: An existing ListBuffer object.
// Postconditions: All memory used by the ArrayBuffers within the ListBuffer is freed.

ListBuffer::~ListBuffer() {
    clear();
}


// Name: clear() - Clear Method
// Desc: Clears the contents of the ListBuffer and resets its attributes.
// Preconditions: An existing ListBuffer object.
// Postconditions: The ListBuffer is emptied, and its attributes are reset.

void ListBuffer::clear() {
    // Break the circular link if m_cursor is not nullptr
    if (m_cursor != nullptr) {
        ArrayBuffer *first = m_cursor->m_next;
        m_cursor->m_next = nullptr;  // Break the circular link
        m_cursor = nullptr;  // Avoid dangling pointer

        ArrayBuffer *current = first;
        while (current != nullptr) {
            ArrayBuffer *temp = current;
            current = current->m_next;
            temp->clear();
            delete temp;
        }
    }
    m_listSize = 0;  // Reset the list size
}



// Name: enqueue(const int &data) - Enqueue Method
// Desc: Adds an integer to the ListBuffer. Expands the buffer if necessary.
// Preconditions: An integer to add to the ListBuffer.
// Postconditions: The integer is added to the ListBuffer, expanding the buffer if needed.
void ListBuffer::enqueue(const int &data) {
    try {
        m_cursor->enqueue(data);
    } catch (const overflow_error &) {
        int newCapacity = min(m_cursor->m_capacity * INCREASE_FACTOR, m_minBufCapacity * MAX_FACTOR);
        ArrayBuffer *newBuffer = new ArrayBuffer(newCapacity);
        newBuffer->enqueue(data);
        newBuffer->m_next = m_cursor->m_next;
        m_cursor->m_next = newBuffer;
        m_cursor = newBuffer;
        m_listSize++;
    }
}

// Name: dequeue() - Dequeue Method
// Desc: Removes and returns the first integer from the ListBuffer. Throws underflow_error if the ListBuffer is empty.
// Preconditions: A non-empty ListBuffer.
// Postconditions: The first integer is removed and returned, or an underflow_error is thrown.
int ListBuffer::dequeue() {
    if (m_cursor->m_next->empty()) {
        if (m_listSize == 1) {
            throw std::underflow_error("Queue underflow");
        }
        ArrayBuffer *temp = m_cursor->m_next;
        m_cursor->m_next = temp->m_next;
        temp->clear();
        delete temp;
        m_listSize--;
    }
    return m_cursor->m_next->dequeue();
}


// Name: ListBuffer(const ListBuffer &rhs) - Copy Constructor
// Desc: Creates a new ListBuffer as a deep copy of another ListBuffer.
// Preconditions: An existing ListBuffer object to copy from.
// Postconditions: A deep copy of the ListBuffer is created.
ListBuffer::ListBuffer(const ListBuffer &rhs) {
    if (rhs.m_cursor == nullptr) {
        m_cursor = nullptr;
        m_listSize = 0;
        m_minBufCapacity = rhs.m_minBufCapacity;
    } else {
        m_cursor = new ArrayBuffer(*rhs.m_cursor); // Copy first ArrayBuffer
        ArrayBuffer *newCursor = m_cursor;

        ArrayBuffer *currentRHS = rhs.m_cursor->m_next;
        while (currentRHS != rhs.m_cursor) {
            newCursor->m_next = new ArrayBuffer(*currentRHS);
            newCursor = newCursor->m_next;
            currentRHS = currentRHS->m_next;
        }
        newCursor->m_next = m_cursor; // Complete the circular linked list

        m_listSize = rhs.m_listSize;
        m_minBufCapacity = rhs.m_minBufCapacity;
    }
}


// Name: operator=(const ListBuffer &rhs) - Overloaded Assignment Operator
// Desc: Assigns one ListBuffer to another, using deep copy.
// Preconditions: An existing ListBuffer object to assign from.
// Postconditions: The current ListBuffer becomes a deep copy of the provided ListBuffer.
const ListBuffer & ListBuffer::operator=(const ListBuffer &rhs) {
    if (this != &rhs) {
        clear(); // Clear current contents

        if (rhs.m_cursor == nullptr) {
            m_cursor = nullptr;
            m_listSize = 0;
            m_minBufCapacity = rhs.m_minBufCapacity;
        } else {
            m_cursor = new ArrayBuffer(*rhs.m_cursor); // Copy first ArrayBuffer
            ArrayBuffer *newCursor = m_cursor;

            ArrayBuffer *currentRHS = rhs.m_cursor->m_next;
            while (currentRHS != rhs.m_cursor) {
                newCursor->m_next = new ArrayBuffer(*currentRHS);
                newCursor = newCursor->m_next;
                currentRHS = currentRHS->m_next;
            }
            newCursor->m_next = m_cursor; // Complete the circular linked list

            m_listSize = rhs.m_listSize;
            m_minBufCapacity = rhs.m_minBufCapacity;
        }
    }
    return *this;
}



void ListBuffer::dump(){
    ArrayBuffer* temp = m_cursor->m_next;
    for(int i=0;i<m_listSize;i++){
        temp->dump();
        temp = temp->m_next;
        cout << endl;
    }
    cout << endl;
}