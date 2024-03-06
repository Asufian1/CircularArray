A large e-commerce company needs to increase the efficiency of their customer service system. Their customer service department works all year around, however, there are some periods with a very high number of requests per day. The current system places the incoming calls in a queue in the order they arrive. Since the number of calls are not sometimes predictable the queue overflows. Once a queue overflow happens the current system creates a larger queue and copies the calls from the older one to the new one. This is a very inefficient process. The Company is looking for a solution that does not overflow and is fast.

You are assigned a task to design and implement a queue which can dynamically expand and shrink and still performs the operations in an efficient manner. This data structure is a combination of circular arrays and a circular linked list. The following figures present an array queue which will become full after two more insertions.


Since the array is full and the calls keep coming in, we create a new array which holds twice as many calls. All new calls will be placed in the new queue. At this point we have two queues, we dequeue from the old array and we enqueue in the new array.


Now, the question is that what if the second queue becomes full too? The solution is creating a third queue and add it to the set of queues. To manage the set of queues we add them to a linked list. To perform the linked list operations efficiently we use a circular linked list. The linked list will change dynamically too since we need to delete the older arrays that become empty.


If we need to allocate a new array queue, we add it as a node after the current cursor and the new node will be the cursor. If we need to deallocate an old array queue, in the linked list we remove the oldest node which is the next node of current cursor. One may notice that the array queues are stored in the linked list in the order they were created.
