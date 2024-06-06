#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>

#define BUFFER_SIZE 256

const char* scriptPath="/home/team3/Desktop/proiect/numar_imagine.py";
FILE* file;
// Message structure
typedef struct {
    char* message;
    struct message_t* next;
} message_t;

// Stack for messages to be sent to Arduino
message_t* send_stack = NULL;

// List for messages received from Arduino
message_t* receive_list = NULL;

// Mutex to ensure thread-safe access to stacks/lists
pthread_mutex_t mutex;

// aici am copiat din prezentare codul pt rulare python
void runPythonScript(const char* scriptPath) {
    char command[256];
    sprintf(command, "python3 %s", scriptPath);
    // Open a pipe to execute the Python script
    FILE* pipe = popen(command, "r");
    if (!pipe) {
    printf("Error opening pipe to Python script.\n");
    }
    char buffer[256];
    int result = 0;
    // Read the output of the Python script
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
    printf("Python script returned: %s\n", buffer);
    //pclose(pipe);
    //return buffer;
    }
    pclose(pipe);
    //return "";
}

//pana aici cine

// Functions for message handling
void push(message_t** head, char* message) {
    message_t* new_node = (message_t*) malloc(sizeof(message_t));
    new_node->message = message;
    new_node->next = NULL;
    if(*head ==NULL)
    {
    (*head) = new_node;
}
else{
    message_t* temp =*head;
    while(temp->next !=NULL){
        temp=temp->next;
}
temp->next =new_node;
}
}

char* pop(message_t** head) {
    if (*head == NULL) {
        return NULL;
    }
    char* message = (*head)->message;
    message_t* temp = *head;
    (*head) = (*head)->next;
    free(temp);
    return message;
}

void append(message_t** head, char* message) {
    message_t* new_node = (message_t*) malloc(sizeof(message_t));
    new_node->message = message;
    new_node->next = NULL;
    if (*head == NULL) {
        (*head) = new_node;
        return;
    }
    else{
        
        message_t* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
}
}

// Thread function for sending messages to Arduino
void* send_thread_func(void* arg) {
    
    int arduino_fd = *(int*) arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        char* message = pop(&send_stack);
        pthread_mutex_unlock(&mutex);
        if (message != NULL) {
            serialPuts(arduino_fd, message);
            serialPutchar(arduino_fd, '\n');
            printf("Sent message to Arduino: %s\n", message);
            free(message);
        }
    }
    return NULL;
}

// Thread function for receiving messages from Arduino
void* receive_thread_func(void* arg) {
     
    int arduino_fd = *(int*) arg;
    char buffer[BUFFER_SIZE];
    int index=0;
    while (1) {
        if(serialDataAvail(arduino_fd)){
            char ch = serialGetchar(arduino_fd);
            if(ch == '\n'){
                buffer[index] = '\0';
                printf("Received message from Arduino: %s\n", buffer);
                if(strcmp(buffer,"CAMERA")==0)
                {
                    runPythonScript(scriptPath);
                    file=fopen("fisier.txt", "r");
                    if(file==NULL)
                    {
                        printf("Nu s-a putut deschide fisierul");
                        exit(-1);
                    }
                    if(fgets(buffer, sizeof(buffer), file)==NULL)
                    {
                        printf("Eroare la scrierea in buffer");
                        exit(-1);
                    }
                    fclose(file);
                }   
                else
                {
                    strcpy(buffer,"");
                    printf("Received message from Arduino: %s\n", buffer);
                }
                    
                pthread_mutex_lock(&mutex);
                append(&receive_list, strdup(buffer));
                pthread_mutex_unlock(&mutex);
                pthread_mutex_lock(&mutex);
                push(&send_stack, buffer);
                pthread_mutex_unlock(&mutex);
                index=0;
            }
            else{
                if(index < BUFFER_SIZE -1){
                    buffer[index++] = ch;
                }
        }
    }
}
    return NULL;
}

// Thread function for generating messages to be sent to Arduino
void* generate_thread_func(void* arg) {
    while (1) {
        char* message = (char*) malloc(BUFFER_SIZE);
        printf("Enter message to send to Arduino: ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';
        pthread_mutex_lock(&mutex);
        push(&send_stack, message);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}







int main() {
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Open the serial port for communication with Arduino
    int arduino_fd = serialOpen("/dev/ttyUSB0", 9600);
    if (arduino_fd < 0) {
        fprintf(stderr, "Failed to open serial port\n");
        return 1;
    }

    // Set up the three threads
    pthread_t send_thread, receive_thread, generate_thread;

    pthread_create(&receive_thread, NULL, receive_thread_func, (void*) &arduino_fd);    
    pthread_create(&send_thread, NULL, send_thread_func, (void*) &arduino_fd);
    
    //pthread_create(&generate_thread, NULL, generate_thread_func, NULL);

    // Wait for the threads to finish
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);
    //pthread_join(generate_thread, NULL);

    // Clean up the mutex and serial port
    pthread_mutex_destroy(&mutex);
    serialClose(arduino_fd);

    return 0;
}
