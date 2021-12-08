#include<stdio.h>

// AJ Gonzalez
// OS - Allen Gottlieb
// Lab 1: FIFO/RR Scheduling

// Create a structure for a single process
typedef struct process{
    int pid;
    int r1;
    int b1;
    int r2;
    int b2;
    // All States: 0 - ready 1 - running 2 - blocked
    // 3 - terminating 4 - terminating
    int currentState;
    int inLine; 
    int blocked; 
} process;

// Create a method to keep a running list of processes in the queue
int manageLine(process allProcesses[], int num){
    int loc, x, y;
    int count = 1;

    for(x = 0; x < num; x++){
        // visit each process
        for(y = 0; y < num; y++){
            // iterate through each process
            int pos = allProcesses[y].inLine;
            if(pos == count){
                count++;
                break;
            }
        }
    }

    return count;
}

int main(){
    // Variables from the user 
    int n, sa, q, i;

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter '1' for FIFO or '2' for RR: ");
    scanf("%d", &sa);

    // If red robin is chosen, get q value
    if (sa == 2){
        printf("Enter quantum value: ");
        scanf("%d", &q);
    }

    process allProcesses[n];

    for (i = 0; i < n; i++){
        process new;
        int r1, b1, r2, b2;

        // Prompt user for the R and B values
        printf("Enter the R1, B1, R2, B2 values for processes %d: ", (i+1));
        new.pid = i+1;
        new.currentState = 0;
        // Initilize position in queue
        new.inLine = i + 1;
        scanf("%d", &r1);
        new.r1 = r1;
        scanf("%d", &b1);
        new.b1 = b1;
        scanf("%d", &r2);
        new.r2 = r2;
        scanf("%d", &b2);
        new.b2 = b2;

        allProcesses[i] = new;
    }

    // Split between either FIFO or RR

    //FIFO
    if (sa == 1){

        // Track how many processes have been terminated
        int done = 0;
        int cycleCount = 1;
        int running = 0;
        int check = 0;
        int c;

        // Loop through running processes 
        while (done < n){
            printf("Cycle #%d: ", cycleCount);
            for(c = 0; c < n; c++){
                process temp = allProcesses[c];

                if(((temp.currentState == 0) && (running == 0)) || (temp.currentState == 1 && running == 1)){
                    allProcesses[c].currentState = 1;
                    running = 1;
                    printf("Process %d: Running, ", temp.pid);

                    if(temp.r1 != 0){
                        allProcesses[c].r1--;
                    } else if (temp.r1 == 0 && temp.r2 != 0){
                        allProcesses[c].r2--;
                    }

                    if(allProcesses[c].r1 == 0 && temp.b1 != 0){
                        allProcesses[c].currentState = 2;
                        check = 1;
                    }else if (allProcesses[c].r2 == 0){
                        allProcesses[c].currentState = 2;
                        check = 1;
                    }
                } else if (temp.currentState == 0){
                    printf("Process #%d: Ready, ", temp.pid);
                } else if (temp.currentState == 2){
                    printf("Process #%d: Blocked, ", temp.pid);

                    if(temp.b1 != 0){
                        allProcesses[c].b1--;
                    } else if (temp.b1 == 0 && temp.b2 != 0){
                        allProcesses[c].b2--;
                    }

                    if(allProcesses[c].b1 == 0 && allProcesses[c].r2 != 0){
                        allProcesses[c].currentState = 0;
                    } else if (allProcesses[c].b2 == 0){
                        allProcesses[c].currentState = 3;
                    }
                } else if ((temp.currentState == 3) && (running == 0)){
                    printf("Process #%d: Terminated, ", temp.pid);
                    allProcesses[c].currentState = 4;
                    done++;
                }
            }
            
            if (check == 1){
                running = 0;
                check = 0;
            }

            printf("\n");
            int i;
            for(i = 0; i < n; i++){
                process x = allProcesses[i];
                printf("Process #%d Current Data: \n R1:\t %d \n B1:\t %d \n R2:\t %d \n B2:\t %d \n", x.pid, x.r1, x.b1, x.r2, x.b2);
            }    
            
            cycleCount++;
            if(cycleCount == 20){
                done = n;
            }

        }
    
    // Round Robin (RR)
    } else if (sa == 2){
        int done = 0;
        int cycleCount = 1;
        int running = 0;
        int check = 0;
        int qCount = 0;
        int block = 0;
        int c,x,y,z;

        while (done < n){

            printf("Cycle #%d: ", cycleCount);

            int theQueue = n;
            for (x = 0; x < n; x++){
                int qLoc = allProcesses[x].inLine;

                if (qLoc > 0){
                    if (qLoc < theQueue){
                        theQueue = qLoc;
                    }
                }
            }

            if (theQueue != 1){
                for(y = 0; y < n; y++){
                    allProcesses[y].inLine--;
                }
            }

            for (c = 0; c < n; c++){
                process temp = allProcesses[c];

                // Run
                if(((temp.currentState == 0 && running == 0) || (temp.currentState == 1 && running == 1)) && temp.inLine == 1){
                    allProcesses[c].currentState = 1;
                    running = 1;
                    qCount++;
                    printf("Process #%d: Running, ", temp.pid);

                    if (temp.r1 != 0){
                        allProcesses[c].r1--;
                    } else if (temp.r1 == 0 && temp.r2 != 0){
                        allProcesses[c].r2--;
                    }

                    if((allProcesses[c].r1 == 0) && (temp.b1 != 0)){
                        allProcesses[c].currentState = 2;
                        check = 1;
                    } else if (allProcesses[c].r2 == 0){
                        allProcesses[c].currentState = 2;
                        check = 1;
                    } else if (qCount == q){
                        allProcesses[c].currentState = 0;
                        check = 1;
                    }

                    // Ready
                } else if (temp.currentState == 0){
                    printf("Process #%d: Ready, ", temp.pid);

                    //Block
                } else if (temp.currentState == 2){
                    // add a blocked process
                    printf("Process #%d: Blocked, ", temp.pid);
                    allProcesses[c].inLine = n + 1;

                    if(temp.b1 != 0){
                        allProcesses[c].b1--;
                    } else if (temp.b1 == 0 && temp.b2 != 0){
                        allProcesses[c].b2--;
                    }

                    if((allProcesses[c].b1 == 0) && (allProcesses[c].r2 != 0)){
                        allProcesses[c].currentState = 0;

                        if(check == 1){
                            block = 1;
                        }

                        int makeQ = manageLine(allProcesses, n);
                        allProcesses[c].inLine = makeQ;
                    } else if (allProcesses[c].b2 == 0){
                        allProcesses[c].currentState = 3;
                    }
                } else if (temp.currentState == 3){
                    if (running == 0){
                        running = 1;
                        printf("Process #%d: Terminated, ", temp.pid);
                        allProcesses[c].currentState = 4;
                        allProcesses[c].inLine = -1;
                        done ++;
                        check = 1;
                    } else {
                        printf("Process #%d: Ready, ", temp.pid);
                    }
                }
                
            }
            
            if(check == 1){
                running = 0;
                check = 0;
                qCount = 0;

                if (block == 0){
                    for (z = 0; z < n; z++){
                        allProcesses[z].inLine--;
                        if(allProcesses[z].inLine == 0){
                            allProcesses[z].inLine = n;
                        }
                    }
                }

                block = 0;

            }

            // Print Output
            printf("\n");
            int i;
            for(i = 0; i < n; i++){
                process x = allProcesses[i];
                printf("Process #%d Current Data: \n R1:\t %d \n B1:\t %d \n R2:\t %d \n B2:\t %d \n", x.pid, x.r1, x.b1, x.r2, x.b2);
            }

            cycleCount ++;
            if(cycleCount == 20){
                    done = n;
            }

        }

    }

    return 1;
}