#include <stdio.h>
#include <pthread.h>

struct custom_data{
    double x;
    int y;
    double ans;
};

void* custom_power(void* arg) {
    struct custom_data* inp = (struct custom_data*) arg;
    double x = inp->x;
    int y = inp->y;
    double tmp_res = 0;

    if (y == 0) {
        tmp_res = 0;
    }
    else if (y == 1) {
        tmp_res = x;
    }
    else {
        struct custom_data inp_power_1 = {x, (y / 2), 0};
        struct custom_data inp_power_2 = {x, (y / 2) + (y % 2), 0};

        pthread_t thread_power_1, thread_power_2;

        pthread_create(&thread_power_1, NULL, custom_power, &inp_power_1);
        pthread_create(&thread_power_2, NULL, custom_power, &inp_power_2);

        pthread_join(thread_power_1, NULL);
        pthread_join(thread_power_2, NULL);

        double res_power_1 = inp_power_1.ans,
               res_power_2 = inp_power_2.ans;

        tmp_res = res_power_1 * res_power_2;
    }

    inp->ans = tmp_res;

    return NULL;
}

int main() {
    double x, ans = 0;
    int y;

    printf("x = ");
    scanf("%lf", &x);

    printf("y = ");
    scanf("%d", &y);

    if (y == 0) {
        ans = x;
    }
    else {
        struct custom_data function_input = {x, y, 0};

        pthread_t thread;
        pthread_create(&thread, NULL, custom_power, &function_input);
        pthread_join(thread, NULL);

        ans = function_input.ans;
    }

    printf("%lf", ans);

    return 0;
}
