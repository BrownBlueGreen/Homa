

/* NOTE: THIS QUEUE IS NOT THREADSAFE! */
template <typename T, std::size_t N>
class Queue final {
public:

  Queue(uint32_t cap) : 
  size_(0), 
  cap_(cap),
  head_(nullptr),
  tail_(nullptr) {}

  bool push(T& data) {
    if ((size_ - 1) == cap_) return false; // can't push 
    
  }

  T& pop() {

  }

  const T& head() const noexcept {

  }

  const T& tail() const noexcept {

  }

private:

  uint32_t size_;
  uint32_t cap_;
  uint32_t head_;
  uint32_t tail_;
  T data_[N];
}




