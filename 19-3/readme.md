Реализовать функции с сигнатурой:

typedef double (*function_t)(double);

double* pmap_process(function_t func, const double *in, size_t count);
void pmap_free(double *ptr, size_t count);

Функция pmap_process является функцией высшего порядка, которая принимает на вход некоторую произвольную функцию func, применяет её ко всем элементам массива in, и возвращает указатель на полученный результат.

Для этого создается N процессов, которые соответствует количеству ядер (или процессоров) для распараллеливания вычислений.

Функция pmap_free должна освобождать память, выделенную в функции pmap_process.

Использовать ожидание завершения процесса для синхронизации запрещено, но тем не менее, необходимо выполнять очистку от процессов-зомби.

Для определения количества процессоров используйте функцию get_nprocs.