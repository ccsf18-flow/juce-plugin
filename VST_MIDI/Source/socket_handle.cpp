#include "socket_handle.hpp"

/*
 * Call this to send message.
 * parameters:
 * @tag     -> tag to designate what message is sent
 * @ms      -> current system time in ms message was sent
 * @gen_msg -> message struct to be sent.
 * return   -> returns true on completion, -1 otherwise.
 */

struct sockaddr_in address;
int sock = 0, valread;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};


template<typename T>
bool queue_message(uint8_t tag, uint64_t ms, T gen_msg){
    msg msg_out;
    switch(tag){
        case 1:
            msg_out = {tag, ms, gen_msg};
            break;
    }
    out_msgs.push(msg_out);
    msg_sem.notify();
    return true;
}

void start_threads(){
    pthread_t read_thread;
    pthread_create(&read_thread, NULL, thread_routine, NULL);
}

template<typename T>
bool send_message(uint8_t tag, uint64_t ms, T gen_msg){
    msg msg_out;
    switch(tag){
        case 1:
            msg_out = {tag, ms, gen_msg};
            break;
    }

    uint32_t x = 15;
    send(sock , &msg_out, sizeof(struct msg) , 0 );
    return 1;
}

void * thread_routine(void * args){
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf("\n Socket creation error \n");
        exit(-1);
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "192.168.0.101", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(-1);
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        exit(-1);
    }
    
    
    while(1){
        msg_sem.wait();
        msg msg_out = out_msgs.back();
        out_msgs.pop();
        send_message(1, 1, msg_out.data_msg);
    }
}

/*
 * Call this to recieve data. NOTE: it must be called before send_msg
 * and will halt execution until it completes.
 *
 * return -> msg struct with all information sent. No validation done
 */
msg recv_data(){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addr_len = sizeof(address);
    char buffer[1024] = {0};
    msg rec_val;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    //Force onto port 8080?
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("port binding failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addr_len))<0)
    {
        perror("accept failure");
        exit(EXIT_FAILURE);
    }
    read( new_socket , &rec_val, sizeof(struct msg));
    printf("recieved message\n");
    return rec_val;
}

