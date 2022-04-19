Big Integer Project
### Condition:
Implement two functions for counting the area and the perimeter of the polygon. The polygon is given as the triad:
+ const size_t n – the amount of dots
+ const double * const X – the X coordinates of the dots.
+ const double * const Y – the Y coordinates of the dots.

In order to count the perimeter, you need to count the length of every edge and sum all the values. You can use #include <cmath> and `sqrt()` function in order to count the square root. \
To count the area, you need to iterate on all of the segments (suppose that seg_start is the start point of the segment, and seg_end is the end point of the segment, and 0 is the point (0,0)): on every iteration you need to count cross product of two vectors: `0 -> seg_start` and `0 -> seg_end`. You need to sum all this cross products that you've got at the each iteration. After that, you need to divide the result by 2 and get the absolute value of the result and return it.
  
The coordinates are sorted (clockwise or counterclockwise).
