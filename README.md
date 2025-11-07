# SYSC 4001 Assignment 2 Part 2

# Question 1
    gcc -o q1 part2_q1_101272210_101316700.c
    ./q1

    ps -ef | grep q1
    kill PID

# Question 2
    gcc -o q2_proc1 part2_q2_proc1_101272210_101316700.c
    gcc -o q2_proc2 part2_q2_proc2_101272210_101316700.c
    ./q2_proc1

    ps -ef | grep q2_proc1
    ps -ef | grep q2_proc2
    kill PID
