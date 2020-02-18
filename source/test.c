#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int main(){
	int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

	signal(SIGINT, sigint_handler);

	while(1){
		if(hardware_read_stop_signal()){
		    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		    break;
		}

		if(hardware_read_floor_sensor(0)){
		    hardware_command_movement(HARDWARE_MOVEMENT_UP);
		}
		if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
		    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
		}
		if(hardware_read_obstruction_signal()){
			hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		}
	}

return 0;
}
