**Task description** :

The data is initially an empty board with dimensions n x m consisting of n*m squares and k blocks. Each of the blocks is assigned an identifier
specified with a capital letter of the English alphabet and is a coherent set of squares, with two squares adjacent to each other when they share a common side.

Write a program that will check whether a certain number of blocks can be placed on the board so that each area of the board is covered by exactly one block.
No block can protrude beyond the board. Each block can be used at most once. The blocks can be rotated in multiples of 90 degrees. If the board can be covered,
the result is board coverage.

**Assumptions** :

You can assume that 1≤n,m,k≤10. The blocks are not empty.

**Input description** :

The first line contains the numbers n, m, k separated by spaces. Then follows the description of k blocks. The ith block is described as n lines of m characters,
with no spaces; A dot indicates a non-block field, and the i-th uppercase letter of the alphabet indicates a block field.
Between the descriptions of the next blocks there is an empty line.

**Output description** :

In the first line of the output, there should be one word TAK(YES) or NIE(NO). The word TAK means that the desired board coverage exists, and the word NIE means
that it does not exist.
If the answer is TAK, then the description of the covered board should follow, in the form of n lines of m characters. Each character should be an uppercase letter
to indicate the ID of the block that you want to cover the field.

If there are many correct ways of coverage, you should list one, any of them.

**Example input** :  
4 5 4  
A....  
A....  
AAAAA  
.....  

..B..  
.BBB.  
.BBB.  
..... 

.....  
.CC..  
.CC..  
.....  

.....  
.DD..  
..D..  
.DDD.  

**Example output** :  
TAK  
AAAAA  
BBDDA  
BBBDA  
BBDDD  
