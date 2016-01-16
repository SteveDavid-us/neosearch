// QSORTRAND.H


// Randomized quicksort for guaranteed NlogN sort time, but it's about 50% slower (can't 
// remember exactly) on random data than the built in qsort.


void qsortrand(void* base, size_t count, size_t width, int(*compare)(const void* d1, const void* d2));

