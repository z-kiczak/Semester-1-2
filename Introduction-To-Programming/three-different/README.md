**Task description** :
Along the highway, which we can imagine as a straight line, there are n motels. Motels are numbered from 1 to n consecutively along the highway.
Each of the motels belongs to a network of motels, which we describe with a number ranging from 1 to n. Bajtek told Bitek that he stopped for the night in a motel
three times while driving on the highway, and each time he stayed in a motel of a different chain.  
Bitek wonders how far apart these three motels must have been. He is interested in the nearest and the farthest three motels.

Formally, Bitek would like to designate three motels A, B, C located along the motorway in this order
and belonging to different networks, for which:  
1. the maximum from the distance from A to B and from B to C is as small as possible (the nearest three),  
2. the minimum from the distance from A to B and from B to C is as high as possible (the farthest three).

Write a program that reads the number of n motels from stdin, followed by descriptions of the motels in order along the highway — for each motel its network number
and distance from the beginning of the highway — and prints two numbers on stdout — as above, the maximum of the distance minima and the minimum of the distance maxima
of the three motels belonging to different networks.  
If Bajtek made a mistake and no three motels of different networks appear along the highway, both numbers should be zero.  
The exact format of the input and output is described in the example below.

**Assumptions** :  
You can assume that the number of motels will be positive and will not exceed one million, and all the numbers in the input will be non-negative and will fit into the int type.  
It may happen that several motels are located at the same point on the highway, or even that Bajtek has stayed overnight in two or three such motels.

Only solutions with linear time complexity can count on maximum score.

**Example input** :  
9  
1 2  
2 6  
2 9  
1 13  
1 17  
3 20  
1 26  
3 27  
1 30  

**Example output** :  
7 10

**Explanation** :  
Motels 3, 4, and 6 (those at distances 9, 13, and 20 from the beginning of the highway) all belong to different networks (2, 1, and 3, respectively) and minimize the maximum
from the distances. Motels 2, 6, and 9 (those within 6, 20, and 30 distances from the start of the highway) all belong to different networks (2, 3, and 1, respectively)
and maximize the minimum from the distances.
