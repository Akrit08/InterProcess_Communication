#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Signals defined
#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500

// user defined signal
#define MEDIAN_FOUND 404

// variables declared
int id0 = 0;
int id1 = 1;
int id2 = 2;
int id3 = 3;
int id4 = 4;
int id5 = 5;
int n = 25;
int randElement;

// functions declared
int isChildReady(int *fds, int id);
void childProcess(int *fdr, int *fdw, char fileName[]);
void setChildIds(int *parent_ch1, int *parent_ch2, int *parent_ch3,
                 int *parent_ch4, int *parent_ch5);
int sendParentReq(int i, int *fds, int *fd);
void sendParentPivot(int *pfdw);
void sendPivotElement(int pivot, int *pfdw);

int main()
{
    // seeding done to get different random values
    srand(getpid());

    // initialized pipes for parent to all children
    int i, parent_ch1[2], parent_ch2[2], parent_ch3[2], parent_ch4[2],
        parent_ch5[2], child1_prt[2], child2_prt[2], child3_prt[2],
        child4_prt[2], child5_prt[2];
    pipe(parent_ch1); // pipe used by the parent to child 1 process
    pipe(parent_ch2); // pipe used by the parent to child 2 process
    pipe(parent_ch3); // pipe used by the parent to child 3 process
    pipe(parent_ch4); // pipe used by the parent to child 4 process
    pipe(parent_ch5); // pipe used by the parent to child 5 process

    pipe(child1_prt); // pipe used by the parent to child 1 process
    pipe(child2_prt); // pipe used by the parent to child 2 process
    pipe(child3_prt); // pipe used by the parent to child 3 process
    pipe(child4_prt); // pipe used by the parent to child 4 process
    pipe(child5_prt); // pipe used by the parent to child 5 process

    // create first child process
    if (fork() == 0)
    {
        childProcess(&parent_ch1[0], &child1_prt[1], "input_1.txt");
    }
    // create second child process
    else if (fork() == 0)
    {
        childProcess(&parent_ch2[0], &child2_prt[1], "input_2.txt");
    }
    // create third child process
    else if (fork() == 0)
    {
        childProcess(&parent_ch3[0], &child3_prt[1], "input_3.txt");
    }
    // create fourth child process
    else if (fork() == 0)
    {
        childProcess(&parent_ch4[0], &child4_prt[1], "input_4.txt");
    }
    // create fifth child process
    else if (fork() == 0)
    {
        childProcess(&parent_ch5[0], &child5_prt[1], "input_5.txt");
    }
    else
    {
        // Parent Process
        int pivot = 0;
        // set child ids
        setChildIds(&parent_ch1[1], &parent_ch2[1], &parent_ch3[1],
                    &parent_ch4[1], &parent_ch5[1]);
        // is child ready
        isChildReady(&child1_prt[0], 1);
        isChildReady(&child2_prt[0], 2);
        isChildReady(&child3_prt[0], 3);
        isChildReady(&child4_prt[0], 4);
        isChildReady(&child5_prt[0], 5);
        // prints parent READY
        printf("Parent READY\n");
        /*
        initialized k = ceil(n/2) takes ceiling value to avoid double value
        if n is even
        */
        int k = ceil(n / 2);

        // continous loop
        while (1)
        {
            // select random integer between 5 and 1
            int randIntSelected = rand() % (5 + 1 - 1) + 1;
            // if condition if the value is 1
            if (randIntSelected == 1)
            {
                pivot = sendParentReq(1, &parent_ch1[1], &child1_prt[0]);
            }
            // if condition if the value is 2
            else if (randIntSelected == 2)
            {
                pivot = sendParentReq(2, &parent_ch2[1], &child2_prt[0]);
            }
            // if condition if the value is 3
            else if (randIntSelected == 3)
            {
                pivot = sendParentReq(3, &parent_ch3[1], &child3_prt[0]);
            }
            // if condition if the value is 4
            else if (randIntSelected == 4)
            {
                pivot = sendParentReq(4, &parent_ch4[1], &child4_prt[0]);
            }
            // else condition if the value is 5
            else
            {
                pivot = sendParentReq(5, &parent_ch5[1], &child5_prt[0]);
            }

            // if the pivot value is -1 if the size of the array is 0
            while (pivot == -1)
            {
                // select random integer between 5 and 1
                randIntSelected = rand() % (5 + 1 - 1) + 1;

                // if condition if the value is 1
                if (randIntSelected == 1)
                {
                    pivot = sendParentReq(1, &parent_ch1[1], &child1_prt[0]);
                }
                // if condition if the value is 2
                else if (randIntSelected == 2)
                {
                    pivot = sendParentReq(2, &parent_ch2[1], &child2_prt[0]);
                }
                // if condition if the value is 3
                else if (randIntSelected == 3)
                {
                    pivot = sendParentReq(3, &parent_ch3[1], &child3_prt[0]);
                }
                // if condition if the value is 4
                else if (randIntSelected == 4)
                {
                    pivot = sendParentReq(4, &parent_ch4[1], &child4_prt[0]);
                }
                // else condition if the value is 5
                else
                {
                    pivot = sendParentReq(5, &parent_ch5[1], &child5_prt[0]);
                }
            }

            // send PIVOT signal to all child using parent to child pipe
            sendParentPivot(&parent_ch1[1]);
            sendParentPivot(&parent_ch2[1]);
            sendParentPivot(&parent_ch3[1]);
            sendParentPivot(&parent_ch4[1]);
            sendParentPivot(&parent_ch5[1]);

            // send pivot value to all child using parent to child pipe
            sendPivotElement(pivot, &parent_ch1[1]);
            sendPivotElement(pivot, &parent_ch2[1]);
            sendPivotElement(pivot, &parent_ch3[1]);
            sendPivotElement(pivot, &parent_ch4[1]);
            sendPivotElement(pivot, &parent_ch5[1]);
            // prints the following statement
            printf("Parent broadcasts pivot %i to all children\n", pivot);

            /*
            initialize m and m1, m2, m3, m4, m5 as values of respective
            childs
             */
            int m, m1, m2, m3, m4, m5;
            // read value passed from child to parent pipe and print them
            read(child1_prt[0], &m1, sizeof(int));
            printf("Child 1 receives pivot and replies %i\n", m1);
            read(child2_prt[0], &m2, sizeof(int));
            printf("Child 2 receives pivot and replies %i\n", m2);
            read(child3_prt[0], &m3, sizeof(int));
            printf("Child 3 receives pivot and replies %i\n", m3);
            read(child4_prt[0], &m4, sizeof(int));
            printf("Child 4 receives pivot and replies %i\n", m4);
            read(child5_prt[0], &m5, sizeof(int));
            printf("Child 5 receives pivot and replies %i\n", m5);

            // sum m1, m2, m3, m4, m5 to get value of m
            m = m1 + m2 + m3 + m4 + m5;
            // if condition if m = k
            if (m == k)
            {
                // print the following statements that prints median
                printf("Parent: m = %i + %i + %i + %i + %i = %i. 12 = %i/2.",
                       m1, m2, m3, m4, m5, m, n);
                printf(" Median found!\n");
                printf("Parent sends kill signals to all children\n");
                // initialize medianCommand = MEDIAN_FOUND
                int medianCommand = MEDIAN_FOUND;
                // write MEDIAN_FOUND signal to all children
                write(parent_ch1[1], &medianCommand, sizeof(int));
                write(parent_ch2[1], &medianCommand, sizeof(int));
                write(parent_ch3[1], &medianCommand, sizeof(int));
                write(parent_ch4[1], &medianCommand, sizeof(int));
                write(parent_ch5[1], &medianCommand, sizeof(int));
                exit(0);
            }
            // if condition if m is greater than k
            else if (m > k)
            {
                // initialize smallCommand = SMALL
                int smallCommand = SMALL;
                // write SMALL signal to all children
                write(parent_ch1[1], &smallCommand, sizeof(int));
                write(parent_ch2[1], &smallCommand, sizeof(int));
                write(parent_ch3[1], &smallCommand, sizeof(int));
                write(parent_ch4[1], &smallCommand, sizeof(int));
                write(parent_ch5[1], &smallCommand, sizeof(int));
            }
            // if condition if m is lesser than k
            else if (m < k)
            {
                // initialize largeCommand = LARGE
                int largeCommand = LARGE;
                // write LARGE signal to all children
                write(parent_ch1[1], &largeCommand, sizeof(int));
                write(parent_ch2[1], &largeCommand, sizeof(int));
                write(parent_ch3[1], &largeCommand, sizeof(int));
                write(parent_ch4[1], &largeCommand, sizeof(int));
                write(parent_ch5[1], &largeCommand, sizeof(int));
                // subtract k by m
                k = k - m;
            }
        }
    }
    return 0;
}

// function that checks to see if child is ready with parameters *fds and id
int isChildReady(int *fds, int id)
{
    int val_from_child;
    // read value from child by child to parent pipe
    read(*fds, &val_from_child, sizeof(int));
    // if condition if value from child is READY then print they are READY
    if (val_from_child == READY)
    {
        printf("Child %i sends READY\n", id);
        return 1;
    }
    return 0;
}

// function for child process with parameters *fdr, *fdw and fileName
void childProcess(int *fdr, int *fdw, char fileName[])
{
    int child_id;
    // read child id using parent to child pipe
    read(*fdr, &child_id, sizeof(int));
    FILE *fp;
    fp = fopen(fileName, "r");
    int size = 5;
    // read file into array numberArray
    int numberArray[size];
    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%d", &numberArray[i]);
    }

    int ready_val = READY;
    // write READY signal using child to parent pipe
    write(*fdw, &ready_val, sizeof(int));
    int readValParent;
    int pivot;

    // continous loop
    while (1)
    {
        read(*fdr, &readValParent, sizeof(int));
        /*
        if condition to check if the signal sent from parent is REQUEST and
        check if the randomly selected child's array's size is 0
        */
        if (readValParent == REQUEST)
        {
            // if condition if size = 0 then write -1 to parent
            if (size == 0)
            {
                int negOne = -1;
                write(*fdw, &negOne, sizeof(int));
            }
            /*
            else gets a random value from the selected child's array and
            write it to parent using respective child to parent pipe
            */
            else
            {
                int randVal = ((rand() % (((size - 1) + 1) - (0))) + (0));
                randElement = numberArray[randVal];
                if (write(*fdw, &randElement, sizeof(int)) != -1)
                {
                    printf("Child %i sends %i to parent\n", child_id,
                           randElement);
                }
            }
        }

        /*
        if condition to check if the signal sent from parent is PIVOT and if
        true reads the pivot from parent
        */
        else if (readValParent == PIVOT)
        {
            int numLargerThanPivot = 0;
            read(*fdr, &pivot, sizeof(int));
            // if size is not equal to 0 then counts number larger than pivot
            if (size != 0)
            {
                for (int i = 0; i < size; i++)
                {
                    if (numberArray[i] > pivot)
                    {
                        numLargerThanPivot++;
                    }
                }
            }
            // else initialize numLargerThanPivot to 0
            else
            {
                numLargerThanPivot = 0;
            }
            // write the value of numLargerThanPivot to parent
            write(*fdw, &numLargerThanPivot, sizeof(int));
        }

        /*
        condition to check if the signal sent from parent is SMALL and check
        if any element in child's array are smaller than the pivot
        */
        else if (readValParent == SMALL)
        {
            int index = 0;
            // while condition until index is less than size
            while (index < size)
            {
                // checks if the element is less than pivot
                if (numberArray[index] < pivot)
                {
                    if (index < (size - 1))
                    {
                        for (int j = index; j < (size - 1); j++)
                        {
                            numberArray[j] = numberArray[j + 1];
                        }
                    }
                    size--; // updates size of list as it removes the element
                }
                else
                    index++;
            }
        }

        /*
        condition to check if the signal sent from parent is LARGE and check
        if the element in child's array is larger than the pivot
        */
        else if (readValParent == LARGE)
        {
            int index = 0;
            // while condition until index is less than size
            while (index < size)
            {
                // checks if the element is greater than pivot
                if (numberArray[index] > pivot)
                {
                    if (index < (size - 1))
                    {
                        for (int j = index; j < (size - 1); j++)
                        {
                            numberArray[j] = numberArray[j + 1];
                        }
                    }
                    size--; // updates size of list as it removes the element
                }
                else
                    index++;
            }
        }
        /*
        condition to check if the signal sent from parent is MEDIAN_FOUND
        and if true terminates the process.
        */
        else if (readValParent == MEDIAN_FOUND)
        {
            printf("Child %i terminates \n", child_id);
            exit(0);
        }
    }
}

// function to set ids for all childs and pipe them to the respective child
void setChildIds(int *parent_ch1, int *parent_ch2, int *parent_ch3,
                 int *parent_ch4, int *parent_ch5)
{
    int id = 1;
    write(*parent_ch1, &id, sizeof(int));
    id = 2;
    write(*parent_ch2, &id, sizeof(int));
    id = 3;
    write(*parent_ch3, &id, sizeof(int));
    id = 4;
    write(*parent_ch4, &id, sizeof(int));
    id = 5;
    write(*parent_ch5, &id, sizeof(int));
}

// function to send REQUEST signal and read the pivot returned
int sendParentReq(int i, int *fds, int *fd)
{
    int requestVal = REQUEST;
    if (write(*fds, &requestVal, sizeof(int)) != -1)
    {
        printf("Parent sends REQUEST to Child %i\n", i);
    }
    int pivot;
    read(*fd, &pivot, sizeof(int));
    return pivot;
}

// function to send PIVOT signal
void sendParentPivot(int *pfdw)
{
    int pivotVal = PIVOT;
    write(*pfdw, &pivotVal, sizeof(int));
}

// function to send pivot value
void sendPivotElement(int pivot, int *pfdw)
{
    write(*pfdw, &pivot, sizeof(int));
}
