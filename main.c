#include <Windows.h>
#include "scaler.h"

int main(void)
{
    scaler_init();

    while(1){
        scaler_run();
        //msleep(50);
    }
    return 0;
}

