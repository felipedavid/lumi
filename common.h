
template <typename T>
struct Vector {
    T *arr = NULL;
    size_t len = 0;
    size_t cap = 0;

    void push(T val);
    T get(int index);
};

void *xmalloc(size_t n_bytes);
