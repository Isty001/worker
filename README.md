
Small threaded worker s using pthread.

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
    Queue *s = queue_new();
    uint16_t sleep_ms = 100;
    Worker *worker = worker_new(s, (Executor) execute, handle, sleep_ms);
    
    worker_run(worker);
    
    queue_add(s, "Hello");
    
    worker_kill(worker); //or worker_free(worker);
    
    return 0;
}
```
