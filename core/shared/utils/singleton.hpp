#ifndef SINGLETON_ATOMIC_HPP_
#define SINGLETON_ATOMIC_HPP_

#include <atomic>
#include <mutex>
#include <utility>

template <typename Derived>
class Singleton
{
public:
    template <typename... Args>
    static void Construct(Args&&... args)
    {
#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
        using Instance = Derived;
#else
        struct Dummy final : Derived
        {
            using Derived::Derived;
            consteval void
            ProhibitConstructFromDerived() const noexcept override
            {
            }
        };
        using Instance = Dummy;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

        if (!instance_.load(std::memory_order_acquire))
        {
            std::unique_lock lock{mutex_};

            if (!instance_.load(std::memory_order_relaxed))
            {
                instance_.store(new Instance{std::forward<Args>(args)...},
                                std::memory_order_release);
                lock.unlock();
                instance_.notify_all();
            }
        }
    }

    static Derived* GetInstance() noexcept
    {
        instance_.wait(nullptr, std::memory_order_acquire);
        return instance_.load(std::memory_order_relaxed);
    }

protected:
    Singleton() = default;
    Singleton(Singleton const&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton const&) = delete;
    Singleton& operator=(Singleton&&) = delete;
#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
    ~Singleton() = default;
#else
    virtual ~SingletonAtomic() = default;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

private:
    struct Deleter
    {
        ~Deleter() noexcept(noexcept(std::declval<Derived>().~Derived()))
        {
            delete instance.load(std::memory_order_acquire);
        }

        std::atomic<Derived*> instance{nullptr};
    };

#ifdef SINGLETON_INJECT_ABSTRACT_CLASS
    consteval virtual void ProhibitConstructFromDerived() const noexcept = 0;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

    inline static Deleter deleter_;
    inline static auto& instance_{deleter_.instance};
    inline static std::mutex mutex_;
};

#endif // SINGLETON_ATOMIC_HPP_
