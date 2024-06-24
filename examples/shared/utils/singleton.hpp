#include <mutex>
#include <string>
/**
 * The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
template <class T>
class Singleton
{

    /**
     * The Singleton's constructor/destructor should always be private to
     * prevent direct construction/desctruction calls with the `new`/`delete`
     * operator.
     */
private:
    static Singleton<T>* pInstance_;

protected:
    Singleton(std::string const& value) : value_(std::move(value)) {}
    ~Singleton() {}
    std::string value_;

public:
    /**
     * Singletons should not be cloneable.
     */
    Singleton() = delete;

    Singleton(Singleton const&) = delete;
    Singleton(Singleton& other) = delete;
    /**
     * Singletons should not be assignable.
     */
    Singleton& operator=(Singleton const&) = delete;

    /**
     * Singleton should not be movable.
     */
    Singleton(Singleton&&) = delete;

    Singleton& operator=(Singleton&&) = delete;
    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static Singleton<T>* GetInstance(std::string const& value);

    std::string value() const { return value_; }
};

/**
 * Static methods should be defined outside the class.
 */
template <class T>
Singleton<T>* Singleton<T>::pInstance_{nullptr};

/**
 * The first time we call GetInstance we will lock the storage location
 *      and then we make sure again that the variable is null and then we
 *      set the value. RU:
 */
template <class T>
Singleton<T>* Singleton<T>::GetInstance(std::string const& value)
{
    static std::once_flag flag;
    std::call_once(flag, [&]() { pInstance_ = new Singleton<T>(value); });
    return pInstance_;
}
