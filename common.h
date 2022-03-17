
template <typename T>
struct Vector {
    T *arr = NULL;
    size_t len = 0;
    size_t cap = 0;

    void push(T val);
    T get(int index);
};

void *xmalloc(size_t n_bytes);
void *xrealloc(void *ptr, size_t n_bytes);
const char *intern_str_range(const char *start, const char *end);
