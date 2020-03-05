#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void){

    int die, lastdie, count, unfinished, throws;
    char *dienames[] = {"ones", "twos", "threes", "fours", "fives", "sixes"};

    srand(time(NULL));

    printf("This program throws dice until we get the same number thrice in a row\n");
    count = 0;
    unfinished = 1;
    lastdie = 0;
    throws = 0;

    while (unfinished){

        /* Simulate a throw */
        throws++;
        die = rand()%6 + 1;
        printf("Throw %d shows %d dots\n", throws, die);

        /* Compare to the last one */
        if (die == lastdie)
            count++;
        else
            count = 0;
        lastdie = die;
        unfinished = count !=  2; //While loop was set to continue as long as count == 2 which exited the loop on first entry
        //Changed to continue the loop as long as count != 2 which means to loop while we haven't thrown the same dots thrice.
    }

    printf("We got three %s after %d throws\n", dienames[die-1], throws);
    return 1;
}
