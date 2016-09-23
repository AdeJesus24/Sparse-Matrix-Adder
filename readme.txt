...........................................................
...........................................................
.....SSSSS...PPPPP......AA.....RRRRR.....SSSSS...EEEEEE....
....S........P....P....A..A....R....R...S........E.........
....S........P....P...A....A...R....R...S........E.........
.....SSSS....P....P...A....A...RRRRR.....SSSS....EEEEE.....
.........S...PPPPP....AAAAAA...R...R.........S...E.........
.........S...P........A....A...R....R........S...E.........
....SSSSS....P........A....A...R.....R..SSSSS....EEEEEE....
...........................................................
...........................................................

HOW TO RUN:
./sparse "operation=<add|multiply>;storage=<array|linkedlist>;recursive=<Y/N>;A=<file>;B=<file>;result=<file>"

HOW TO WRITE INPUTS:
Input lines are triplet values. The first two values are positive integers, and the
final value is any real number. The coordinate for a matrix position is given by
the first two values, while its contents are given by the third. The final matrix value
MUST be given, i.e an [i,j] sized matrix must have a line with i and j as coordinates.
Hashes (#) can be used as comments in the input file.

EXAMPLE FILE:
#A matrix
1 1 2
1 2 4.5
1 3 5
3 2 100
2 2 -5
5 5 .25
100 100 0

#B matrix
1 1 4
2 2 -5
3 3 1000
100 100 -4
1 2 4

NOTES:
You do not need the input files to be sorted, the output will be sorted.
Empty lines and invalid input will be skipped.
Do not put the same coordinates twice in one file.
The recursive loop will be just as efficient as the iterative loop.
Only the addition operator has been implemented.
