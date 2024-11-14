**Task description** :

The task is to effectively simulate the events occurring on the working day of the office. There are m windows in the office, numbered from 0 to m-1.
There are customers queuing up to each window. When the clerk is free, they serve the first customer in the queue, after which they leave the office
and does not return that day.
In addition, there are various other situations in the office, described by the functions in kol.h header file.

**Other requirements**:
The library does not free up memory after any customer. This is the responsibility of the library user. The user releases the memory only when the client leaves the office.
**Note**: The user will be freeing memory after the prospects using the free function, so library must allocate memory using the malloc function.

**Example usage**:
g++ @opcjeCpp main.cpp kol.cpp -o main.e
