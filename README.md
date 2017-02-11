
Small threaded worker queue using pthread.

### Example

```c
int execute(char *msg)
{
    puts(msg);
    
    if (somethin_went_wrong) {
        return -1;
    }
    
    return 0;
}

void handle(int err)
{
    printf("Something went wrong. Error %d", err);
}

int main(void)
{
    Queue *queue = queue_new();
    uint16_t sleep_ms = 100;
    Worker *worker = worker_new(queue, (Executor) execute, handle, sleep_ms);
    
    worker_run(worker);
    
    queue_add(queue, "Hello");
    
    worker_kill(worker);
    
    return 0;
}
```
