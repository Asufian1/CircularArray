// CMSC 341 - Spring 2024 - Project 1
#include "fastq.h"
#include <random>
#include <vector>
#include <algorithm>
class Tester {
public:
    // Test functions for ArrayBuffer
    bool testArrayBufferNormalCases();
    bool testArrayBufferEdgeCases();
    bool testArrayBufferExceptionCases();
    bool testArrayBufferEmptyNormal();
    bool testArrayBufferEmptyEdge();
    bool testArrayBufferCopyConstructorNormalCases();
    bool testArrayBufferAssignmentOperatorNormalCases();
    bool testArrayBufferCopyConstructorEdgeCases();
    bool testArrayBufferAssignmentOperatorEdgeCases();

    // Test functions for ListBuffer
    bool testListBufferNormalCases();
    bool testListBufferDequeueException();
    bool testListBufferCopyConstructorNormalCase();
    bool testListBufferCopyConstructorEdgeCase();
    bool testListBufferAssignmentOperator();
    bool testListBufferEnqueueNormal();
    bool testListBufferEnqueueEdge();
    bool testListBufferDequeueNormal();
    bool testListBufferDequeueEdge();
};


//Test function for ArrayBuffer normal cases.
//Enqueue and dequeue elements to check the correctness of the buffer.
bool Tester::testArrayBufferNormalCases() {
    ArrayBuffer buffer(5);
    try {
        for (int i = 0; i < 5; ++i) {
            buffer.enqueue(i);
        }
        for (int i = 0; i < 5; ++i) {
            if (buffer.dequeue() != i) return false;
        }
        //Expected
        return buffer.empty();
    } catch (...) {
        return false;
    }
}

//Test function for ArrayBuffer edge cases.
//Enqueue and dequeue elements at the buffer's capacity limits.
bool Tester::testArrayBufferEdgeCases() {
    ArrayBuffer buffer(1);
    try {
        buffer.enqueue(1);
        if (buffer.dequeue() != 1) return false;
        buffer.enqueue(2);
        return buffer.dequeue() == 2;
    } catch (...) {
        return false;
    }
}


//Test function for ArrayBuffer exception cases.
//Verify that the correct exceptions are thrown when enqueuing or dequeuing
//beyond buffer limits.
bool Tester::testArrayBufferExceptionCases() {
    bool allTestsPassed = true;

    // Test 1: Enqueue on a Full Buffer
    try {
        ArrayBuffer buffer(2); // Small buffer for test
        buffer.enqueue(1);
        buffer.enqueue(2);
        buffer.enqueue(3); // This should throw an overflow_error
        cout << "Test 1 Failed: No exception thrown for buffer overflow." << endl;
        allTestsPassed = false;
    } catch (const overflow_error& e) {
        cout << "Test 1 Passed: Correctly caught overflow error: " << e.what() << endl;
    } catch (const exception& e) {
        cout << "Test 1 Failed: Caught unexpected exception: " << e.what() << endl;
        allTestsPassed = false;
    }

    // Test 2: Dequeue on an Empty Buffer
    try {
        ArrayBuffer buffer(2);
        buffer.enqueue(1);
        buffer.dequeue(); // Buffer is now empty
        buffer.dequeue(); // This should throw an underflow_error
        cout << "Test 2 Failed: No exception thrown for buffer underflow." << endl;
        allTestsPassed = false;
    } catch (const underflow_error& e) {
        cout << "Test 2 Passed: Correctly caught underflow error: " << e.what() << endl;
    } catch (const exception& e) {
        cout << "Test 2 Failed: Caught unexpected exception: " << e.what() << endl;
        allTestsPassed = false;
    }

    return allTestsPassed;
}

//Test function for empty function in edge case
//should not be empty  if there is at least one item in queue
bool Tester::testArrayBufferEmptyNormal() {
    
    ArrayBuffer buffer(5); 

    // Initially, the buffer should be empty
    if (!buffer.empty()) {
        cout << "Test Failed: Buffer should be empty initially." << endl;
        return false;
    }

    // Add an item to the buffer
    buffer.enqueue(1);

    // Now, the buffer should not be empty
    if (buffer.empty()) {
        cout << "Test Failed: Buffer should not be empty after enqueueing an item." << endl;
        return false;
    }

    return true; // Test passed
}

//Test function for empty function in edge case
//should be empty 
bool Tester::testArrayBufferEmptyEdge() {
    ArrayBuffer buffer(5); 
    // Add and then remove an item
    buffer.enqueue(1);
    buffer.dequeue();

    // After removal, the buffer should be empty again
    if (!buffer.empty()) {
        cout << "Test Failed: Buffer should be empty after dequeuing the only item." << endl;
        return false;
    }

    return true; // Test passed
}



//Test function for ArrayBuffer copy constructor normal cases.
//Verify that a copied ArrayBuffer retains the original's data and behavior.
bool Tester::testArrayBufferCopyConstructorNormalCases() {
    bool allTestsPassed = true;

    // Normal Case 1: Partially filled buffer
    ArrayBuffer original1(5);
    for (int i = 0; i < 3; ++i) {  // Fill less than capacity
        original1.enqueue(i);
    }

    ArrayBuffer copy1(original1);
    for (int i = 0; i < 3; ++i) {
        if (copy1.dequeue() != i) {
            cout << "Normal Case 1 Failed: Copied values do not match." << endl;
            allTestsPassed = false;
            break;
        }
    }

    // Normal Case 2: Fully filled buffer
    ArrayBuffer original2(5);
    for (int i = 0; i < 5; ++i) {  // Fill to capacity
        original2.enqueue(i);
    }

    ArrayBuffer copy2(original2);
    for (int i = 0; i < 5; ++i) {
        if (copy2.dequeue() != i) {
            cout << "Normal Case 2 Failed: Copied values do not match." << endl;
            allTestsPassed = false;
            break;
        }
    }

    return allTestsPassed;
}


//Test function for ArrayBuffer copy constructor edge cases.
//Verify that a copied ArrayBuffer retains the original's data and behavior.
bool Tester::testArrayBufferCopyConstructorEdgeCases() {
    bool allTestsPassed = true;

    // Edge Case 1: Empty buffer
    ArrayBuffer original3(5);
    ArrayBuffer copy3(original3);
    if (!copy3.empty()) {
        cout << "Edge Case 1 Failed: Copy of empty buffer should be empty." << endl;
        allTestsPassed = false;
    }

    // Edge Case 2: Buffer with wrap-around 
    ArrayBuffer original4(5);
    for (int i = 0; i < 5; ++i) {
        original4.enqueue(i);
    }
    original4.dequeue(); // Cause wrap-around
    original4.enqueue(99);

    ArrayBuffer copy4(original4);
    original4.dequeue(); // Modify original after copy
    if (copy4.dequeue() != 1 || copy4.dequeue() != 2) {
        cout << "Edge Case 2 Failed: Wrap-around not handled correctly in copy." << endl;
        allTestsPassed = false;
    }

    return allTestsPassed;
}


//Test function for ArrayBuffer assignment operator normal cases.
//Verify that special cases, like self-assignment, are handled correctly.
bool Tester::testArrayBufferAssignmentOperatorNormalCases() {
    bool allTestsPassed = true;

    // Normal Case 1: Assigning a partially filled buffer
    ArrayBuffer original1(5);
    for (int i = 0; i < 3; ++i) {  // Fill less than capacity
        original1.enqueue(i);
    }

    ArrayBuffer copy1 = original1; // Assignment
    for (int i = 0; i < 3; ++i) {
        if (copy1.dequeue() != i) {
            cout << "Normal Case 1 Failed: Copied values do not match after assignment." << endl;
            allTestsPassed = false;
            break;
        }
    }

    // Normal Case 2: Assigning a fully filled buffer
    ArrayBuffer original2(5);
    for (int i = 0; i < 5; ++i) {  // Fill to capacity
        original2.enqueue(i);
    }

    ArrayBuffer copy2 = original2; // Assignment
    for (int i = 0; i < 5; ++i) {
        if (copy2.dequeue() != i) {
            cout << "Normal Case 2 Failed: Copied values do not match after assignment." << endl;
            allTestsPassed = false;
            break;
        }
    }

    return allTestsPassed;
}


//Test function for ArrayBuffer assignment operator edge cases.
//Verify that special cases, like self-assignment, are handled correctly.
bool Tester::testArrayBufferAssignmentOperatorEdgeCases() {
    
    bool allTestsPassed = true;

    // Edge Case 1: Self-assignment
    ArrayBuffer buffer(5);
    buffer.enqueue(1);
    buffer = buffer; // Self-assignment
    if (buffer.empty() || buffer.dequeue() != 1) {
        cout << "Edge Case 1 Failed: Self-assignment should not modify buffer." << endl;
        allTestsPassed = false;
    }

    // Edge Case 2: Assigning an empty buffer
    ArrayBuffer original3(5);
    ArrayBuffer copy3(5);
    copy3 = original3; // Assignment of empty buffer
    if (!copy3.empty()) {
        cout << "Edge Case 2 Failed: Assigned buffer should be empty." << endl;
        allTestsPassed = false;
    }

    return allTestsPassed;
}



//***** LIST BUFFER TESTS *****/

//Test function for ListBuffer normal cases.
//Enqueue and dequeue elements to check the correctness of the buffer.
bool Tester::testListBufferNormalCases() {
    ListBuffer listBuffer(DEFAULT_MIN_CAPACITY); 

    // Enqueue 10,000 items
    for (int i = 0; i < 10000; ++i) {
        listBuffer.enqueue(i);
    }

    // Dequeue and check the items
    bool testPassed = true;
    for (int i = 0; i < 10000; ++i) {
        try {
            int data = listBuffer.dequeue();
            // if data doesnt match index, fail the test
            if (data != i) {
                cout << "Test Failed: Dequeued data (" << data << ") does not match expected (" << i << ")" << endl;
                testPassed = false;
                break;
            }
        } catch (const std::exception& e) {
            cout << "Test Failed: Exception caught during dequeue: " << e.what() << endl;
            testPassed = false;
            break;
        }
    }
    //else should pass

    return testPassed;
}


//Test function for ListBuffer dequeue exception.
//Verify that an exception is thrown when dequeuing from an empty buffer.
bool Tester::testListBufferDequeueException() {
    bool testPassed = true;
    int minBufCapacity = 100;
    ListBuffer listBuffer(minBufCapacity);

    // Attempt to dequeue from an empty ListBuffer
    try {
        listBuffer.dequeue();
        cout << "Test Failed: No exception thrown for dequeuing from empty queue." << endl;
        testPassed = false;
    } catch (const std::underflow_error& e) {
        cout << "Test Passed: Correctly caught underflow error: " << e.what() << endl;
    } catch (const std::exception& e) {
        cout << "Test Failed: Caught unexpected exception: " << e.what() << endl;
        testPassed = false;
    }

    return testPassed;
}


//Test function for ListBuffer copy constructor normal case.
//Verify that a copied ListBuffer retains the original's data and behavior.
bool Tester::testListBufferCopyConstructorNormalCase() { 
    ListBuffer original(DEFAULT_MIN_CAPACITY);

    // Fill the original ListBuffer with data
    for (int i = 0; i < 300; ++i) {  // More than minBufCapacity to ensure multiple ArrayBuffers are used
        original.enqueue(i);
    }

    // Create a copy of the original ListBuffer
    ListBuffer copy(original);

    // Verify the copy
    bool testPassed = true;
    for (int i = 0; i < 300; ++i) {
        if (copy.dequeue() != i) {
            cout << "Test Failed: Copied values do not match original." << endl;
            testPassed = false;
            break;
        }
    }

    return testPassed;
}


//Test function for ListBuffer copy constructor edge case.
// Verify that special cases, like empty buffers, are handled correctly.
bool Tester::testListBufferCopyConstructorEdgeCase() {
    
    ListBuffer original(DEFAULT_MIN_CAPACITY);

    // Edge Case: Copying an empty ListBuffer
    ListBuffer emptyCopy(original);
    try {
        emptyCopy.dequeue();
        cout << "Test Failed: Expected underflow error for empty copy." << endl;
        return false;
    } catch (const std::underflow_error&) {
        // Expected, pass this case
    } catch (const std::exception& e) {
        cout << "Test Failed: Unexpected exception: " << e.what() << endl;
        return false;
    }

    // Edge Case: ListBuffer with just one ArrayBuffer object
    original.enqueue(42); // Add a single item
    ListBuffer singleCopy(original);
    bool testPassed = singleCopy.dequeue() == 42;

    return testPassed;
}


// Test function for ListBuffer assignment operator.
//Verify that the assignment operator correctly copies ListBuffer data.
bool Tester::testListBufferAssignmentOperator() {
    ListBuffer original(DEFAULT_MIN_CAPACITY);

    // Fill the original ListBuffer with data
    for (int i = 0; i < 300; ++i) {  // More than minBufCapacity to ensure multiple ArrayBuffers are used
        original.enqueue(i);
    }

    // Assign the original ListBuffer to a new ListBuffer
    ListBuffer copy = original;

    // Clear the original ListBuffer
    original.clear();

    // Verify the copy retained its data
    bool testPassed = true;
    for (int i = 0; i < 300; ++i) {
        try {
            if (copy.dequeue() != i) {
                cout << "Test Failed: Copied values do not match expected values after clearing the original." << endl;
                testPassed = false;
                break;
            }
        } catch (const std::exception& e) {
            cout << "Test Failed: Exception caught during dequeue: " << e.what() << endl;
            testPassed = false;
            break;
        }
    }

    return testPassed;
}


//Test function for ListBuffer enqueue normal case.
//Enqueue elements to check the correctness of the buffer.
bool Tester::testListBufferEnqueueNormal() {
    int minBufCapacity = 100;
    ListBuffer listBuffer(minBufCapacity);

    // Enqueue a reasonable number of items
    try {
        for (int i = 0; i < 500; ++i) {
            listBuffer.enqueue(i);
        }
    } catch (const std::exception& e) {
        cout << "Test Failed: Exception caught during enqueue: " << e.what() << endl;
        return false;
    }

    // Additional check: Ensure items are enqueued correctly
    bool testPassed = true;
    for (int i = 0; i < 500; ++i) {
        if (listBuffer.dequeue() != i) {
            testPassed = false;
            break;
        }
    }

    return testPassed;
}


//Test function for ListBuffer dequeue normal case.
// dequeue elements to check the correctness of the buffer.
bool Tester::testListBufferEnqueueEdge() {
    int minBufCapacity = 10; // A smaller capacity to trigger multiple ArrayBuffer creation
    ListBuffer listBuffer(minBufCapacity);

    // Enqueue a large number of items
    try {
        for (int i = 0; i < 10000; ++i) {
            listBuffer.enqueue(i);
        }
    } catch (const std::exception& e) {
        cout << "Test Failed: Exception caught during enqueue: " << e.what() << endl;
        return false;
    }

    return true; // If no exceptions were thrown, the test is passed
}

// Test function for ListBuffer normal cases.
   // Enqueue and dequeue elements to check the correctness of the buffer.
bool Tester::testListBufferDequeueNormal() {
    ListBuffer listBuffer(DEFAULT_MIN_CAPACITY);

    // Fill the buffer
    for (int i = 0; i < 500; ++i) {
        listBuffer.enqueue(i);
    }

    // Dequeue and check the items
    bool testPassed = true;
    for (int i = 0; i < 500; ++i) {
        if (listBuffer.dequeue() != i) {
            cout << "Test Failed: Dequeued data does not match expected." << endl;
            testPassed = false;
            break;
        }
    }

    return testPassed;
}
// Test function for ListBuffer edge cases.
bool Tester::testListBufferDequeueEdge() {
    ListBuffer listBuffer(DEFAULT_MIN_CAPACITY);

    // Fill the buffer
    for (int i = 0; i < 10000; ++i) {
        listBuffer.enqueue(i);
    }

    // Dequeue and check the items
    bool testPassed = true;
    for (int i = 0; i < 10000; ++i) {
        if (listBuffer.dequeue() != i) {
            cout << "Test Failed: Dequeued data does not match expected." << endl;
            testPassed = false;
            break;
        }
    }

    return testPassed;
}


int main(){
    Tester tester;

    //testArrayBufferEnqueue();
    cout << "Testing ArrayBufferNormal case: " << (tester.testArrayBufferNormalCases() ? "Passed" : "Failed") << endl;
    cout << "Testing ArrayBufferEdge case: " << (tester.testArrayBufferEdgeCases() ? "Passed" : "Failed") << endl;
    cout << "Testing ArrayBuffer Exception Cases: " << (tester.testArrayBufferExceptionCases() ? "Passed" : "Failed") << endl;
    cout << "Testing Array Buffer Empty Normal case: " << (tester.testArrayBufferEmptyNormal() ? "Passed" : "Failed") << endl;
    cout << "Testing Array Buffer Empty Edge case: " << (tester.testArrayBufferEmptyEdge() ? "Passed" : "Failed") << endl;
    cout << "Testing ArrayBufferCopyConstructor: " << (tester.testArrayBufferCopyConstructorNormalCases() ? "Passed" : "Failed") << endl;
    cout << "Testing ArrayBufferCopyConstructor: " << (tester.testArrayBufferCopyConstructorEdgeCases() ? "Passed" : "Failed") << endl;
    cout << "Testing ArrayBufferAssignmentOperator Normal Cases: " << (tester.testArrayBufferAssignmentOperatorNormalCases() ? "Passed" : "Failed") << endl;
    cout << "Testing ArrayBufferAssignmentOperator Normal Cases: " << (tester.testArrayBufferAssignmentOperatorNormalCases() ? "Passed" : "Failed") << endl;
    cout << "Testing ArrayBufferAssignmentOperator Edge Cases: " << (tester.testArrayBufferAssignmentOperatorEdgeCases() ? "Passed" : "Failed") << endl;
    
    cout << "Testing testListBuffer NormalCases: " << (tester.testListBufferNormalCases() ? "Passed" : "Failed") << endl;
    cout << "Testing ListBuffer Dequeue Operation on Empty Queue: " << (tester.testListBufferDequeueException() ? "Passed" : "Failed") << endl;
    cout << "Testing List Buffer Copy Constructor Edge Case: " << (tester.testListBufferCopyConstructorEdgeCase() ? "Passed" : "Failed") << endl;
    cout << "Testing List Buffer Assignment operator: " << (tester.testListBufferAssignmentOperator()  ? "Passed" : "Failed") << endl;
    cout << "Testing List Buffer Enqueue Normal case: " << (tester.testListBufferEnqueueNormal()  ? "Passed" : "Failed") << endl;
    cout << "Testing List Buffer Enqueue Edge case: " << (tester.testListBufferEnqueueEdge()  ? "Passed" : "Failed") << endl;
    cout << "Testing List Buffer Dequeue Normal case: " << (tester.testListBufferDequeueNormal()  ? "Passed" : "Failed") << endl;
    cout << "Testing List Buffer Enqueue edge case: " << (tester.testListBufferDequeueEdge()  ? "Passed" : "Failed") << endl;
    return 0;
}