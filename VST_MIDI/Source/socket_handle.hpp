#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <queue>
#include <condition_variable>
#define PORT 8080


struct beat_msg{
    uint8_t note;
    uint8_t intensity;
} __attribute__((packed)) ;

struct pitch_peak_msg {
	uint8_t holder;
} __attribute__((packed)) ;


struct msg {
    uint8_t tag;
    uint64_t time_in_ms;
    
    union{
        beat_msg b_msg;
        pitch_peak_msg p_msg;
    } data_msg;
}  __attribute__((packed));

class semaphore
{
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.
    
public:
    void notify() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }
    
    void wait() {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        while(!count_) // Handle spurious wake-ups.
            condition_.wait(lock);
        --count_;
    }
    
    bool try_wait() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        if(count_) {
            --count_;
            return true;
        }
        return false;
    }
};

std::queue<msg> out_msgs;
pthread_mutex_t msgs_lock;
std::condition_variable msg_add;
semaphore msg_sem;

msg recv_data();
bool send_message(uint8_t tag, uint64_t ms, void* data_msg);
bool queue_message(uint8_t tag, uint64_t ms, void* data_msg);
void start_threads();
void * thread_routine(void*);
