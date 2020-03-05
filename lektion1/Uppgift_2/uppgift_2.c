
#include <stdio.h>
#include <stdlib.h>
#include "array_2d.h"

#define LO_RIDX 0
#define HI_RIDX 3
#define LO_CIDX 0
#define HI_CIDX 3

int main(){
    array_2d *a2d = array_2d_create(0, 3, 0, 4, free);
    for (int i=LO_RIDX; i < HI_RIDX; i++) {
        for (int j = LO_CIDX; j < HI_CIDX; j++) {
            double *v = malloc(sizeof(*v));
            *v = (double)i + (double)j/10;
            array_2d_set_value(a2d, v, i, j);
            fprintf("%p\n", *(double*)array_2d_inspect_value(a2d, i, j));
        }
    }


    array_2d_kill(a2d);
    return 0;
}
