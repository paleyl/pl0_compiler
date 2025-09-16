usage:
gcc src/pl0.c -o bin/pl0_1.bin # compiling, generate the interpreter
./bin/pl0_1.bin example1/infile1.txt # run the interpreter and generate assembly code and run it

the output looks like:

0var x, y;

1procedure multiply;

1begin

TABLE: 
NULL

3write(x * y)

7end;

2 int 0 3
3 lod 1 3
4 lod 1 4
5 opr 0 4
6 opr 0 14
7 opr 0 15
8 opr 0 0
9begin

TABLE: 
1 var x, lev = 0 addr = 3
2 var y, lev = 0 addr = 4
3 proc multiply, lev = 0 addr = 2 size = 3

10x := 19;

12y := 29;

14write(x + y, x, y);

23call multiply

24end.

9 int 0 5
10 lit 0 19
11 sto 0 3
12 lit 0 29
13 sto 0 4
14 lod 0 3
15 lod 0 4
16 opr 0 2
17 opr 0 14
18 lod 0 3
19 opr 0 14
20 lod 0 4
21 opr 0 14
22 opr 0 15
23 cal 0 2
24 opr 0 0
0 jmp 0 9
1 jmp 0 2
2 int 0 3
3 lod 1 3
4 lod 1 4
5 opr 0 4
6 opr 0 14
7 opr 0 15
8 opr 0 0
9 int 0 5
10 lit 0 19
11 sto 0 3
12 lit 0 29
13 sto 0 4
14 lod 0 3
15 lod 0 4
16 opr 0 2
17 opr 0 14
18 lod 0 3
19 opr 0 14
20 lod 0 4
21 opr 0 14
22 opr 0 15
23 cal 0 2
24 opr 0 0
start pl0
481929
551
