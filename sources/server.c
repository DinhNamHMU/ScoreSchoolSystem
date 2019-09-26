#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
    
#define TRUE 1 
#define FALSE 0 
#define PORT 8080 
#define MAX 80 

typedef struct Student {
    char username[MAX];
    char password[MAX];
    char fullname[MAX];
    char studenID[MAX];
    float math;
    float physic; 
    float chemistry; 
} student;

void getDatabase(student st[], int quantity, FILE *fp);
void printDatabase(student st[], int quantity); 
int checkAccount(student st[], int quantity, char *message);
void handlingScore(student st[], int quantity, char *message, int index, int sd);

int main(int argc, char *argv[]) { 
    int opt = TRUE; 
    int master_socket, addrlen, new_socket, client_socket[30], max_clients = 30, activity, i, valread , sd, n; 
    int max_sd; 
    struct sockaddr_in address; 
    char buffer[1025]; 
    char message[1025];
    int quantity, index, logined = 0;

    char *greeting_Message = "Hi, I'm a Server. Please log in to continue! [0Username:Password]\n"; 
    char *LogInFailure_Message="Username or password incorrect. Please log in to continue! [0Username:Password]\n";
    char *RepeatLogIn_Message="You are not logged in. Please log in to continue! [0Username:Password]\n";
    char *LogOutSuccess_Message="You haved logged out. Please log in to continue! [0Username:Password]\n";
    char *exitClient = "exit";

    student st[100];

    // Get database
    FILE *fp;
    fp = fopen("../databases/Database.txt","r");
    fscanf(fp,"%d", &quantity);
    getDatabase(st, quantity, fp);
    fclose(fp);
    
    // printDatabase(st, quantity);
        
    // Set of socket descriptors 
    fd_set readfds; 
        
    // Initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++) { 
        client_socket[i] = 0; 
    } 
        
    // Create a master socket 
    if( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Set master socket to allow multiple connections, this is just a good habit, it will work without this 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Type of socket created 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
        
    // Bind the socket to localhost port 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
        
    // Try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
        
    // Accept the incoming connection 
    addrlen = sizeof(address); 
    puts("Waiting for connections ..."); 
        
    while(TRUE) { 
        // Clear the socket set 
        FD_ZERO(&readfds); 
    
        // Add master socket to set 
        FD_SET(master_socket, &readfds); 
        max_sd = master_socket; 
            
        // Add child sockets to set 
        for ( i = 0; i < max_clients; i++) { 
            // Socket descriptor 
            sd = client_socket[i]; 
                
            // If valid socket descriptor then add to read list 
            if(sd > 0) 
                FD_SET(sd, &readfds); 
                
            // Highest file descriptor number, need it for the select function 
            if(sd > max_sd) 
                max_sd = sd; 
        } 
    
        // Wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely 
        activity = select( max_sd + 1, &readfds, NULL, NULL, NULL); 
    
        if ((activity < 0) && (errno!=EINTR)) { 
            printf("Select error\n"); 
        } 
            
        // If something happened on the master socket, then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds)) { 
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
                perror("accept"); 
                exit(EXIT_FAILURE); 
            } 
            puts("New connection!\n");
        
            // Send new connection greeting message 
            if (send(new_socket, greeting_Message, strlen(greeting_Message), 0) != strlen(greeting_Message)) { 
                perror("send"); 
            } 
                
            // Add new socket to array of sockets 
            for (i = 0; i < max_clients; i++) { 
                // If position is empty 
                if( client_socket[i] == 0 ) { 
                    client_socket[i] = new_socket; 
                    break; 
                } 
            } 
        } 
            
        // Else its some IO operation on some other socket 
        for (i = 0; i < max_clients; i++) { 
            sd = client_socket[i]; 
                
            if (FD_ISSET(sd , &readfds)) { 
                // Check if it was for closing , and also read the incoming message 
                if ((valread = read(sd, buffer, 1024)) == 0) { 
                    // Somebody disconnected, get his details and print 
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen); 
                    printf("Host disconnected\n");
                        
                    // Close the socket and mark as 0 in list for reuse 
                    close(sd); 
                    client_socket[i] = 0; 
                } 
                    
                // Echo back the message that came in 
                else { 
                    printf("From Client: %s", buffer); 
                    buffer[valread] = '\0';

                    // Handling request from Client
                    if (buffer[0] == '0') {
                        int j=0;
                        for (i=0; i < sizeof(buffer); i++) {
                            message[j]=buffer[i+1];
                            j++;
                        }
                        index = checkAccount(st, quantity, message);
                        if (index == -1) {
                            send(sd, LogInFailure_Message, strlen(LogInFailure_Message), 0);
                        }
                        else {
                        	// Login with StudentID
    						char LogInSuccess_Message[200]="Congratulations. Successful login. Can I help you, please!";
                        	strcat(LogInSuccess_Message, " ");
                        	strcat(LogInSuccess_Message, st[index].fullname);
                        	strcat(LogInSuccess_Message, ", ");
                        	strcat(LogInSuccess_Message, st[index].studenID);
                        	LogInSuccess_Message[strlen(LogInSuccess_Message)] = '.';
                        	LogInSuccess_Message[strlen(LogInSuccess_Message)] = '\n';
                        	
                            send(sd, LogInSuccess_Message, strlen(LogInSuccess_Message), 0);
                            logined = 1;
                        }
                    }
                    else if (buffer[0] == '1') { 
                        if (logined == 0) {
                            send(sd, RepeatLogIn_Message, strlen(RepeatLogIn_Message), 0);
                        }
                        else {
                            // Access scores
                            int j=0;

                            for (i=0; i < sizeof(buffer); i++) {
                                message[j]=buffer[i+1];
                                j++;
                            }

                            // Handling string 
                            char studentID[200];
						    char subject[200];
						    int count = 0;
						    int count1 = 0;
						    int count2 = 0;
						    int currentIndex;

						    while (message[count] != ':') {
						    	studentID[count1] = message[count];
						    	count++;
						    	count1++;
						    }
						    count++;
						    while (message[count] != '\n') {
						    	subject[count2] = message[count];
						    	count++;
						    	count2++;
						    }
						    studentID[count1] = '\0';
						    subject[count2] = '\0';

						    // Access Scores with StudentID;
						    // Check account in database
						    for (int i=0; i < quantity; i++) {
						    	if (strcmp(st[i].studenID, studentID) == 0) {
						    		currentIndex = i;
						    	}
						    }
                            handlingScore(st, quantity, subject, currentIndex, sd);
                        }
                        
                    }
                    else if (buffer[0] == '2') {
                        if (logined == 0) {
                            send(sd, RepeatLogIn_Message, strlen(RepeatLogIn_Message), 0);
                        }
                        else {
                            // Exit. Log out
                            int j=0;
                            for (i=0; i < sizeof(buffer); i++) {
                                message[j]=buffer[i+1];
                                j++;
                            }
                            index=-1;
                            logined=0;
                            send(sd, LogOutSuccess_Message, strlen(LogOutSuccess_Message), 0);
                        }

                    }
                    else if (strcmp(buffer, "exit")) {
                    	send(sd, exitClient, strlen(exitClient), 0);
                    }
                    else {
                        send(sd, RepeatLogIn_Message, strlen(RepeatLogIn_Message), 0);
                    }
                }

            } 
        } 
    } 
    return 0; 
} 

void getDatabase(student st[], int quantity, FILE *fp) {
    char ss[5];
    fgets(ss, 5, fp);
    for (int i = 0; i < quantity; i++) {
        fgets(st[i].username, MAX, fp);
        st[i].username[strlen(st[i].username)-1] = '\0';

        fgets(st[i].password, MAX, fp);
        st[i].password[strlen(st[i].password)-1] = '\0';

        fgets(st[i].fullname, MAX, fp);
        st[i].fullname[strlen(st[i].fullname)-1] = '\0';

        fgets(st[i].studenID, MAX, fp);
        st[i].studenID[strlen(st[i].studenID)-1] = '\0';

        fscanf(fp, "%f", &st[i].math);
        fscanf(fp, "%f", &st[i].physic);
        fscanf(fp, "%f", &st[i].chemistry);
        fgets(ss, 5, fp);
    }
}

void printDatabase(student st[], int quantity) {
    for (int i = 0; i < quantity; i++)
        printf("%s %s %s %s %0.1f %0.1f %0.1f \n", st[i].username, st[i].password, st[i].fullname, st[i].studenID, 
                                                   st[i].math, st[i].physic, st[i].chemistry);
}

int checkAccount(student st[], int quantity, char *message) {
    char username[200];
    char password[200];
    int j = 0;

    while (*message != ':') {
        username[j] = *message;
        message++;
        j++;
    }
    username[j] = '\0'; 
    printf("Username: %s\n",username);

    j = 0;
    message++;
    while (*message != '\n') {
        password[j] = *message;
        message++;
        j++;
    }
    password[j] = '\0';
    printf("Password: %s\n",password);

    //Check account in database
    for (int i = 0; i < quantity; i++) {
        if (strcmp(st[i].username, username)==0 && strcmp(st[i].password, password)==0) {
            return i;
        }
    }
    return -1;
 }

 void handlingScore(student st[], int quantity, char message[], int index, int sd) {
    char math[] = "Math"; char physic[] = "Physic"; char chemistry[] = "Chemistry"; char all[] = "All";
    int errorMath = 0, errorPhysic = 0, errorChemistry = 0, errorAll = 0;
    char buf[200] = ""; char buf1[200] =  ""; char buf2[200] = ""; char buf3[200] = "";
    char bufFullName[200] = "Your full name: ";

    gcvt(st[index].math, 200, buf1);
    gcvt(st[index].physic, 200, buf2);
    gcvt(st[index].chemistry, 200, buf3);

    for (int i = 0; i<sizeof(math)-1; i++) {
        if (math[i] != message[i]) {
            errorMath=1;
            break;
        }
    }

    if (errorMath == 0) {
        strcat(bufFullName, st[index].fullname);

        strcat(bufFullName, "; Your student ID: ");
        strcat(bufFullName, st[index].studenID);

        strcat(bufFullName, "; Math: ");
        strcat(bufFullName, buf1);
        strcat(bufFullName, " score.\n");
        send(sd, bufFullName, strlen(bufFullName), 0);
    }

    for (int i = 0; i<sizeof(physic)-1; i++) { 
        if (physic[i] != message[i]) {
            errorPhysic=1;
            break;
        }
    }

    if (errorPhysic == 0) {
        strcat(bufFullName, st[index].fullname);

        strcat(bufFullName, "; Your student ID: ");
        strcat(bufFullName, st[index].studenID);

        strcat(bufFullName, "; Physic: ");
        strcat(bufFullName, buf2);
        strcat(bufFullName, " score.\n");
        send(sd, bufFullName, strlen(bufFullName), 0);
    }

    for (int i = 0; i<sizeof(chemistry)-1; i++) {
        if (chemistry[i] != message[i]) {
            errorChemistry=1;
            break;
        }
    }

    if (errorChemistry == 0) {
        strcat(bufFullName, st[index].fullname);

        strcat(bufFullName, "; Your student ID: ");
        strcat(bufFullName, st[index].studenID);

        strcat(bufFullName, "; Chemistry: ");
        strcat(bufFullName, buf3);
        strcat(bufFullName, " score.\n");
           
        send(sd, bufFullName, strlen(bufFullName), 0);
    }

    for (int i = 0; i<sizeof(all)-1; i++) {
        if (all[i] != message[i]) {
            errorAll=1;
            break;
        }
    }

    if (errorAll == 0) {
        char bufAll[] = "";
        strcat(bufAll, "Math: ");
        strcat(bufAll, buf1);
        strcat(bufAll, " score");
        strcat(bufAll, "; Physic: ");
        strcat(bufAll, buf2);
        strcat(bufAll, " score");
        strcat(bufAll, "; Chemistry: ");
        strcat(bufAll, buf3);
        strcat(bufAll, " score.\n");

        send(sd, bufAll, strlen(bufAll), 0);
    }
 }