#include <stdio.h>
#include "minunit.h"
#include "../src/worker.h"

struct test {
    int a;
};


static int EXECUTED[3];

static int ERR = 0;


int execute(struct test *msg)
{
    static int count = 0;

    if (count == 3) {
        return -1;
    }

    EXECUTED[count++] = msg->a;

    return 0;
}

void handle(int err)
{
    ERR = err;
}

MU_TEST(test_worker)
{
    Queue *queue = queue_new();
    Worker *worker = worker_new(queue, (Executor) execute, handle, 100);

    struct test a = {10}, b = {22}, c = {40};

    worker_run(worker);

    queue_add(queue, &a);
    queue_add(queue, &b);
    queue_add(queue, &c);
    queue_add(queue, &c);

    sleep(1);
    worker_kill(worker);

    mu_assert_int_eq(10, EXECUTED[0]);
    mu_assert_int_eq(22, EXECUTED[1]);
    mu_assert_int_eq(40, EXECUTED[2]);

    mu_assert_int_eq(-1, ERR);
}

int main(void)
{
    MU_RUN_TEST(test_worker);

    MU_REPORT();

    return 0;
}
