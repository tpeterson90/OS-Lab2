#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(void){
    const int numberOfChildern = 15; //total child processes
    pid_t childPids[numberOfChildern]; // Store child PIDs
    int status; 
    int normal = 0; //counts for summary
    int nonezero = 0; //counts for summary
    int exit1 = 0; //counts for summary

    //Array of 15 unique child command processes
    char *args[][3] = {
        {"ls","-l", NULL},
        {"echo", "Hello Tasha", NULL},
        {"date", NULL, NULL},
        {"pwd", NULL, NULL},
        {"mkdir", NULL, NULL},
        {"ps", NULL, NULL},
        {"df", "-a", NULL},
        {"id", NULL, NULL},
        {"mv", NULL, NULL},
        {"rm", NULL, NULL},
        {"uname", NULL, NULL},
        {"touch", NULL, NULL},
        {"ln", NULL, NULL},
        {"free", NULL, NULL},
        {"clear", NULL, NULL}

    };
    //Print parent PID
    printf("Parent process PID: %d\n", getpid());

    //Create children processes
    for (int i=0;i<numberOfChildern;i++){
        pid_t pid = fork(); //Generate new PID

        //error handling
        if(pid<0){
            perror("fork failed");
            return 1;
        }
        //Child
        if(pid==0){
            printf("Child #%d PID=%d executing: %s\n", i, getpid(), args[i][0]);

            //Two children intentionally terminate
            if(i == 3 || i ==9 ){
                abort();
            }

            // Two children intentionally invaild command
            if(i ==5 || i ==12){
                char *bad[] = {"Not a real command", NULL};
                execvp(bad[0], bad);
                perror("execvp failed: invaild command");
                exit(127);
            }
            //All other children replaced with commands
            execvp(args[i][0], args[i]);
            //error handling
            perror("execvp failed");
            exit(127);

        //Parent
            }
            childPids[i] = pid;
        }
    
        
    //Parent process waits for all child processes
    printf("Print out history: -----");

    //wait for child to be created
    for(int i = 0; i < numberOfChildern; i++){
        pid_t pid = waitpid(childPids[i], &status, 0);
    
        if (WIFEXITED(status)){
            int exit_status = WEXITSTATUS(status);
            printf("Child %d normal exit %d\n", pid, exit_status);
        
            if (exit_status == 0){
                normal++;
            }else{
                nonezero++;
            }
        }else {
            exit1++;
        }
    }
    
    printf("\nSummary:\n");
    printf("Total Children: %d\n", numberOfChildern);
    printf("Normal Children: %d\n", normal);
    printf("Terminated: %d\n", exit1);
    printf("Invalid Command: %d\n", nonezero);

return 0;
}