Author: Vikram Dudee

Course: CS 463G

Date submitted: 9-7-2016



Collaboration: None, except for looking at pictures of what the puzzle looks like online.


Technical Problems: Segmentation fault when trying to store states of the puzzle. Within
my "Node" class,
I tried to save the bottom 2-D array, the top 2-D array, the inverted
value, the shift value, and the
 heuristic value. This was unsuccessful. What I planned
to do was make a tree where the initial
 generated random state was the root and every
node had three children leaves: 1 from the puzzle state 
resulting from twisting
counter-clockwise, 1 from twisting clockwise, and 1 from flipping. Then, using 
the
Iterative Deepening approach, I was going to put the chain resulting from traversing
down the node 
with the lowest heuristic value into a priority queue to be processed
first since it has the highest 
chance of being correct. A big part of my failures were
due to not being able to clone data of various 
data types from values that were pointed
to without messing up.



Heuristic Value Calculation: I decided to calculate my heuristic by going through every
ball in every 
tube except for the very top ones and comparing them to the ball above to
see if they were the same. 
If they weren't, I incremented my heuristic value because for
every change in ball color through tubes 
because for every transition in ball color,
at least one move it required to separate them. Also,
 an additional point was added to
the h-value if one or more balls were in the current-top tubes 
because that would also
warrant a move to move closer to the solution.
 
Randomize Function: After user is asked
how many random operations they wish to perform, the function takes this in and performs

said random operations, with flip, rotate CW, and rotate CCW each having a 1 in 3 chance
of being the random operation each 
iteration of the loop. 


What I learned: You can accidentally access elements in the wrong part of 2-dimensional
arrays or even get a value that is completely 
irrelevant to your program and must be
very careful in loops that deal with static arrays. No compiler nor syntax error may
arise 
to warn you. Also, the storage of multiple 2-D arrays can be very strenuous on memory.