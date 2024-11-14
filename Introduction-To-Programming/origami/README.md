**Task description.**

Write a program for origami fans to study how many layers a cleverly folded piece of paper has at a given point.  
The program should load a description of creating successive origami cards and questions about the number of layers in the given points of the stdin cards.
In the first line of the input there are two numbers n and q, denoting the number of sheets and the number of questions, respectively. Then, at the input,
there are n rows describing successive sheets and q questions about the number of layers of the cards in the given points.

The description of the i-th sheet (1 ≤ and ≤ n) can be found in line i + 1.  
This description has one of three forms:  
1. P x1 y1 x2 y2
2. K x y r
3. Z k x1 y1 x2 y2

The first form represents a closed rectangle with sides parallel to the axis of the coordinate system with the lower left corner at the point P1 = (x1, y1)
and the upper right corner at the point P2 = (x2, y2). The point P1 must therefore be in non-sharp angle to the left and down from the point P2.  
When a pin is inserted into the sheet inside (or at the edges) of the rectangle, the sheet will be pierced 1 time, otherwise 0 times.
 
The second form represents a closed circle centered at the point (x, y) and radius r.

The third form represents a sheet of paper formed by folding the k-th sheet of paper (1 ≤ k< i) ) along a line passing through the points
P1 = (x1, y1) and P1 = (x2, y2) - they must be different points.

The paper is folded in such a way that it is turned from the right side of the straight line (looking in the direction from P1 to P2) to the left.
For such a folded sheet, the puncture on the right side of the straight gives 0 layers; piercing exactly on a straight line should return the same amount as
piercing the sheet before folding; on the left side - as much as before folding plus the puncture of the unfolded sheet of paper at the point that overlapped the puncture point.

The j-th query (1 ≤ j ≤ q) is in the line n + j + 1 and is of the form k x y, where k is the number of the sheet (1 ≤ k ≤ n) and(x, y) is the point where
we stick the pin to determine the number of layers of the k-th sheet at this point. The program should print to stdout q rows - answers to queries about the number of layers.  
Notes: The numbers denoting the coordinates of the points or the radius of the circle are floating-point numbers.

**Example input** :  
4 5  
P 0 0 2.5 1  
Z 1 0.5 0 0.5 1  
K 0 1 5  
Z 3 0 1 -1 0  
1 1.5 0.5  
2 1.5 0.5  
2 0 0.5  
3 4 4  
4 4 4  

**Example output** :  
1  
0  
2  
1  
2  
