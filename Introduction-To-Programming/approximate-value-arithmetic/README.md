First project in the course - "Approximate Value Arithmetic"

**Short description of the task** :  
Where we measure physical quantities, the results are subject to some error, e.g. 5m ± 10%.
We treat each such approximate value as a set of possible values.
Implement a package of arithmetic operations on such approximate values that conforms to the included ary.h interface.

**Assumptions** :  
We implicitly assume that all double arguments are real numbers (i.e. they are different from HUGE_VAL, -HUGE_VAL and NAN). 
However, if the result is not a real number, it should be the appropriate value: HUGE_VAL, -HUGE_VAL or NAN.

Assume that the modifiers close the resulting sets of values – that is, if the result is an open interval, then assume that it is converted to a closed interval.
Operations on approximate values are monotonic due to the inclusion of sets of possible values.

You can assume that a double is zero if the following function evaluates it to true:  
**bool iszero(double x) { return fabs(x) < 1e-10; }**

**Example usage** : 
Some examples based on the above principle can be found in the example.c file. Compile command (ary.c and ary.h files must be in the directory):  
**gcc @opcje przyklad.c ary.c -o przyklad.e -lm**

**Comments** :  
Floating-point numbers and operations on them can be surprising.  
For example, the IEEE standard provides for two representations of zero (+0.0 and -0.0), with 1.0 /. 0.0 = HUGE_VAL, and 1.0 /. (-0.0) = -HUGE_VAL.  
This can be helpful, for example, if you are dividing by an approximate value that contains a one-sided neighborhood of zero.
But it can also be a trap when you're considering dividing by a value exactly equal to zero.
