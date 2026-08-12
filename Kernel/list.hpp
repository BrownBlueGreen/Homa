


template <class T, T* T::*Link>
class IntrusiveList {

public:

  void pushBack(T* t) {
    t->*Link = nullptr;
    
    if (tail_) tail_->*Link = t;
    else head_ = t;

    tail_ = t;
  }

  void pushFront(T* t) {
    t->*Link = head_;
    head_ = t;

    if(!tail_) tail_ = t;    
  }

  T* popBack() {
    if(empty()) return nullptr;

    T* t = tail_;

    if (head_ == tail_) head_ = tail_ = nullptr;
    else {
      T* h = head_;
      while(h->*Link != tail_) h = h->*Link;
      h->*Link = nullptr;
      tail_ = h;
    }

    t->*Link = nullptr;
    return t;
  }

  T* popFront() {
    if (empty()) return nullptr;

    auto t = head_;
    head_ = head_->*Link;

    if (head_ == nullptr) tail_ = head_;
    t->*Link = nullptr;

    return t;
  }

  bool remove(T* t) {
    if (empty()) return false;

    if (head_ == t) { popFront(); return true; } 
    if (tail_ == t) { popBack(); return true; }

    for (T* h = head_; h->*Link; h = h->*Link) {
      if (h->*Link == t) {
        h->*Link = t->*Link;
        t->*Link = nullptr;
        return true;
      }
    }
    return false;
  }

  void insertSorted(T* t, bool (*less)(const T*, const T*)) {
    T** pp = &head_;
    while (*pp && !less(t, *pp)) pp = &((*pp)->*Link);
    t->*Link = *pp;
    if (!*pp) tail_ = t;
    *pp = t;
  }

  bool contains(const T* t) const {
    for (const T* h = head_; h; h = h->*Link)
      if (h == t) return true;
    return false;
  }

  T* front() const {
    return head_;
  }

  T* back() const {
    return tail_;
  }
  
  bool empty() const { return head_ == nullptr; }

private:

  T* head_ = nullptr;
  T* tail_ = nullptr;

};