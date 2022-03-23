#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class ThreadSafeQueue{
public:
    ThreadSafeQueue() = default;

    T front()
    {
        std::lock_guard guard(mutex_);
        const T front_element = queue_.front();
        return front_element;
    }

    T back()
    {
        std::lock_guard guard(mutex_);
        const T back_element = queue_.back();
        return back_element;
    }

    bool empty()
    {
        std::lock_guard guard(mutex_);
        const bool is_empty = queue_.empty();
        return is_empty;
    }

    void push(T elem)
    {
        std::lock_guard guard(mutex_);
        queue_.push(elem);
    }

    void pop()
    {
        std::lock_guard guard(mutex_);
        queue_.pop();
    }

    auto size()
    {
        std::lock_guard guard(mutex_);
        return queue_.size();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
};

void push_n_times(ThreadSafeQueue<int>& queue, int n)
{
    for(int i = 0; i < n; i ++){
        queue.push(1);
    }
}

int main()
{
    ThreadSafeQueue<int> queue;
    for(auto i = 0; i < 30000; i ++){
        queue.push(1);
    }
    std::thread thread(push_n_times, std::ref(queue), 30000);
    for(auto i = 0; i < 30000; i ++){
        queue.pop();
    }
    thread.join();
    std::cout << "Size: " << queue.size() << '\n';
    std::cout << "Front: " << queue.front() << '\n';
    std::cout << "Back: " << queue.back() << '\n';
    std::cout << "Is empty: " << std::boolalpha << queue.empty() << '\n';
    return 0;
}
