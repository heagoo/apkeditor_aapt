// -*- C++ -*-
//===-------------------------- memory ------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_MEMORY
#define _LIBCPP_MEMORY

/*
    memory synopsis

namespace std
{

struct allocator_arg_t { };
constexpr allocator_arg_t allocator_arg = allocator_arg_t();

template <class T, class Alloc> struct uses_allocator;

template <class Ptr>
struct pointer_traits
{
    typedef Ptr pointer;
    typedef <details> element_type;
    typedef <details> difference_type;

    template <class U> using rebind = <details>;

    static pointer pointer_to(<details>);
};

template <class T>
struct pointer_traits<T*>
{
    typedef T* pointer;
    typedef T element_type;
    typedef ptrdiff_t difference_type;

    template <class U> using rebind = U*;

    static pointer pointer_to(<details>) noexcept;
};

template <class Alloc>
struct allocator_traits
{
    typedef Alloc                        allocator_type;
    typedef typename allocator_type::value_type
                                         value_type;

    typedef Alloc::pointer | value_type* pointer;
    typedef Alloc::const_pointer
          | pointer_traits<pointer>::rebind<const value_type>
                                         const_pointer;
    typedef Alloc::void_pointer
          | pointer_traits<pointer>::rebind<void>
                                         void_pointer;
    typedef Alloc::const_void_pointer
          | pointer_traits<pointer>::rebind<const void>
                                         const_void_pointer;
    typedef Alloc::difference_type
          | pointer_traits<pointer>::difference_type
                                         difference_type;
    typedef Alloc::size_type
          | make_unsigned<difference_type>::type
                                         size_type;
    typedef Alloc::propagate_on_container_copy_assignment
          | false_type                   propagate_on_container_copy_assignment;
    typedef Alloc::propagate_on_container_move_assignment
          | false_type                   propagate_on_container_move_assignment;
    typedef Alloc::propagate_on_container_swap
          | false_type                   propagate_on_container_swap;
    typedef Alloc::is_always_equal
          | is_empty                     is_always_equal;

    template <class T> using rebind_alloc  = Alloc::rebind<U>::other | Alloc<T, Args...>;
    template <class T> using rebind_traits = allocator_traits<rebind_alloc<T>>;

    static pointer allocate(allocator_type& a, size_type n);
    static pointer allocate(allocator_type& a, size_type n, const_void_pointer hint);

    static void deallocate(allocator_type& a, pointer p, size_type n) noexcept;

    template <class T, class... Args>
        static void construct(allocator_type& a, T* p, Args&&... args);

    template <class T>
        static void destroy(allocator_type& a, T* p);

    static size_type max_size(const allocator_type& a); // noexcept in C++14

    static allocator_type
        select_on_container_copy_construction(const allocator_type& a);
};

template <>
class allocator<void>
{
public:
    typedef void*                                 pointer;
    typedef const void*                           const_pointer;
    typedef void                                  value_type;

    template <class _Up> struct rebind {typedef allocator<_Up> other;};
};

template <class T>
class allocator
{
public:
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef typename add_lvalue_reference<T>::type       reference;
    typedef typename add_lvalue_reference<const T>::type const_reference;
    typedef T                                     value_type;

    template <class U> struct rebind {typedef allocator<U> other;};

    allocator() noexcept;
    allocator(const allocator&) noexcept;
    template <class U> allocator(const allocator<U>&) noexcept;
    ~allocator();
    pointer address(reference x) const noexcept;
    const_pointer address(const_reference x) const noexcept;
    pointer allocate(size_type, allocator<void>::const_pointer hint = 0);
    void deallocate(pointer p, size_type n) noexcept;
    size_type max_size() const noexcept;
    template<class U, class... Args>
        void construct(U* p, Args&&... args);
    template <class U>
        void destroy(U* p);
};

template <class T, class U>
bool operator==(const allocator<T>&, const allocator<U>&) noexcept;

template <class T, class U>
bool operator!=(const allocator<T>&, const allocator<U>&) noexcept;

template <class OutputIterator, class T>
class raw_storage_iterator
    : public iterator<output_iterator_tag,
                      T,                               // purposefully not C++03
                      ptrdiff_t,                       // purposefully not C++03
                      T*,                              // purposefully not C++03
                      raw_storage_iterator&>           // purposefully not C++03
{
public:
    explicit raw_storage_iterator(OutputIterator x);
    raw_storage_iterator& operator*();
    raw_storage_iterator& operator=(const T& element);
    raw_storage_iterator& operator++();
    raw_storage_iterator  operator++(int);
};

template <class T> pair<T*,ptrdiff_t> get_temporary_buffer(ptrdiff_t n) noexcept;
template <class T> void               return_temporary_buffer(T* p) noexcept;

template <class T> T* addressof(T& r) noexcept;

template <class InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result);

template <class InputIterator, class Size, class ForwardIterator>
ForwardIterator
uninitialized_copy_n(InputIterator first, Size n, ForwardIterator result);

template <class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x);

template <class ForwardIterator, class Size, class T>
ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x);

template <class Y> struct auto_ptr_ref {};

template<class X>
class auto_ptr
{
public:
    typedef X element_type;

    explicit auto_ptr(X* p =0) throw();
    auto_ptr(auto_ptr&) throw();
    template<class Y> auto_ptr(auto_ptr<Y>&) throw();
    auto_ptr& operator=(auto_ptr&) throw();
    template<class Y> auto_ptr& operator=(auto_ptr<Y>&) throw();
    auto_ptr& operator=(auto_ptr_ref<X> r) throw();
    ~auto_ptr() throw();

    typename add_lvalue_reference<X>::type operator*() const throw();
    X* operator->() const throw();
    X* get() const throw();
    X* release() throw();
    void reset(X* p =0) throw();

    auto_ptr(auto_ptr_ref<X>) throw();
    template<class Y> operator auto_ptr_ref<Y>() throw();
    template<class Y> operator auto_ptr<Y>() throw();
};

template <class T>
struct default_delete
{
    constexpr default_delete() noexcept = default;
    template <class U> default_delete(const default_delete<U>&) noexcept;

    void operator()(T*) const noexcept;
};

template <class T>
struct default_delete<T[]>
{
    constexpr default_delete() noexcept = default;
    void operator()(T*) const noexcept;
    template <class U> void operator()(U*) const = delete;
};

template <class T, class D = default_delete<T>>
class unique_ptr
{
public:
    typedef see below pointer;
    typedef T element_type;
    typedef D deleter_type;

    // constructors
    constexpr unique_ptr() noexcept;
    explicit unique_ptr(pointer p) noexcept;
    unique_ptr(pointer p, see below d1) noexcept;
    unique_ptr(pointer p, see below d2) noexcept;
    unique_ptr(unique_ptr&& u) noexcept;
    unique_ptr(nullptr_t) noexcept : unique_ptr() { }
    template <class U, class E>
        unique_ptr(unique_ptr<U, E>&& u) noexcept;
    template <class U>
        unique_ptr(auto_ptr<U>&& u) noexcept;

    // destructor
    ~unique_ptr();

    // assignment
    unique_ptr& operator=(unique_ptr&& u) noexcept;
    template <class U, class E> unique_ptr& operator=(unique_ptr<U, E>&& u) noexcept;
    unique_ptr& operator=(nullptr_t) noexcept;

    // observers
    typename add_lvalue_reference<T>::type operator*() const;
    pointer operator->() const noexcept;
    pointer get() const noexcept;
    deleter_type& get_deleter() noexcept;
    const deleter_type& get_deleter() const noexcept;
    explicit operator bool() const noexcept;

    // modifiers
    pointer release() noexcept;
    void reset(pointer p = pointer()) noexcept;
    void swap(unique_ptr& u) noexcept;
};

template <class T, class D>
class unique_ptr<T[], D>
{
public:
    typedef implementation-defined pointer;
    typedef T element_type;
    typedef D deleter_type;

    // constructors
    constexpr unique_ptr() noexcept;
    explicit unique_ptr(pointer p) noexcept;
    unique_ptr(pointer p, see below d) noexcept;
    unique_ptr(pointer p, see below d) noexcept;
    unique_ptr(unique_ptr&& u) noexcept;
    unique_ptr(nullptr_t) noexcept : unique_ptr() { }

    // destructor
    ~unique_ptr();

    // assignment
    unique_ptr& operator=(unique_ptr&& u) noexcept;
    unique_ptr& operator=(nullptr_t) noexcept;

    // observers
    T& operator[](size_t i) const;
    pointer get() const noexcept;
    deleter_type& get_deleter() noexcept;
    const deleter_type& get_deleter() const noexcept;
    explicit operator bool() const noexcept;

    // modifiers
    pointer release() noexcept;
    void reset(pointer p = pointer()) noexcept;
    void reset(nullptr_t) noexcept;
    template <class U> void reset(U) = delete;
    void swap(unique_ptr& u) noexcept;
};

template <class T, class D>
    void swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y) noexcept;

template <class T1, class D1, class T2, class D2>
    bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <class T1, class D1, class T2, class D2>
    bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <class T1, class D1, class T2, class D2>
    bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <class T1, class D1, class T2, class D2>
    bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <class T1, class D1, class T2, class D2>
    bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <class T1, class D1, class T2, class D2>
    bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

template <class T, class D>
    bool operator==(const unique_ptr<T, D>& x, nullptr_t) noexcept;
template <class T, class D>
    bool operator==(nullptr_t, const unique_ptr<T, D>& y) noexcept;
template <class T, class D>
    bool operator!=(const unique_ptr<T, D>& x, nullptr_t) noexcept;
template <class T, class D>
    bool operator!=(nullptr_t, const unique_ptr<T, D>& y) noexcept;

template <class T, class D>
    bool operator<(const unique_ptr<T, D>& x, nullptr_t);
template <class T, class D>
    bool operator<(nullptr_t, const unique_ptr<T, D>& y);
template <class T, class D>
    bool operator<=(const unique_ptr<T, D>& x, nullptr_t);
template <class T, class D>
    bool operator<=(nullptr_t, const unique_ptr<T, D>& y);
template <class T, class D>
    bool operator>(const unique_ptr<T, D>& x, nullptr_t);
template <class T, class D>
    bool operator>(nullptr_t, const unique_ptr<T, D>& y);
template <class T, class D>
    bool operator>=(const unique_ptr<T, D>& x, nullptr_t);
template <class T, class D>
    bool operator>=(nullptr_t, const unique_ptr<T, D>& y);

class bad_weak_ptr
    : public std::exception
{
    bad_weak_ptr() noexcept;
};

template<class T, class... Args> unique_ptr<T> make_unique(Args&&... args);     // C++14
template<class T>                unique_ptr<T> make_unique(size_t n);           // C++14
template<class T, class... Args> unspecified   make_unique(Args&&...) = delete; // C++14, T == U[N]

template<class T>
class shared_ptr
{
public:
    typedef T element_type;

    // constructors:
    constexpr shared_ptr() noexcept;
    template<class Y> explicit shared_ptr(Y* p);
    template<class Y, class D> shared_ptr(Y* p, D d);
    template<class Y, class D, class A> shared_ptr(Y* p, D d, A a);
    template <class D> shared_ptr(nullptr_t p, D d);
    template <class D, class A> shared_ptr(nullptr_t p, D d, A a);
    template<class Y> shared_ptr(const shared_ptr<Y>& r, T *p) noexcept;
    shared_ptr(const shared_ptr& r) noexcept;
    template<class Y> shared_ptr(const shared_ptr<Y>& r) noexcept;
    shared_ptr(shared_ptr&& r) noexcept;
    template<class Y> shared_ptr(shared_ptr<Y>&& r) noexcept;
    template<class Y> explicit shared_ptr(const weak_ptr<Y>& r);
    template<class Y> shared_ptr(auto_ptr<Y>&& r);
    template <class Y, class D> shared_ptr(unique_ptr<Y, D>&& r);
    shared_ptr(nullptr_t) : shared_ptr() { }

    // destructor:
    ~shared_ptr();

    // assignment:
    shared_ptr& operator=(const shared_ptr& r) noexcept;
    template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r) noexcept;
    shared_ptr& operator=(shared_ptr&& r) noexcept;
    template<class Y> shared_ptr& operator=(shared_ptr<Y>&& r);
    template<class Y> shared_ptr& operator=(auto_ptr<Y>&& r);
    template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

    // modifiers:
    void swap(shared_ptr& r) noexcept;
    void reset() noexcept;
    template<class Y> void reset(Y* p);
    template<class Y, class D> void reset(Y* p, D d);
    template<class Y, class D, class A> void reset(Y* p, D d, A a);

    // observers:
    T* get() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    long use_count() const noexcept;
    bool unique() const noexcept;
    explicit operator bool() const noexcept;
    template<class U> bool owner_before(shared_ptr<U> const& b) const;
    template<class U> bool owner_before(weak_ptr<U> const& b) const;
};

// shared_ptr comparisons:
template<class T, class U>
    bool operator==(shared_ptr<T> const& a, shared_ptr<U> const& b) noexcept;
template<class T, class U>
    bool operator!=(shared_ptr<T> const& a, shared_ptr<U> const& b) noexcept;
template<class T, class U>
    bool operator<(shared_ptr<T> const& a, shared_ptr<U> const& b) noexcept;
template<class T, class U>
    bool operator>(shared_ptr<T> const& a, shared_ptr<U> const& b) noexcept;
template<class T, class U>
    bool operator<=(shared_ptr<T> const& a, shared_ptr<U> const& b) noexcept;
template<class T, class U>
    bool operator>=(shared_ptr<T> const& a, shared_ptr<U> const& b) noexcept;

template <class T>
    bool operator==(const shared_ptr<T>& x, nullptr_t) noexcept;
template <class T>
    bool operator==(nullptr_t, const shared_ptr<T>& y) noexcept;
template <class T>
    bool operator!=(const shared_ptr<T>& x, nullptr_t) noexcept;
template <class T>
    bool operator!=(nullptr_t, const shared_ptr<T>& y) noexcept;
template <class T>
    bool operator<(const shared_ptr<T>& x, nullptr_t) noexcept;
template <class T>
bool operator<(nullptr_t, const shared_ptr<T>& y) noexcept;
template <class T>
    bool operator<=(const shared_ptr<T>& x, nullptr_t) noexcept;
template <class T>
    bool operator<=(nullptr_t, const shared_ptr<T>& y) noexcept;
template <class T>
    bool operator>(const shared_ptr<T>& x, nullptr_t) noexcept;
template <class T>
    bool operator>(nullptr_t, const shared_ptr<T>& y) noexcept;
template <class T>
    bool operator>=(const shared_ptr<T>& x, nullptr_t) noexcept;
template <class T>
    bool operator>=(nullptr_t, const shared_ptr<T>& y) noexcept;

// shared_ptr specialized algorithms:
template<class T> void swap(shared_ptr<T>& a, shared_ptr<T>& b) noexcept;

// shared_ptr casts:
template<class T, class U>
    shared_ptr<T> static_pointer_cast(shared_ptr<U> const& r) noexcept;
template<class T, class U>
    shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const& r) noexcept;
template<class T, class U>
    shared_ptr<T> const_pointer_cast(shared_ptr<U> const& r) noexcept;

// shared_ptr I/O:
template<class E, class T, class Y>
    basic_ostream<E, T>& operator<< (basic_ostream<E, T>& os, shared_ptr<Y> const& p);

// shared_ptr get_deleter:
template<class D, class T> D* get_deleter(shared_ptr<T> const& p) noexcept;

template<class T, class... Args>
    shared_ptr<T> make_shared(Args&&... args);
template<class T, class A, class... Args>
    shared_ptr<T> allocate_shared(const A& a, Args&&... args);

template<class T>
class weak_ptr
{
public:
    typedef T element_type;

    // constructors
    constexpr weak_ptr() noexcept;
    template<class Y> weak_ptr(shared_ptr<Y> const& r) noexcept;
    weak_ptr(weak_ptr const& r) noexcept;
    template<class Y> weak_ptr(weak_ptr<Y> const& r) noexcept;
    weak_ptr(weak_ptr&& r) noexcept;                      // C++14
    template<class Y> weak_ptr(weak_ptr<Y>&& r) noexcept; // C++14

    // destructor
    ~weak_ptr();

    // assignment
    weak_ptr& operator=(weak_ptr const& r) noexcept;
    template<class Y> weak_ptr& operator=(weak_ptr<Y> const& r) noexcept;
    template<class Y> weak_ptr& operator=(shared_ptr<Y> const& r) noexcept;
    weak_ptr& operator=(weak_ptr&& r) noexcept;                      // C++14
    template<class Y> weak_ptr& operator=(weak_ptr<Y>&& r) noexcept; // C++14

    // modifiers
    void swap(weak_ptr& r) noexcept;
    void reset() noexcept;

    // observers
    long use_count() const noexcept;
    bool expired() const noexcept;
    shared_ptr<T> lock() const noexcept;
    template<class U> bool owner_before(shared_ptr<U> const& b) const;
    template<class U> bool owner_before(weak_ptr<U> const& b) const;
};

// weak_ptr specialized algorithms:
template<class T> void swap(weak_ptr<T>& a, weak_ptr<T>& b) noexcept;

// class owner_less:
template<class T> struct owner_less;

template<class T>
struct owner_less<shared_ptr<T>>
    : binary_function<shared_ptr<T>, shared_ptr<T>, bool>
{
    typedef bool result_type;
    bool operator()(shared_ptr<T> const&, shared_ptr<T> const&) const;
    bool operator()(shared_ptr<T> const&, weak_ptr<T> const&) const;
    bool operator()(weak_ptr<T> const&, shared_ptr<T> const&) const;
};

template<class T>
struct owner_less<weak_ptr<T>>
    : binary_function<weak_ptr<T>, weak_ptr<T>, bool>
{
    typedef bool result_type;
    bool operator()(weak_ptr<T> const&, weak_ptr<T> const&) const;
    bool operator()(shared_ptr<T> const&, weak_ptr<T> const&) const;
    bool operator()(weak_ptr<T> const&, shared_ptr<T> const&) const;
};

template<class T>
class enable_shared_from_this
{
protected:
    constexpr enable_shared_from_this() noexcept;
    enable_shared_from_this(enable_shared_from_this const&) noexcept;
    enable_shared_from_this& operator=(enable_shared_from_this const&) noexcept;
    ~enable_shared_from_this();
public:
    shared_ptr<T> shared_from_this();
    shared_ptr<T const> shared_from_this() const;
};

template<class T>
    bool atomic_is_lock_free(const shared_ptr<T>* p);
template<class T>
    shared_ptr<T> atomic_load(const shared_ptr<T>* p);
template<class T>
    shared_ptr<T> atomic_load_explicit(const shared_ptr<T>* p, memory_order mo);
template<class T>
    void atomic_store(shared_ptr<T>* p, shared_ptr<T> r);
template<class T>
    void atomic_store_explicit(shared_ptr<T>* p, shared_ptr<T> r, memory_order mo);
template<class T>
    shared_ptr<T> atomic_exchange(shared_ptr<T>* p, shared_ptr<T> r);
template<class T>
    shared_ptr<T>
    atomic_exchange_explicit(shared_ptr<T>* p, shared_ptr<T> r, memory_order mo);
template<class T>
    bool
    atomic_compare_exchange_weak(shared_ptr<T>* p, shared_ptr<T>* v, shared_ptr<T> w);
template<class T>
    bool
    atomic_compare_exchange_strong( shared_ptr<T>* p, shared_ptr<T>* v, shared_ptr<T> w);
template<class T>
    bool
    atomic_compare_exchange_weak_explicit(shared_ptr<T>* p, shared_ptr<T>* v,
                                          shared_ptr<T> w, memory_order success,
                                          memory_order failure);
template<class T>
    bool
    atomic_compare_exchange_strong_explicit(shared_ptr<T>* p, shared_ptr<T>* v,
                                            shared_ptr<T> w, memory_order success,
                                            memory_order failure);
// Hash support
template <class T> struct hash;
template <class T, class D> struct hash<unique_ptr<T, D> >;
template <class T> struct hash<shared_ptr<T> >;

// Pointer safety
enum class pointer_safety { relaxed, preferred, strict };
void declare_reachable(void *p);
template <class T> T *undeclare_reachable(T *p);
void declare_no_pointers(char *p, size_t n);
void undeclare_no_pointers(char *p, size_t n);
pointer_safety get_pointer_safety() noexcept;

void* align(size_t alignment, size_t size, void*& ptr, size_t& space);

}  // std

*/

#include <__config>
#include <type_traits>
#include <typeinfo>
#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>
#include <limits>
#include <iterator>
#include <__functional_base>
#include <iosfwd>
#include <tuple>
#include <stdexcept>
#include <cstring>
#if defined(_LIBCPP_NO_EXCEPTIONS)
    #include <cassert>
#endif

#if !defined(_LIBCPP_HAS_NO_ATOMIC_HEADER)
#  include <atomic>
#endif

#include <__undef_min_max>
#include <__undef___deallocate>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _ValueType>
inline _LIBCPP_ALWAYS_INLINE
_ValueType __libcpp_relaxed_load(_ValueType const* __value) {
#if !defined(_LIBCPP_HAS_NO_THREADS) && \
    defined(__ATOMIC_RELAXED) &&        \
    (__has_builtin(__atomic_load_n) || _GNUC_VER >= 407)
    return __atomic_load_n(__value, __ATOMIC_RELAXED);
#else
    return *__value;
#endif
}

// addressof moved to <__functional_base>

template <class _Tp> class allocator;

template <>
class _LIBCPP_TYPE_VIS_ONLY allocator<void>
{
public:
    typedef void*             pointer;
    typedef const void*       const_pointer;
    typedef void              value_type;

    template <class _Up> struct rebind {typedef allocator<_Up> other;};
};

template <>
class _LIBCPP_TYPE_VIS_ONLY allocator<const void>
{
public:
    typedef const void*       pointer;
    typedef const void*       const_pointer;
    typedef const void        value_type;

    template <class _Up> struct rebind {typedef allocator<_Up> other;};
};

// pointer_traits

template <class _Tp>
struct __has_element_type
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::element_type* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Ptr, bool = __has_element_type<_Ptr>::value>
struct __pointer_traits_element_type;

template <class _Ptr>
struct __pointer_traits_element_type<_Ptr, true>
{
    typedef typename _Ptr::element_type type;
};

#ifndef _LIBCPP_HAS_NO_VARIADICS

template <template <class, class...> class _Sp, class _Tp, class ..._Args>
struct __pointer_traits_element_type<_Sp<_Tp, _Args...>, true>
{
    typedef typename _Sp<_Tp, _Args...>::element_type type;
};

template <template <class, class...> class _Sp, class _Tp, class ..._Args>
struct __pointer_traits_element_type<_Sp<_Tp, _Args...>, false>
{
    typedef _Tp type;
};

#else  // _LIBCPP_HAS_NO_VARIADICS

template <template <class> class _Sp, class _Tp>
struct __pointer_traits_element_type<_Sp<_Tp>, true>
{
    typedef typename _Sp<_Tp>::element_type type;
};

template <template <class> class _Sp, class _Tp>
struct __pointer_traits_element_type<_Sp<_Tp>, false>
{
    typedef _Tp type;
};

template <template <class, class> class _Sp, class _Tp, class _A0>
struct __pointer_traits_element_type<_Sp<_Tp, _A0>, true>
{
    typedef typename _Sp<_Tp, _A0>::element_type type;
};

template <template <class, class> class _Sp, class _Tp, class _A0>
struct __pointer_traits_element_type<_Sp<_Tp, _A0>, false>
{
    typedef _Tp type;
};

template <template <class, class, class> class _Sp, class _Tp, class _A0, class _A1>
struct __pointer_traits_element_type<_Sp<_Tp, _A0, _A1>, true>
{
    typedef typename _Sp<_Tp, _A0, _A1>::element_type type;
};

template <template <class, class, class> class _Sp, class _Tp, class _A0, class _A1>
struct __pointer_traits_element_type<_Sp<_Tp, _A0, _A1>, false>
{
    typedef _Tp type;
};

template <template <class, class, class, class> class _Sp, class _Tp, class _A0,
                                                           class _A1, class _A2>
struct __pointer_traits_element_type<_Sp<_Tp, _A0, _A1, _A2>, true>
{
    typedef typename _Sp<_Tp, _A0, _A1, _A2>::element_type type;
};

template <template <class, class, class, class> class _Sp, class _Tp, class _A0,
                                                           class _A1, class _A2>
struct __pointer_traits_element_type<_Sp<_Tp, _A0, _A1, _A2>, false>
{
    typedef _Tp type;
};

#endif  // _LIBCPP_HAS_NO_VARIADICS

template <class _Tp>
struct __has_difference_type
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::difference_type* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Ptr, bool = __has_difference_type<_Ptr>::value>
struct __pointer_traits_difference_type
{
    typedef ptrdiff_t type;
};

template <class _Ptr>
struct __pointer_traits_difference_type<_Ptr, true>
{
    typedef typename _Ptr::difference_type type;
};

template <class _Tp, class _Up>
struct __has_rebind
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Xp> static __two __test(...);
    template <class _Xp> static char __test(typename _Xp::template rebind<_Up>* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Tp, class _Up, bool = __has_rebind<_Tp, _Up>::value>
struct __pointer_traits_rebind
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename _Tp::template rebind<_Up> type;
#else
    typedef typename _Tp::template rebind<_Up>::other type;
#endif
};

#ifndef _LIBCPP_HAS_NO_VARIADICS

template <template <class, class...> class _Sp, class _Tp, class ..._Args, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp, _Args...>, _Up, true>
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename _Sp<_Tp, _Args...>::template rebind<_Up> type;
#else
    typedef typename _Sp<_Tp, _Args...>::template rebind<_Up>::other type;
#endif
};

template <template <class, class...> class _Sp, class _Tp, class ..._Args, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp, _Args...>, _Up, false>
{
    typedef _Sp<_Up, _Args...> type;
};

#else  // _LIBCPP_HAS_NO_VARIADICS

template <template <class> class _Sp, class _Tp, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp>, _Up, true>
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename _Sp<_Tp>::template rebind<_Up> type;
#else
    typedef typename _Sp<_Tp>::template rebind<_Up>::other type;
#endif
};

template <template <class> class _Sp, class _Tp, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp>, _Up, false>
{
    typedef _Sp<_Up> type;
};

template <template <class, class> class _Sp, class _Tp, class _A0, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp, _A0>, _Up, true>
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename _Sp<_Tp, _A0>::template rebind<_Up> type;
#else
    typedef typename _Sp<_Tp, _A0>::template rebind<_Up>::other type;
#endif
};

template <template <class, class> class _Sp, class _Tp, class _A0, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp, _A0>, _Up, false>
{
    typedef _Sp<_Up, _A0> type;
};

template <template <class, class, class> class _Sp, class _Tp, class _A0,
                                         class _A1, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp, _A0, _A1>, _Up, true>
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename _Sp<_Tp, _A0, _A1>::template rebind<_Up> type;
#else
    typedef typename _Sp<_Tp, _A0, _A1>::template rebind<_Up>::other type;
#endif
};

template <template <class, class, class> class _Sp, class _Tp, class _A0,
                                         class _A1, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp, _A0, _A1>, _Up, false>
{
    typedef _Sp<_Up, _A0, _A1> type;
};

template <template <class, class, class, class> class _Sp, class _Tp, class _A0,
                                                class _A1, class _A2, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp, _A0, _A1, _A2>, _Up, true>
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename _Sp<_Tp, _A0, _A1, _A2>::template rebind<_Up> type;
#else
    typedef typename _Sp<_Tp, _A0, _A1, _A2>::template rebind<_Up>::other type;
#endif
};

template <template <class, class, class, class> class _Sp, class _Tp, class _A0,
                                                class _A1, class _A2, class _Up>
struct __pointer_traits_rebind<_Sp<_Tp, _A0, _A1, _A2>, _Up, false>
{
    typedef _Sp<_Up, _A0, _A1, _A2> type;
};

#endif  // _LIBCPP_HAS_NO_VARIADICS

template <class _Ptr>
struct _LIBCPP_TYPE_VIS_ONLY pointer_traits
{
    typedef _Ptr                                                     pointer;
    typedef typename __pointer_traits_element_type<pointer>::type    element_type;
    typedef typename __pointer_traits_difference_type<pointer>::type difference_type;

#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    template <class _Up> using rebind = typename __pointer_traits_rebind<pointer, _Up>::type;
#else
    template <class _Up> struct rebind
        {typedef typename __pointer_traits_rebind<pointer, _Up>::type other;};
#endif  // _LIBCPP_HAS_NO_TEMPLATE_ALIASES

private:
    struct __nat {};
public:
    _LIBCPP_INLINE_VISIBILITY
    static pointer pointer_to(typename conditional<is_void<element_type>::value,
                                           __nat, element_type>::type& __r)
        {return pointer::pointer_to(__r);}
};

template <class _Tp>
struct _LIBCPP_TYPE_VIS_ONLY pointer_traits<_Tp*>
{
    typedef _Tp*      pointer;
    typedef _Tp       element_type;
    typedef ptrdiff_t difference_type;

#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    template <class _Up> using rebind = _Up*;
#else
    template <class _Up> struct rebind {typedef _Up* other;};
#endif

private:
    struct __nat {};
public:
    _LIBCPP_INLINE_VISIBILITY
    static pointer pointer_to(typename conditional<is_void<element_type>::value,
                                      __nat, element_type>::type& __r) _NOEXCEPT
        {return _VSTD::addressof(__r);}
};

template <class _From, class _To>
struct __rebind_pointer {
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename pointer_traits<_From>::template rebind<_To>        type;
#else
    typedef typename pointer_traits<_From>::template rebind<_To>::other type;
#endif
};

// allocator_traits

namespace __has_pointer_type_imp
{
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::pointer* = 0);
}

template <class _Tp>
struct __has_pointer_type
    : public integral_constant<bool, sizeof(__has_pointer_type_imp::__test<_Tp>(0)) == 1>
{
};

namespace __pointer_type_imp
{

template <class _Tp, class _Dp, bool = __has_pointer_type<_Dp>::value>
struct __pointer_type
{
    typedef typename _Dp::pointer type;
};

template <class _Tp, class _Dp>
struct __pointer_type<_Tp, _Dp, false>
{
    typedef _Tp* type;
};

}  // __pointer_type_imp

template <class _Tp, class _Dp>
struct __pointer_type
{
    typedef typename __pointer_type_imp::__pointer_type<_Tp, typename remove_reference<_Dp>::type>::type type;
};

template <class _Tp>
struct __has_const_pointer
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::const_pointer* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Tp, class _Ptr, class _Alloc, bool = __has_const_pointer<_Alloc>::value>
struct __const_pointer
{
    typedef typename _Alloc::const_pointer type;
};

template <class _Tp, class _Ptr, class _Alloc>
struct __const_pointer<_Tp, _Ptr, _Alloc, false>
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename pointer_traits<_Ptr>::template rebind<const _Tp> type;
#else
    typedef typename pointer_traits<_Ptr>::template rebind<const _Tp>::other type;
#endif
};

template <class _Tp>
struct __has_void_pointer
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::void_pointer* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Ptr, class _Alloc, bool = __has_void_pointer<_Alloc>::value>
struct __void_pointer
{
    typedef typename _Alloc::void_pointer type;
};

template <class _Ptr, class _Alloc>
struct __void_pointer<_Ptr, _Alloc, false>
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename pointer_traits<_Ptr>::template rebind<void> type;
#else
    typedef typename pointer_traits<_Ptr>::template rebind<void>::other type;
#endif
};

template <class _Tp>
struct __has_const_void_pointer
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::const_void_pointer* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Ptr, class _Alloc, bool = __has_const_void_pointer<_Alloc>::value>
struct __const_void_pointer
{
    typedef typename _Alloc::const_void_pointer type;
};

template <class _Ptr, class _Alloc>
struct __const_void_pointer<_Ptr, _Alloc, false>
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename pointer_traits<_Ptr>::template rebind<const void> type;
#else
    typedef typename pointer_traits<_Ptr>::template rebind<const void>::other type;
#endif
};

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
__to_raw_pointer(_Tp* __p) _NOEXCEPT
{
    return __p;
}

template <class _Pointer>
inline _LIBCPP_INLINE_VISIBILITY
typename pointer_traits<_Pointer>::element_type*
__to_raw_pointer(_Pointer __p) _NOEXCEPT
{
    return _VSTD::__to_raw_pointer(__p.operator->());
}

template <class _Tp>
struct __has_size_type
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::size_type* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Alloc, class _DiffType, bool = __has_size_type<_Alloc>::value>
struct __size_type
{
    typedef typename make_unsigned<_DiffType>::type type;
};

template <class _Alloc, class _DiffType>
struct __size_type<_Alloc, _DiffType, true>
{
    typedef typename _Alloc::size_type type;
};

template <class _Tp>
struct __has_propagate_on_container_copy_assignment
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::propagate_on_container_copy_assignment* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Alloc, bool = __has_propagate_on_container_copy_assignment<_Alloc>::value>
struct __propagate_on_container_copy_assignment
{
    typedef false_type type;
};

template <class _Alloc>
struct __propagate_on_container_copy_assignment<_Alloc, true>
{
    typedef typename _Alloc::propagate_on_container_copy_assignment type;
};

template <class _Tp>
struct __has_propagate_on_container_move_assignment
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::propagate_on_container_move_assignment* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Alloc, bool = __has_propagate_on_container_move_assignment<_Alloc>::value>
struct __propagate_on_container_move_assignment
{
    typedef false_type type;
};

template <class _Alloc>
struct __propagate_on_container_move_assignment<_Alloc, true>
{
    typedef typename _Alloc::propagate_on_container_move_assignment type;
};

template <class _Tp>
struct __has_propagate_on_container_swap
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::propagate_on_container_swap* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Alloc, bool = __has_propagate_on_container_swap<_Alloc>::value>
struct __propagate_on_container_swap
{
    typedef false_type type;
};

template <class _Alloc>
struct __propagate_on_container_swap<_Alloc, true>
{
    typedef typename _Alloc::propagate_on_container_swap type;
};

template <class _Tp>
struct __has_is_always_equal
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::is_always_equal* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Alloc, bool = __has_is_always_equal<_Alloc>::value>
struct __is_always_equal
{
    typedef typename _VSTD::is_empty<_Alloc>::type type;
};

template <class _Alloc>
struct __is_always_equal<_Alloc, true>
{
    typedef typename _Alloc::is_always_equal type;
};

template <class _Tp, class _Up, bool = __has_rebind<_Tp, _Up>::value>
struct __has_rebind_other
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Xp> static __two __test(...);
    template <class _Xp> static char __test(typename _Xp::template rebind<_Up>::other* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Tp, class _Up>
struct __has_rebind_other<_Tp, _Up, false>
{
    static const bool value = false;
};

template <class _Tp, class _Up, bool = __has_rebind_other<_Tp, _Up>::value>
struct __allocator_traits_rebind
{
    typedef typename _Tp::template rebind<_Up>::other type;
};

#ifndef _LIBCPP_HAS_NO_VARIADICS

template <template <class, class...> class _Alloc, class _Tp, class ..._Args, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp, _Args...>, _Up, true>
{
    typedef typename _Alloc<_Tp, _Args...>::template rebind<_Up>::other type;
};

template <template <class, class...> class _Alloc, class _Tp, class ..._Args, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp, _Args...>, _Up, false>
{
    typedef _Alloc<_Up, _Args...> type;
};

#else  // _LIBCPP_HAS_NO_VARIADICS

template <template <class> class _Alloc, class _Tp, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp>, _Up, true>
{
    typedef typename _Alloc<_Tp>::template rebind<_Up>::other type;
};

template <template <class> class _Alloc, class _Tp, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp>, _Up, false>
{
    typedef _Alloc<_Up> type;
};

template <template <class, class> class _Alloc, class _Tp, class _A0, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp, _A0>, _Up, true>
{
    typedef typename _Alloc<_Tp, _A0>::template rebind<_Up>::other type;
};

template <template <class, class> class _Alloc, class _Tp, class _A0, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp, _A0>, _Up, false>
{
    typedef _Alloc<_Up, _A0> type;
};

template <template <class, class, class> class _Alloc, class _Tp, class _A0,
                                         class _A1, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp, _A0, _A1>, _Up, true>
{
    typedef typename _Alloc<_Tp, _A0, _A1>::template rebind<_Up>::other type;
};

template <template <class, class, class> class _Alloc, class _Tp, class _A0,
                                         class _A1, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp, _A0, _A1>, _Up, false>
{
    typedef _Alloc<_Up, _A0, _A1> type;
};

template <template <class, class, class, class> class _Alloc, class _Tp, class _A0,
                                                class _A1, class _A2, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp, _A0, _A1, _A2>, _Up, true>
{
    typedef typename _Alloc<_Tp, _A0, _A1, _A2>::template rebind<_Up>::other type;
};

template <template <class, class, class, class> class _Alloc, class _Tp, class _A0,
                                                class _A1, class _A2, class _Up>
struct __allocator_traits_rebind<_Alloc<_Tp, _A0, _A1, _A2>, _Up, false>
{
    typedef _Alloc<_Up, _A0, _A1, _A2> type;
};

#endif  // _LIBCPP_HAS_NO_VARIADICS

#ifndef _LIBCPP_HAS_NO_ADVANCED_SFINAE

template <class _Alloc, class _SizeType, class _ConstVoidPtr>
auto
__has_allocate_hint_test(_Alloc&& __a, _SizeType&& __sz, _ConstVoidPtr&& __p)
    -> decltype(__a.allocate(__sz, __p), true_type());

template <class _Alloc, class _SizeType, class _ConstVoidPtr>
auto
__has_allocate_hint_test(const _Alloc& __a, _SizeType&& __sz, _ConstVoidPtr&& __p)
    -> false_type;

template <class _Alloc, class _SizeType, class _ConstVoidPtr>
struct __has_allocate_hint
    : integral_constant<bool,
        is_same<
            decltype(__has_allocate_hint_test(declval<_Alloc>(),
                                          declval<_SizeType>(),
                                          declval<_ConstVoidPtr>())),
            true_type>::value>
{
};

#else  // _LIBCPP_HAS_NO_ADVANCED_SFINAE

template <class _Alloc, class _SizeType, class _ConstVoidPtr>
struct __has_allocate_hint
    : true_type
{
};

#endif  // _LIBCPP_HAS_NO_ADVANCED_SFINAE

#if !defined(_LIBCPP_HAS_NO_ADVANCED_SFINAE) && !defined(_LIBCPP_HAS_NO_VARIADICS)

template <class _Alloc, class _Tp, class ..._Args>
decltype(_VSTD::declval<_Alloc>().construct(_VSTD::declval<_Tp*>(),
                                           _VSTD::declval<_Args>()...),
                                           true_type())
__has_construct_test(_Alloc&& __a, _Tp* __p, _Args&& ...__args);

template <class _Alloc, class _Pointer, class ..._Args>
false_type
__has_construct_test(const _Alloc& __a, _Pointer&& __p, _Args&& ...__args);

template <class _Alloc, class _Pointer, class ..._Args>
struct __has_construct
    : integral_constant<bool,
        is_same<
            decltype(__has_construct_test(declval<_Alloc>(),
                                          declval<_Pointer>(),
                                          declval<_Args>()...)),
            true_type>::value>
{
};

template <class _Alloc, class _Pointer>
auto
__has_destroy_test(_Alloc&& __a, _Pointer&& __p)
    -> decltype(__a.destroy(__p), true_type());

template <class _Alloc, class _Pointer>
auto
__has_destroy_test(const _Alloc& __a, _Pointer&& __p)
    -> false_type;

template <class _Alloc, class _Pointer>
struct __has_destroy
    : integral_constant<bool,
        is_same<
            decltype(__has_destroy_test(declval<_Alloc>(),
                                        declval<_Pointer>())),
            true_type>::value>
{
};

template <class _Alloc>
auto
__has_max_size_test(_Alloc&& __a)
    -> decltype(__a.max_size(), true_type());

template <class _Alloc>
auto
__has_max_size_test(const volatile _Alloc& __a)
    -> false_type;

template <class _Alloc>
struct __has_max_size
    : integral_constant<bool,
        is_same<
            decltype(__has_max_size_test(declval<_Alloc&>())),
            true_type>::value>
{
};

template <class _Alloc>
auto
__has_select_on_container_copy_construction_test(_Alloc&& __a)
    -> decltype(__a.select_on_container_copy_construction(), true_type());

template <class _Alloc>
auto
__has_select_on_container_copy_construction_test(const volatile _Alloc& __a)
    -> false_type;

template <class _Alloc>
struct __has_select_on_container_copy_construction
    : integral_constant<bool,
        is_same<
            decltype(__has_select_on_container_copy_construction_test(declval<_Alloc&>())),
            true_type>::value>
{
};

#else  // _LIBCPP_HAS_NO_ADVANCED_SFINAE

#ifndef _LIBCPP_HAS_NO_VARIADICS

template <class _Alloc, class _Pointer, class ..._Args>
struct __has_construct
    : false_type
{
};

#else  // _LIBCPP_HAS_NO_VARIADICS

template <class _Alloc, class _Pointer, class _Args>
struct __has_construct
    : false_type
{
};

#endif  // _LIBCPP_HAS_NO_VARIADICS

template <class _Alloc, class _Pointer>
struct __has_destroy
    : false_type
{
};

template <class _Alloc>
struct __has_max_size
    : true_type
{
};

template <class _Alloc>
struct __has_select_on_container_copy_construction
    : false_type
{
};

#endif  // _LIBCPP_HAS_NO_ADVANCED_SFINAE

template <class _Alloc, class _Ptr, bool = __has_difference_type<_Alloc>::value>
struct __alloc_traits_difference_type
{
    typedef typename pointer_traits<_Ptr>::difference_type type;
};

template <class _Alloc, class _Ptr>
struct __alloc_traits_difference_type<_Alloc, _Ptr, true>
{
    typedef typename _Alloc::difference_type type;
};

template <class _Alloc>
struct _LIBCPP_TYPE_VIS_ONLY allocator_traits
{
    typedef _Alloc                              allocator_type;
    typedef typename allocator_type::value_type value_type;

    typedef typename __pointer_type<value_type, allocator_type>::type pointer;
    typedef typename __const_pointer<value_type, pointer, allocator_type>::type const_pointer;
    typedef typename __void_pointer<pointer, allocator_type>::type void_pointer;
    typedef typename __const_void_pointer<pointer, allocator_type>::type const_void_pointer;

    typedef typename __alloc_traits_difference_type<allocator_type, pointer>::type difference_type;
    typedef typename __size_type<allocator_type, difference_type>::type size_type;

    typedef typename __propagate_on_container_copy_assignment<allocator_type>::type
                     propagate_on_container_copy_assignment;
    typedef typename __propagate_on_container_move_assignment<allocator_type>::type
                     propagate_on_container_move_assignment;
    typedef typename __propagate_on_container_swap<allocator_type>::type
                     propagate_on_container_swap;
    typedef typename __is_always_equal<allocator_type>::type
                     is_always_equal;

#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    template <class _Tp> using rebind_alloc =
                  typename __allocator_traits_rebind<allocator_type, _Tp>::type;
    template <class _Tp> using rebind_traits = allocator_traits<rebind_alloc<_Tp>>;
#else  // _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    template <class _Tp> struct rebind_alloc
        {typedef typename __allocator_traits_rebind<allocator_type, _Tp>::type other;};
    template <class _Tp> struct rebind_traits
        {typedef allocator_traits<typename rebind_alloc<_Tp>::other> other;};
#endif  // _LIBCPP_HAS_NO_TEMPLATE_ALIASES

    _LIBCPP_INLINE_VISIBILITY
    static pointer allocate(allocator_type& __a, size_type __n)
        {return __a.allocate(__n);}
    _LIBCPP_INLINE_VISIBILITY
    static pointer allocate(allocator_type& __a, size_type __n, const_void_pointer __hint)
        {return allocate(__a, __n, __hint,
            __has_allocate_hint<allocator_type, size_type, const_void_pointer>());}

    _LIBCPP_INLINE_VISIBILITY
    static void deallocate(allocator_type& __a, pointer __p, size_type __n) _NOEXCEPT
        {__a.deallocate(__p, __n);}

#ifndef _LIBCPP_HAS_NO_VARIADICS
    template <class _Tp, class... _Args>
        _LIBCPP_INLINE_VISIBILITY
        static void construct(allocator_type& __a, _Tp* __p, _Args&&... __args)
            {__construct(__has_construct<allocator_type, _Tp*, _Args...>(),
                         __a, __p, _VSTD::forward<_Args>(__args)...);}
#else  // _LIBCPP_HAS_NO_VARIADICS
    template <class _Tp>
        _LIBCPP_INLINE_VISIBILITY
        static void construct(allocator_type& __a, _Tp* __p)
            {
                ::new ((void*)__p) _Tp();
            }
    template <class _Tp, class _A0>
        _LIBCPP_INLINE_VISIBILITY
        static void construct(allocator_type& __a, _Tp* __p, const _A0& __a0)
            {
                ::new ((void*)__p) _Tp(__a0);
            }
    template <class _Tp, class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        static void construct(allocator_type& __a, _Tp* __p, const _A0& __a0,
                              const _A1& __a1)
            {
                ::new ((void*)__p) _Tp(__a0, __a1);
            }
    template <class _Tp, class _A0, class _A1, class _A2>
        _LIBCPP_INLINE_VISIBILITY
        static void construct(allocator_type& __a, _Tp* __p, const _A0& __a0,
                              const _A1& __a1, const _A2& __a2)
            {
                ::new ((void*)__p) _Tp(__a0, __a1, __a2);
            }
#endif  // _LIBCPP_HAS_NO_VARIADICS

    template <class _Tp>
        _LIBCPP_INLINE_VISIBILITY
        static void destroy(allocator_type& __a, _Tp* __p)
            {__destroy(__has_destroy<allocator_type, _Tp*>(), __a, __p);}

    _LIBCPP_INLINE_VISIBILITY
    static size_type max_size(const allocator_type& __a) _NOEXCEPT
        {return __max_size(__has_max_size<const allocator_type>(), __a);}

    _LIBCPP_INLINE_VISIBILITY
    static allocator_type
        select_on_container_copy_construction(const allocator_type& __a)
            {return select_on_container_copy_construction(
                __has_select_on_container_copy_construction<const allocator_type>(),
                __a);}

    template <class _Ptr>
        _LIBCPP_INLINE_VISIBILITY
        static
        void
        __construct_forward(allocator_type& __a, _Ptr __begin1, _Ptr __end1, _Ptr& __begin2)
        {
            for (; __begin1 != __end1; ++__begin1, ++__begin2)
                construct(__a, _VSTD::__to_raw_pointer(__begin2), _VSTD::move_if_noexcept(*__begin1));
        }

    template <class _Tp>
        _LIBCPP_INLINE_VISIBILITY
        static
        typename enable_if
        <
            (is_same<allocator_type, allocator<_Tp> >::value
                || !__has_construct<allocator_type, _Tp*, _Tp>::value) &&
             is_trivially_move_constructible<_Tp>::value,
            void
        >::type
        __construct_forward(allocator_type& __a, _Tp* __begin1, _Tp* __end1, _Tp*& __begin2)
        {
            ptrdiff_t _Np = __end1 - __begin1;
            if (_Np > 0)
            {
                _VSTD::memcpy(__begin2, __begin1, _Np * sizeof(_Tp));
                __begin2 += _Np;
            }
        }

    template <class _Iter, class _Ptr>
        _LIBCPP_INLINE_VISIBILITY
        static
        void
        __construct_range_forward(allocator_type& __a, _Iter __begin1, _Iter __end1, _Ptr& __begin2)
        {
            for (; __begin1 != __end1; ++__begin1, (void) ++__begin2)
                construct(__a, _VSTD::__to_raw_pointer(__begin2), *__begin1);
        }

    template <class _Tp>
        _LIBCPP_INLINE_VISIBILITY
        static
        typename enable_if
        <
            (is_same<allocator_type, allocator<_Tp> >::value
                || !__has_construct<allocator_type, _Tp*, _Tp>::value) &&
             is_trivially_move_constructible<_Tp>::value,
            void
        >::type
        __construct_range_forward(allocator_type& __a, _Tp* __begin1, _Tp* __end1, _Tp*& __begin2)
        {
            typedef typename remove_const<_Tp>::type _Vp;
            ptrdiff_t _Np = __end1 - __begin1;
            if (_Np > 0)
            {
                _VSTD::memcpy(const_cast<_Vp*>(__begin2), __begin1, _Np * sizeof(_Tp));
                __begin2 += _Np;
            }
        }

    template <class _Ptr>
        _LIBCPP_INLINE_VISIBILITY
        static
        void
        __construct_backward(allocator_type& __a, _Ptr __begin1, _Ptr __end1, _Ptr& __end2)
        {
            while (__end1 != __begin1)
            {
                construct(__a, _VSTD::__to_raw_pointer(__end2-1), _VSTD::move_if_noexcept(*--__end1));
                --__end2;
            }
        }

    template <class _Tp>
        _LIBCPP_INLINE_VISIBILITY
        static
        typename enable_if
        <
            (is_same<allocator_type, allocator<_Tp> >::value
                || !__has_construct<allocator_type, _Tp*, _Tp>::value) &&
             is_trivially_move_constructible<_Tp>::value,
            void
        >::type
        __construct_backward(allocator_type& __a, _Tp* __begin1, _Tp* __end1, _Tp*& __end2)
        {
            ptrdiff_t _Np = __end1 - __begin1;
            __end2 -= _Np;
            if (_Np > 0)
                _VSTD::memcpy(__end2, __begin1, _Np * sizeof(_Tp));
        }

private:

    _LIBCPP_INLINE_VISIBILITY
    static pointer allocate(allocator_type& __a, size_type __n,
        const_void_pointer __hint, true_type)
        {return __a.allocate(__n, __hint);}
    _LIBCPP_INLINE_VISIBILITY
    static pointer allocate(allocator_type& __a, size_type __n,
        const_void_pointer, false_type)
        {return __a.allocate(__n);}

#ifndef _LIBCPP_HAS_NO_VARIADICS
    template <class _Tp, class... _Args>
        _LIBCPP_INLINE_VISIBILITY
        static void __construct(true_type, allocator_type& __a, _Tp* __p, _Args&&... __args)
            {__a.construct(__p, _VSTD::forward<_Args>(__args)...);}
    template <class _Tp, class... _Args>
        _LIBCPP_INLINE_VISIBILITY
        static void __construct(false_type, allocator_type&, _Tp* __p, _Args&&... __args)
            {
                ::new ((void*)__p) _Tp(_VSTD::forward<_Args>(__args)...);
            }
#endif  // _LIBCPP_HAS_NO_VARIADICS

    template <class _Tp>
        _LIBCPP_INLINE_VISIBILITY
        static void __destroy(true_type, allocator_type& __a, _Tp* __p)
            {__a.destroy(__p);}
    template <class _Tp>
        _LIBCPP_INLINE_VISIBILITY
        static void __destroy(false_type, allocator_type&, _Tp* __p)
            {
                __p->~_Tp();
            }

    _LIBCPP_INLINE_VISIBILITY
    static size_type __max_size(true_type, const allocator_type& __a)
            {return __a.max_size();}
    _LIBCPP_INLINE_VISIBILITY
    static size_type __max_size(false_type, const allocator_type&)
            {return numeric_limits<size_type>::max() / sizeof(value_type);}

    _LIBCPP_INLINE_VISIBILITY
    static allocator_type
        select_on_container_copy_construction(true_type, const allocator_type& __a)
            {return __a.select_on_container_copy_construction();}
    _LIBCPP_INLINE_VISIBILITY
    static allocator_type
        select_on_container_copy_construction(false_type, const allocator_type& __a)
            {return __a;}
};

template <class _Traits, class _Tp>
struct __rebind_alloc_helper
{
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
    typedef typename _Traits::template rebind_alloc<_Tp>        type;
#else
    typedef typename _Traits::template rebind_alloc<_Tp>::other type;
#endif
};

// allocator

template <class _Tp>
class _LIBCPP_TYPE_VIS_ONLY allocator
{
public:
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef _Tp*              pointer;
    typedef const _Tp*        const_pointer;
    typedef _Tp&              reference;
    typedef const _Tp&        const_reference;
    typedef _Tp               value_type;

    typedef true_type propagate_on_container_move_assignment;
    typedef true_type is_always_equal;

    template <class _Up> struct rebind {typedef allocator<_Up> other;};

    _LIBCPP_INLINE_VISIBILITY allocator() _NOEXCEPT {}
    template <class _Up> _LIBCPP_INLINE_VISIBILITY allocator(const allocator<_Up>&) _NOEXCEPT {}
    _LIBCPP_INLINE_VISIBILITY pointer address(reference __x) const _NOEXCEPT
        {return _VSTD::addressof(__x);}
    _LIBCPP_INLINE_VISIBILITY const_pointer address(const_reference __x) const _NOEXCEPT
        {return _VSTD::addressof(__x);}
    _LIBCPP_INLINE_VISIBILITY pointer allocate(size_type __n, allocator<void>::const_pointer = 0)
        {
        if (__n > max_size())
            __libcpp_throw(length_error("allocator<T>::allocate(size_t n)"
                                      " 'n' exceeds maximum supported size"));
        return static_cast<pointer>(_VSTD::__allocate(__n * sizeof(_Tp)));
        }
    _LIBCPP_INLINE_VISIBILITY void deallocate(pointer __p, size_type) _NOEXCEPT
        {_VSTD::__deallocate((void*)__p);}
    _LIBCPP_INLINE_VISIBILITY size_type max_size() const _NOEXCEPT
        {return size_type(~0) / sizeof(_Tp);}
#if !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)
    template <class _Up, class... _Args>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(_Up* __p, _Args&&... __args)
        {
            ::new((void*)__p) _Up(_VSTD::forward<_Args>(__args)...);
        }
#else  // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p)
        {
            ::new((void*)__p) _Tp();
        }
# if defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

    template <class _A0>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, _A0& __a0)
        {
            ::new((void*)__p) _Tp(__a0);
        }
    template <class _A0>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, const _A0& __a0)
        {
            ::new((void*)__p) _Tp(__a0);
        }
# endif  // defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)
    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, _A0& __a0, _A1& __a1)
        {
            ::new((void*)__p) _Tp(__a0, __a1);
        }
    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, const _A0& __a0, _A1& __a1)
        {
            ::new((void*)__p) _Tp(__a0, __a1);
        }
    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, _A0& __a0, const _A1& __a1)
        {
            ::new((void*)__p) _Tp(__a0, __a1);
        }
    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, const _A0& __a0, const _A1& __a1)
        {
            ::new((void*)__p) _Tp(__a0, __a1);
        }
#endif  // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)
    _LIBCPP_INLINE_VISIBILITY void destroy(pointer __p) {__p->~_Tp();}
};

template <class _Tp>
class _LIBCPP_TYPE_VIS_ONLY allocator<const _Tp>
{
public:
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef const _Tp*        pointer;
    typedef const _Tp*        const_pointer;
    typedef const _Tp&        reference;
    typedef const _Tp&        const_reference;
    typedef const _Tp         value_type;

    typedef true_type propagate_on_container_move_assignment;
    typedef true_type is_always_equal;

    template <class _Up> struct rebind {typedef allocator<_Up> other;};

    _LIBCPP_INLINE_VISIBILITY allocator() _NOEXCEPT {}
    template <class _Up> _LIBCPP_INLINE_VISIBILITY allocator(const allocator<_Up>&) _NOEXCEPT {}
    _LIBCPP_INLINE_VISIBILITY const_pointer address(const_reference __x) const _NOEXCEPT
        {return _VSTD::addressof(__x);}
    _LIBCPP_INLINE_VISIBILITY pointer allocate(size_type __n, allocator<void>::const_pointer = 0)
    {
        if (__n > max_size())
            __libcpp_throw(length_error("allocator<const T>::allocate(size_t n)"
                                      " 'n' exceeds maximum supported size"));
        return static_cast<pointer>(_VSTD::__allocate(__n * sizeof(_Tp)));
    }
    _LIBCPP_INLINE_VISIBILITY void deallocate(pointer __p, size_type) _NOEXCEPT
        {_VSTD::__deallocate((void*)__p);}
    _LIBCPP_INLINE_VISIBILITY size_type max_size() const _NOEXCEPT
        {return size_type(~0) / sizeof(_Tp);}
#if !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)
    template <class _Up, class... _Args>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(_Up* __p, _Args&&... __args)
        {
            ::new((void*)__p) _Up(_VSTD::forward<_Args>(__args)...);
        }
#else  // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p)
        {
            ::new((void*)__p) _Tp();
        }
# if defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

    template <class _A0>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, _A0& __a0)
        {
            ::new((void*)__p) _Tp(__a0);
        }
    template <class _A0>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, const _A0& __a0)
        {
            ::new((void*)__p) _Tp(__a0);
        }
# endif  // defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)
    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, _A0& __a0, _A1& __a1)
        {
            ::new((void*)__p) _Tp(__a0, __a1);
        }
    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, const _A0& __a0, _A1& __a1)
        {
            ::new((void*)__p) _Tp(__a0, __a1);
        }
    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, _A0& __a0, const _A1& __a1)
        {
            ::new((void*)__p) _Tp(__a0, __a1);
        }
    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        void
        construct(pointer __p, const _A0& __a0, const _A1& __a1)
        {
            ::new((void*)__p) _Tp(__a0, __a1);
        }
#endif  // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)
    _LIBCPP_INLINE_VISIBILITY void destroy(pointer __p) {__p->~_Tp();}
};

template <class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
bool operator==(const allocator<_Tp>&, const allocator<_Up>&) _NOEXCEPT {return true;}

template <class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
bool operator!=(const allocator<_Tp>&, const allocator<_Up>&) _NOEXCEPT {return false;}

template <class _OutputIterator, class _Tp>
class _LIBCPP_TYPE_VIS_ONLY raw_storage_iterator
    : public iterator<output_iterator_tag,
                      _Tp,                                         // purposefully not C++03
                      ptrdiff_t,                                   // purposefully not C++03
                      _Tp*,                                        // purposefully not C++03
                      raw_storage_iterator<_OutputIterator, _Tp>&> // purposefully not C++03
{
private:
    _OutputIterator __x_;
public:
    _LIBCPP_INLINE_VISIBILITY explicit raw_storage_iterator(_OutputIterator __x) : __x_(__x) {}
    _LIBCPP_INLINE_VISIBILITY raw_storage_iterator& operator*() {return *this;}
    _LIBCPP_INLINE_VISIBILITY raw_storage_iterator& operator=(const _Tp& __element)
        {::new(&*__x_) _Tp(__element); return *this;}
#if _LIBCPP_STD_VER >= 14
    _LIBCPP_INLINE_VISIBILITY raw_storage_iterator& operator=(_Tp&& __element)
        {::new(&*__x_) _Tp(_VSTD::move(__element)); return *this;}
#endif
    _LIBCPP_INLINE_VISIBILITY raw_storage_iterator& operator++() {++__x_; return *this;}
    _LIBCPP_INLINE_VISIBILITY raw_storage_iterator  operator++(int)
        {raw_storage_iterator __t(*this); ++__x_; return __t;}
#if _LIBCPP_STD_VER >= 14
    _LIBCPP_INLINE_VISIBILITY _OutputIterator base() const { return __x_; } 
#endif
};

template <class _Tp>
pair<_Tp*, ptrdiff_t>
get_temporary_buffer(ptrdiff_t __n) _NOEXCEPT
{
    pair<_Tp*, ptrdiff_t> __r(0, 0);
    const ptrdiff_t __m = (~ptrdiff_t(0) ^
                           ptrdiff_t(ptrdiff_t(1) << (sizeof(ptrdiff_t) * __CHAR_BIT__ - 1)))
                           / sizeof(_Tp);
    if (__n > __m)
        __n = __m;
    while (__n > 0)
    {
        __r.first = static_cast<_Tp*>(::operator new(__n * sizeof(_Tp), nothrow));
        if (__r.first)
        {
            __r.second = __n;
            break;
        }
        __n /= 2;
    }
    return __r;
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void return_temporary_buffer(_Tp* __p) _NOEXCEPT {::operator delete(__p);}

template <class _Tp>
struct auto_ptr_ref
{
    _Tp* __ptr_;
};

template<class _Tp>
class _LIBCPP_TYPE_VIS_ONLY auto_ptr
{
private:
    _Tp* __ptr_;
public:
    typedef _Tp element_type;

    _LIBCPP_INLINE_VISIBILITY explicit auto_ptr(_Tp* __p = 0) throw() : __ptr_(__p) {}
    _LIBCPP_INLINE_VISIBILITY auto_ptr(auto_ptr& __p) throw() : __ptr_(__p.release()) {}
    template<class _Up> _LIBCPP_INLINE_VISIBILITY auto_ptr(auto_ptr<_Up>& __p) throw()
        : __ptr_(__p.release()) {}
    _LIBCPP_INLINE_VISIBILITY auto_ptr& operator=(auto_ptr& __p) throw()
        {reset(__p.release()); return *this;}
    template<class _Up> _LIBCPP_INLINE_VISIBILITY auto_ptr& operator=(auto_ptr<_Up>& __p) throw()
        {reset(__p.release()); return *this;}
    _LIBCPP_INLINE_VISIBILITY auto_ptr& operator=(auto_ptr_ref<_Tp> __p) throw()
        {reset(__p.__ptr_); return *this;}
    _LIBCPP_INLINE_VISIBILITY ~auto_ptr() throw() {delete __ptr_;}

    _LIBCPP_INLINE_VISIBILITY _Tp& operator*() const throw()
        {return *__ptr_;}
    _LIBCPP_INLINE_VISIBILITY _Tp* operator->() const throw() {return __ptr_;}
    _LIBCPP_INLINE_VISIBILITY _Tp* get() const throw() {return __ptr_;}
    _LIBCPP_INLINE_VISIBILITY _Tp* release() throw()
    {
        _Tp* __t = __ptr_;
        __ptr_ = 0;
        return __t;
    }
    _LIBCPP_INLINE_VISIBILITY void reset(_Tp* __p = 0) throw()
    {
        if (__ptr_ != __p)
            delete __ptr_;
        __ptr_ = __p;
    }

    _LIBCPP_INLINE_VISIBILITY auto_ptr(auto_ptr_ref<_Tp> __p) throw() : __ptr_(__p.__ptr_) {}
    template<class _Up> _LIBCPP_INLINE_VISIBILITY operator auto_ptr_ref<_Up>() throw()
        {auto_ptr_ref<_Up> __t; __t.__ptr_ = release(); return __t;}
    template<class _Up> _LIBCPP_INLINE_VISIBILITY operator auto_ptr<_Up>() throw()
        {return auto_ptr<_Up>(release());}
};

template <>
class _LIBCPP_TYPE_VIS_ONLY auto_ptr<void>
{
public:
    typedef void element_type;
};

template <class _T1, class _T2, bool = is_same<typename remove_cv<_T1>::type,
                                                     typename remove_cv<_T2>::type>::value,
                                bool = is_empty<_T1>::value
                                       && !__libcpp_is_final<_T1>::value,
                                bool = is_empty<_T2>::value
                                       && !__libcpp_is_final<_T2>::value
         >
struct __libcpp_compressed_pair_switch;

template <class _T1, class _T2, bool IsSame>
struct __libcpp_compressed_pair_switch<_T1, _T2, IsSame, false, false> {enum {value = 0};};

template <class _T1, class _T2, bool IsSame>
struct __libcpp_compressed_pair_switch<_T1, _T2, IsSame, true, false>  {enum {value = 1};};

template <class _T1, class _T2, bool IsSame>
struct __libcpp_compressed_pair_switch<_T1, _T2, IsSame, false, true>  {enum {value = 2};};

template <class _T1, class _T2>
struct __libcpp_compressed_pair_switch<_T1, _T2, false, true, true>    {enum {value = 3};};

template <class _T1, class _T2>
struct __libcpp_compressed_pair_switch<_T1, _T2, true, true, true>     {enum {value = 1};};

template <class _T1, class _T2, unsigned = __libcpp_compressed_pair_switch<_T1, _T2>::value>
class __libcpp_compressed_pair_imp;

template <class _T1, class _T2>
class __libcpp_compressed_pair_imp<_T1, _T2, 0>
{
private:
    _T1 __first_;
    _T2 __second_;
public:
    typedef _T1 _T1_param;
    typedef _T2 _T2_param;

    typedef typename remove_reference<_T1>::type& _T1_reference;
    typedef typename remove_reference<_T2>::type& _T2_reference;

    typedef const typename remove_reference<_T1>::type& _T1_const_reference;
    typedef const typename remove_reference<_T2>::type& _T2_const_reference;

    _LIBCPP_INLINE_VISIBILITY __libcpp_compressed_pair_imp() : __first_(), __second_() {}
    _LIBCPP_INLINE_VISIBILITY explicit __libcpp_compressed_pair_imp(_T1_param __t1)
        : __first_(_VSTD::forward<_T1_param>(__t1)), __second_() {}
    _LIBCPP_INLINE_VISIBILITY explicit __libcpp_compressed_pair_imp(_T2_param __t2)
        : __first_(), __second_(_VSTD::forward<_T2_param>(__t2)) {}
    _LIBCPP_INLINE_VISIBILITY __libcpp_compressed_pair_imp(_T1_param __t1, _T2_param __t2)
        : __first_(_VSTD::forward<_T1_param>(__t1)), __second_(_VSTD::forward<_T2_param>(__t2)) {}

#if defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp(const __libcpp_compressed_pair_imp& __p)
        _NOEXCEPT_(is_nothrow_copy_constructible<_T1>::value &&
                   is_nothrow_copy_constructible<_T2>::value)
        : __first_(__p.first()),
          __second_(__p.second()) {}

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp& operator=(const __libcpp_compressed_pair_imp& __p)
        _NOEXCEPT_(is_nothrow_copy_assignable<_T1>::value &&
                   is_nothrow_copy_assignable<_T2>::value)
        {
            __first_ = __p.first();
            __second_ = __p.second();
            return *this;
        }

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp(__libcpp_compressed_pair_imp&& __p)
        _NOEXCEPT_(is_nothrow_move_constructible<_T1>::value &&
                   is_nothrow_move_constructible<_T2>::value)
        : __first_(_VSTD::forward<_T1>(__p.first())),
          __second_(_VSTD::forward<_T2>(__p.second())) {}

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp& operator=(__libcpp_compressed_pair_imp&& __p)
        _NOEXCEPT_(is_nothrow_move_assignable<_T1>::value &&
                   is_nothrow_move_assignable<_T2>::value)
        {
            __first_ = _VSTD::forward<_T1>(__p.first());
            __second_ = _VSTD::forward<_T2>(__p.second());
            return *this;
        }

#endif  // defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

#ifndef _LIBCPP_HAS_NO_VARIADICS

    template <class... _Args1, class... _Args2, size_t... _I1, size_t... _I2>
        _LIBCPP_INLINE_VISIBILITY
        __libcpp_compressed_pair_imp(piecewise_construct_t __pc,
                                     tuple<_Args1...> __first_args,
                                     tuple<_Args2...> __second_args,
                                     __tuple_indices<_I1...>,
                                     __tuple_indices<_I2...>)
            : __first_(_VSTD::forward<_Args1>(_VSTD::get<_I1>(__first_args))...),
              __second_(_VSTD::forward<_Args2>(_VSTD::get<_I2>(__second_args))...)
            {}

#endif  // _LIBCPP_HAS_NO_VARIADICS

    _LIBCPP_INLINE_VISIBILITY _T1_reference       first() _NOEXCEPT       {return __first_;}
    _LIBCPP_INLINE_VISIBILITY _T1_const_reference first() const _NOEXCEPT {return __first_;}

    _LIBCPP_INLINE_VISIBILITY _T2_reference       second() _NOEXCEPT       {return __second_;}
    _LIBCPP_INLINE_VISIBILITY _T2_const_reference second() const _NOEXCEPT {return __second_;}

    _LIBCPP_INLINE_VISIBILITY void swap(__libcpp_compressed_pair_imp& __x)
        _NOEXCEPT_(__is_nothrow_swappable<_T1>::value &&
                   __is_nothrow_swappable<_T2>::value)
    {
        using _VSTD::swap;
        swap(__first_, __x.__first_);
        swap(__second_, __x.__second_);
    }
};

template <class _T1, class _T2>
class __libcpp_compressed_pair_imp<_T1, _T2, 1>
    : private _T1
{
private:
    _T2 __second_;
public:
    typedef _T1 _T1_param;
    typedef _T2 _T2_param;

    typedef _T1&                                        _T1_reference;
    typedef typename remove_reference<_T2>::type& _T2_reference;

    typedef const _T1&                                        _T1_const_reference;
    typedef const typename remove_reference<_T2>::type& _T2_const_reference;

    _LIBCPP_INLINE_VISIBILITY __libcpp_compressed_pair_imp() : __second_() {}
    _LIBCPP_INLINE_VISIBILITY explicit __libcpp_compressed_pair_imp(_T1_param __t1)
        : _T1(_VSTD::forward<_T1_param>(__t1)), __second_() {}
    _LIBCPP_INLINE_VISIBILITY explicit __libcpp_compressed_pair_imp(_T2_param __t2)
        : __second_(_VSTD::forward<_T2_param>(__t2)) {}
    _LIBCPP_INLINE_VISIBILITY __libcpp_compressed_pair_imp(_T1_param __t1, _T2_param __t2)
        : _T1(_VSTD::forward<_T1_param>(__t1)), __second_(_VSTD::forward<_T2_param>(__t2)) {}

#if defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp(const __libcpp_compressed_pair_imp& __p)
        _NOEXCEPT_(is_nothrow_copy_constructible<_T1>::value &&
                   is_nothrow_copy_constructible<_T2>::value)
        : _T1(__p.first()), __second_(__p.second()) {}

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp& operator=(const __libcpp_compressed_pair_imp& __p)
        _NOEXCEPT_(is_nothrow_copy_assignable<_T1>::value &&
                   is_nothrow_copy_assignable<_T2>::value)
        {
            _T1::operator=(__p.first());
            __second_ = __p.second();
            return *this;
        }

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp(__libcpp_compressed_pair_imp&& __p)
        _NOEXCEPT_(is_nothrow_move_constructible<_T1>::value &&
                   is_nothrow_move_constructible<_T2>::value)
        : _T1(_VSTD::move(__p.first())), __second_(_VSTD::forward<_T2>(__p.second())) {}

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp& operator=(__libcpp_compressed_pair_imp&& __p)
        _NOEXCEPT_(is_nothrow_move_assignable<_T1>::value &&
                   is_nothrow_move_assignable<_T2>::value)
        {
            _T1::operator=(_VSTD::move(__p.first()));
            __second_ = _VSTD::forward<_T2>(__p.second());
            return *this;
        }

#endif  // defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

#ifndef _LIBCPP_HAS_NO_VARIADICS

    template <class... _Args1, class... _Args2, size_t... _I1, size_t... _I2>
        _LIBCPP_INLINE_VISIBILITY
        __libcpp_compressed_pair_imp(piecewise_construct_t __pc,
                                     tuple<_Args1...> __first_args,
                                     tuple<_Args2...> __second_args,
                                     __tuple_indices<_I1...>,
                                     __tuple_indices<_I2...>)
            : _T1(_VSTD::forward<_Args1>(_VSTD::get<_I1>(__first_args))...),
              __second_(_VSTD::forward<_Args2>(_VSTD::get<_I2>(__second_args))...)
            {}

#endif  // _LIBCPP_HAS_NO_VARIADICS

    _LIBCPP_INLINE_VISIBILITY _T1_reference       first() _NOEXCEPT       {return *this;}
    _LIBCPP_INLINE_VISIBILITY _T1_const_reference first() const _NOEXCEPT {return *this;}

    _LIBCPP_INLINE_VISIBILITY _T2_reference       second() _NOEXCEPT       {return __second_;}
    _LIBCPP_INLINE_VISIBILITY _T2_const_reference second() const _NOEXCEPT {return __second_;}

    _LIBCPP_INLINE_VISIBILITY void swap(__libcpp_compressed_pair_imp& __x)
        _NOEXCEPT_(__is_nothrow_swappable<_T1>::value &&
                   __is_nothrow_swappable<_T2>::value)
    {
        using _VSTD::swap;
        swap(__second_, __x.__second_);
    }
};

template <class _T1, class _T2>
class __libcpp_compressed_pair_imp<_T1, _T2, 2>
    : private _T2
{
private:
    _T1 __first_;
public:
    typedef _T1 _T1_param;
    typedef _T2 _T2_param;

    typedef typename remove_reference<_T1>::type& _T1_reference;
    typedef _T2&                                        _T2_reference;

    typedef const typename remove_reference<_T1>::type& _T1_const_reference;
    typedef const _T2&                                        _T2_const_reference;

    _LIBCPP_INLINE_VISIBILITY __libcpp_compressed_pair_imp() : __first_() {}
    _LIBCPP_INLINE_VISIBILITY explicit __libcpp_compressed_pair_imp(_T1_param __t1)
        : __first_(_VSTD::forward<_T1_param>(__t1)) {}
    _LIBCPP_INLINE_VISIBILITY explicit __libcpp_compressed_pair_imp(_T2_param __t2)
        : _T2(_VSTD::forward<_T2_param>(__t2)), __first_() {}
    _LIBCPP_INLINE_VISIBILITY __libcpp_compressed_pair_imp(_T1_param __t1, _T2_param __t2)
        _NOEXCEPT_(is_nothrow_move_constructible<_T1>::value &&
                   is_nothrow_move_constructible<_T2>::value)
        : _T2(_VSTD::forward<_T2_param>(__t2)), __first_(_VSTD::forward<_T1_param>(__t1)) {}

#if defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp(const __libcpp_compressed_pair_imp& __p)
        _NOEXCEPT_(is_nothrow_copy_constructible<_T1>::value &&
                   is_nothrow_copy_constructible<_T2>::value)
        : _T2(__p.second()), __first_(__p.first()) {}

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp& operator=(const __libcpp_compressed_pair_imp& __p)
        _NOEXCEPT_(is_nothrow_copy_assignable<_T1>::value &&
                   is_nothrow_copy_assignable<_T2>::value)
        {
            _T2::operator=(__p.second());
            __first_ = __p.first();
            return *this;
        }

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp(__libcpp_compressed_pair_imp&& __p)
        _NOEXCEPT_(is_nothrow_move_constructible<_T1>::value &&
                   is_nothrow_move_constructible<_T2>::value)
        : _T2(_VSTD::forward<_T2>(__p.second())), __first_(_VSTD::move(__p.first())) {}

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp& operator=(__libcpp_compressed_pair_imp&& __p)
        _NOEXCEPT_(is_nothrow_move_assignable<_T1>::value &&
                   is_nothrow_move_assignable<_T2>::value)
        {
            _T2::operator=(_VSTD::forward<_T2>(__p.second()));
            __first_ = _VSTD::move(__p.first());
            return *this;
        }

#endif  // defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

#ifndef _LIBCPP_HAS_NO_VARIADICS

    template <class... _Args1, class... _Args2, size_t... _I1, size_t... _I2>
        _LIBCPP_INLINE_VISIBILITY
        __libcpp_compressed_pair_imp(piecewise_construct_t __pc,
                                     tuple<_Args1...> __first_args,
                                     tuple<_Args2...> __second_args,
                                     __tuple_indices<_I1...>,
                                     __tuple_indices<_I2...>)
            : _T2(_VSTD::forward<_Args2>(_VSTD::get<_I2>(__second_args))...),
              __first_(_VSTD::forward<_Args1>(_VSTD::get<_I1>(__first_args))...)
              
            {}

#endif  // _LIBCPP_HAS_NO_VARIADICS

    _LIBCPP_INLINE_VISIBILITY _T1_reference       first() _NOEXCEPT       {return __first_;}
    _LIBCPP_INLINE_VISIBILITY _T1_const_reference first() const _NOEXCEPT {return __first_;}

    _LIBCPP_INLINE_VISIBILITY _T2_reference       second() _NOEXCEPT       {return *this;}
    _LIBCPP_INLINE_VISIBILITY _T2_const_reference second() const _NOEXCEPT {return *this;}

    _LIBCPP_INLINE_VISIBILITY void swap(__libcpp_compressed_pair_imp& __x)
        _NOEXCEPT_(__is_nothrow_swappable<_T1>::value &&
                   __is_nothrow_swappable<_T2>::value)
    {
        using _VSTD::swap;
        swap(__first_, __x.__first_);
    }
};

template <class _T1, class _T2>
class __libcpp_compressed_pair_imp<_T1, _T2, 3>
    : private _T1,
      private _T2
{
public:
    typedef _T1 _T1_param;
    typedef _T2 _T2_param;

    typedef _T1& _T1_reference;
    typedef _T2& _T2_reference;

    typedef const _T1& _T1_const_reference;
    typedef const _T2& _T2_const_reference;

    _LIBCPP_INLINE_VISIBILITY __libcpp_compressed_pair_imp() {}
    _LIBCPP_INLINE_VISIBILITY explicit __libcpp_compressed_pair_imp(_T1_param __t1)
        : _T1(_VSTD::forward<_T1_param>(__t1)) {}
    _LIBCPP_INLINE_VISIBILITY explicit __libcpp_compressed_pair_imp(_T2_param __t2)
        : _T2(_VSTD::forward<_T2_param>(__t2)) {}
    _LIBCPP_INLINE_VISIBILITY __libcpp_compressed_pair_imp(_T1_param __t1, _T2_param __t2)
        : _T1(_VSTD::forward<_T1_param>(__t1)), _T2(_VSTD::forward<_T2_param>(__t2)) {}

#if defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp(const __libcpp_compressed_pair_imp& __p)
        _NOEXCEPT_(is_nothrow_copy_constructible<_T1>::value &&
                   is_nothrow_copy_constructible<_T2>::value)
        : _T1(__p.first()), _T2(__p.second()) {}

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp& operator=(const __libcpp_compressed_pair_imp& __p)
        _NOEXCEPT_(is_nothrow_copy_assignable<_T1>::value &&
                   is_nothrow_copy_assignable<_T2>::value)
        {
            _T1::operator=(__p.first());
            _T2::operator=(__p.second());
            return *this;
        }

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp(__libcpp_compressed_pair_imp&& __p)
        _NOEXCEPT_(is_nothrow_move_constructible<_T1>::value &&
                   is_nothrow_move_constructible<_T2>::value)
        : _T1(_VSTD::move(__p.first())), _T2(_VSTD::move(__p.second())) {}

    _LIBCPP_INLINE_VISIBILITY
    __libcpp_compressed_pair_imp& operator=(__libcpp_compressed_pair_imp&& __p)
        _NOEXCEPT_(is_nothrow_move_assignable<_T1>::value &&
                   is_nothrow_move_assignable<_T2>::value)
        {
            _T1::operator=(_VSTD::move(__p.first()));
            _T2::operator=(_VSTD::move(__p.second()));
            return *this;
        }

#endif  // defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

#ifndef _LIBCPP_HAS_NO_VARIADICS

    template <class... _Args1, class... _Args2, size_t... _I1, size_t... _I2>
        _LIBCPP_INLINE_VISIBILITY
        __libcpp_compressed_pair_imp(piecewise_construct_t __pc,
                                     tuple<_Args1...> __first_args,
                                     tuple<_Args2...> __second_args,
                                     __tuple_indices<_I1...>,
                                     __tuple_indices<_I2...>)
            : _T1(_VSTD::forward<_Args1>(_VSTD::get<_I1>(__first_args))...),
              _T2(_VSTD::forward<_Args2>(_VSTD::get<_I2>(__second_args))...)
            {}

#endif  // _LIBCPP_HAS_NO_VARIADICS

    _LIBCPP_INLINE_VISIBILITY _T1_reference       first() _NOEXCEPT       {return *this;}
    _LIBCPP_INLINE_VISIBILITY _T1_const_reference first() const _NOEXCEPT {return *this;}

    _LIBCPP_INLINE_VISIBILITY _T2_reference       second() _NOEXCEPT       {return *this;}
    _LIBCPP_INLINE_VISIBILITY _T2_const_reference second() const _NOEXCEPT {return *this;}

    _LIBCPP_INLINE_VISIBILITY void swap(__libcpp_compressed_pair_imp&)
        _NOEXCEPT_(__is_nothrow_swappable<_T1>::value &&
                   __is_nothrow_swappable<_T2>::value)
    {
    }
};

template <class _T1, class _T2>
class __compressed_pair
    : private __libcpp_compressed_pair_imp<_T1, _T2>
{
    typedef __libcpp_compressed_pair_imp<_T1, _T2> base;
public:
    typedef typename base::_T1_param _T1_param;
    typedef typename base::_T2_param _T2_param;

    typedef typename base::_T1_reference _T1_reference;
    typedef typename base::_T2_reference _T2_reference;

    typedef typename base::_T1_const_reference _T1_const_reference;
    typedef typename base::_T2_const_reference _T2_const_reference;

    _LIBCPP_INLINE_VISIBILITY __compressed_pair() {}
    _LIBCPP_INLINE_VISIBILITY explicit __compressed_pair(_T1_param __t1)
        : base(_VSTD::forward<_T1_param>(__t1)) {}
    _LIBCPP_INLINE_VISIBILITY explicit __compressed_pair(_T2_param __t2)
        : base(_VSTD::forward<_T2_param>(__t2)) {}
    _LIBCPP_INLINE_VISIBILITY __compressed_pair(_T1_param __t1, _T2_param __t2)
        : base(_VSTD::forward<_T1_param>(__t1), _VSTD::forward<_T2_param>(__t2)) {}

#if defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

    _LIBCPP_INLINE_VISIBILITY
    __compressed_pair(const __compressed_pair& __p)
        _NOEXCEPT_(is_nothrow_copy_constructible<_T1>::value &&
                   is_nothrow_copy_constructible<_T2>::value)
        : base(__p) {}

    _LIBCPP_INLINE_VISIBILITY
    __compressed_pair& operator=(const __compressed_pair& __p)
        _NOEXCEPT_(is_nothrow_copy_assignable<_T1>::value &&
                   is_nothrow_copy_assignable<_T2>::value)
        {
            base::operator=(__p);
            return *this;
        }

    _LIBCPP_INLINE_VISIBILITY
    __compressed_pair(__compressed_pair&& __p)
        _NOEXCEPT_(is_nothrow_move_constructible<_T1>::value &&
                   is_nothrow_move_constructible<_T2>::value)
        : base(_VSTD::move(__p)) {}

    _LIBCPP_INLINE_VISIBILITY
    __compressed_pair& operator=(__compressed_pair&& __p)
        _NOEXCEPT_(is_nothrow_move_assignable<_T1>::value &&
                   is_nothrow_move_assignable<_T2>::value)
        {
            base::operator=(_VSTD::move(__p));
            return *this;
        }

#endif  // defined(_LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

#ifndef _LIBCPP_HAS_NO_VARIADICS

    template <class... _Args1, class... _Args2>
        _LIBCPP_INLINE_VISIBILITY
        __compressed_pair(piecewise_construct_t __pc, tuple<_Args1...> __first_args,
                                                      tuple<_Args2...> __second_args)
            : base(__pc, _VSTD::move(__first_args), _VSTD::move(__second_args),
                   typename __make_tuple_indices<sizeof...(_Args1)>::type(),
                   typename __make_tuple_indices<sizeof...(_Args2) >::type())
            {}

#endif  // _LIBCPP_HAS_NO_VARIADICS

    _LIBCPP_INLINE_VISIBILITY _T1_reference       first() _NOEXCEPT       {return base::first();}
    _LIBCPP_INLINE_VISIBILITY _T1_const_reference first() const _NOEXCEPT {return base::first();}

    _LIBCPP_INLINE_VISIBILITY _T2_reference       second() _NOEXCEPT       {return base::second();}
    _LIBCPP_INLINE_VISIBILITY _T2_const_reference second() const _NOEXCEPT {return base::second();}

    _LIBCPP_INLINE_VISIBILITY void swap(__compressed_pair& __x)
        _NOEXCEPT_(__is_nothrow_swappable<_T1>::value &&
                   __is_nothrow_swappable<_T2>::value)
        {base::swap(__x);}
};

template <class _T1, class _T2>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(__compressed_pair<_T1, _T2>& __x, __compressed_pair<_T1, _T2>& __y)
        _NOEXCEPT_(__is_nothrow_swappable<_T1>::value &&
                   __is_nothrow_swappable<_T2>::value)
    {__x.swap(__y);}

// __same_or_less_cv_qualified

template <class _Ptr1, class _Ptr2,
          bool = is_same<typename remove_cv<typename pointer_traits<_Ptr1>::element_type>::type,
                         typename remove_cv<typename pointer_traits<_Ptr2>::element_type>::type
                        >::value
         >
struct __same_or_less_cv_qualified_imp
    : is_convertible<_Ptr1, _Ptr2> {};

template <class _Ptr1, class _Ptr2>
struct __same_or_less_cv_qualified_imp<_Ptr1, _Ptr2, false>
    : false_type {};

template <class _Ptr1, class _Ptr2, bool = is_pointer<_Ptr1>::value ||
                                           is_same<_Ptr1, _Ptr2>::value ||
                                           __has_element_type<_Ptr1>::value>
struct __same_or_less_cv_qualified
    : __same_or_less_cv_qualified_imp<_Ptr1, _Ptr2> {};

template <class _Ptr1, class _Ptr2>
struct __same_or_less_cv_qualified<_Ptr1, _Ptr2, false>
    : false_type {};

// default_delete

template <class _Tp>
struct _LIBCPP_TYPE_VIS_ONLY default_delete
{
#ifndef _LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR default_delete() _NOEXCEPT = default;
#else
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR default_delete() _NOEXCEPT {}
#endif
    template <class _Up>
        _LIBCPP_INLINE_VISIBILITY default_delete(const default_delete<_Up>&,
             typename enable_if<is_convertible<_Up*, _Tp*>::value>::type* = 0) _NOEXCEPT {}
    _LIBCPP_INLINE_VISIBILITY void operator() (_Tp* __ptr) const _NOEXCEPT
        {
            static_assert(sizeof(_Tp) > 0, "default_delete can not delete incomplete type");
            static_assert(!is_void<_Tp>::value, "default_delete can not delete incomplete type");
            delete __ptr;
        }
};

template <class _Tp>
struct _LIBCPP_TYPE_VIS_ONLY default_delete<_Tp[]>
{
public:
#ifndef _LIBCPP_HAS_NO_DEFAULTED_FUNCTIONS
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR default_delete() _NOEXCEPT = default;
#else
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR default_delete() _NOEXCEPT {}
#endif
    template <class _Up>
        _LIBCPP_INLINE_VISIBILITY default_delete(const default_delete<_Up[]>&,
             typename enable_if<__same_or_less_cv_qualified<_Up*, _Tp*>::value>::type* = 0) _NOEXCEPT {}
    template <class _Up>
        _LIBCPP_INLINE_VISIBILITY
        void operator() (_Up* __ptr,
                         typename enable_if<__same_or_less_cv_qualified<_Up*, _Tp*>::value>::type* = 0) const _NOEXCEPT
        {
            static_assert(sizeof(_Tp) > 0, "default_delete can not delete incomplete type");
            static_assert(!is_void<_Tp>::value, "default_delete can not delete void type");
            delete [] __ptr;
        }
};

template <class _Tp, class _Dp = default_delete<_Tp> >
class _LIBCPP_TYPE_VIS_ONLY unique_ptr
{
public:
    typedef _Tp element_type;
    typedef _Dp deleter_type;
    typedef typename __pointer_type<_Tp, deleter_type>::type pointer;
private:
    __compressed_pair<pointer, deleter_type> __ptr_;

#ifdef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    unique_ptr(unique_ptr&);
    template <class _Up, class _Ep>
        unique_ptr(unique_ptr<_Up, _Ep>&);
    unique_ptr& operator=(unique_ptr&);
    template <class _Up, class _Ep>
        unique_ptr& operator=(unique_ptr<_Up, _Ep>&);
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    struct __nat {int __for_bool_;};

    typedef       typename remove_reference<deleter_type>::type& _Dp_reference;
    typedef const typename remove_reference<deleter_type>::type& _Dp_const_reference;
public:
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR unique_ptr() _NOEXCEPT
        : __ptr_(pointer())
        {
            static_assert(!is_pointer<deleter_type>::value,
                "unique_ptr constructed with null function pointer deleter");
        }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR unique_ptr(nullptr_t) _NOEXCEPT
        : __ptr_(pointer())
        {
            static_assert(!is_pointer<deleter_type>::value,
                "unique_ptr constructed with null function pointer deleter");
        }
    _LIBCPP_INLINE_VISIBILITY explicit unique_ptr(pointer __p) _NOEXCEPT
        : __ptr_(_VSTD::move(__p))
        {
            static_assert(!is_pointer<deleter_type>::value,
                "unique_ptr constructed with null function pointer deleter");
        }

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY unique_ptr(pointer __p, typename conditional<
                                        is_reference<deleter_type>::value,
                                        deleter_type,
                                        typename add_lvalue_reference<const deleter_type>::type>::type __d)
             _NOEXCEPT
        : __ptr_(__p, __d) {}

    _LIBCPP_INLINE_VISIBILITY unique_ptr(pointer __p, typename remove_reference<deleter_type>::type&& __d)
             _NOEXCEPT
        : __ptr_(__p, _VSTD::move(__d))
        {
            static_assert(!is_reference<deleter_type>::value, "rvalue deleter bound to reference");
        }
    _LIBCPP_INLINE_VISIBILITY unique_ptr(unique_ptr&& __u) _NOEXCEPT
        : __ptr_(__u.release(), _VSTD::forward<deleter_type>(__u.get_deleter())) {}
    template <class _Up, class _Ep>
        _LIBCPP_INLINE_VISIBILITY
        unique_ptr(unique_ptr<_Up, _Ep>&& __u,
                   typename enable_if
                      <
                        !is_array<_Up>::value &&
                         is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>::value &&
                         is_convertible<_Ep, deleter_type>::value &&
                         (
                            !is_reference<deleter_type>::value ||
                            is_same<deleter_type, _Ep>::value
                         ),
                         __nat
                      >::type = __nat()) _NOEXCEPT
            : __ptr_(__u.release(), _VSTD::forward<_Ep>(__u.get_deleter())) {}

    template <class _Up>
        _LIBCPP_INLINE_VISIBILITY unique_ptr(auto_ptr<_Up>&& __p,
                typename enable_if<
                                      is_convertible<_Up*, _Tp*>::value &&
                                      is_same<_Dp, default_delete<_Tp> >::value,
                                      __nat
                                  >::type = __nat()) _NOEXCEPT
            : __ptr_(__p.release())
            {
            }

        _LIBCPP_INLINE_VISIBILITY unique_ptr& operator=(unique_ptr&& __u) _NOEXCEPT
            {
                reset(__u.release());
                __ptr_.second() = _VSTD::forward<deleter_type>(__u.get_deleter());
                return *this;
            }

        template <class _Up, class _Ep>
            _LIBCPP_INLINE_VISIBILITY
            typename enable_if
            <
                !is_array<_Up>::value &&
                is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>::value &&
                is_assignable<deleter_type&, _Ep&&>::value,
                unique_ptr&
            >::type
            operator=(unique_ptr<_Up, _Ep>&& __u) _NOEXCEPT
            {
                reset(__u.release());
                __ptr_.second() = _VSTD::forward<_Ep>(__u.get_deleter());
                return *this;
            }
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    _LIBCPP_INLINE_VISIBILITY operator __rv<unique_ptr>()
    {
        return __rv<unique_ptr>(*this);
    }

    _LIBCPP_INLINE_VISIBILITY unique_ptr(__rv<unique_ptr> __u)
        : __ptr_(__u->release(), _VSTD::forward<deleter_type>(__u->get_deleter())) {}

    template <class _Up, class _Ep>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<
        !is_array<_Up>::value &&
        is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>::value &&
        is_assignable<deleter_type&, _Ep&>::value,
        unique_ptr&
    >::type
    operator=(unique_ptr<_Up, _Ep> __u)
    {
        reset(__u.release());
        __ptr_.second() = _VSTD::forward<_Ep>(__u.get_deleter());
        return *this;
    }

    _LIBCPP_INLINE_VISIBILITY unique_ptr(pointer __p, deleter_type __d)
        : __ptr_(_VSTD::move(__p), _VSTD::move(__d)) {}

    template <class _Up>
        _LIBCPP_INLINE_VISIBILITY
                typename enable_if<
                                      is_convertible<_Up*, _Tp*>::value &&
                                      is_same<_Dp, default_delete<_Tp> >::value,
                                      unique_ptr&
                                  >::type
        operator=(auto_ptr<_Up> __p)
            {reset(__p.release()); return *this;}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY ~unique_ptr() {reset();}

    _LIBCPP_INLINE_VISIBILITY unique_ptr& operator=(nullptr_t) _NOEXCEPT
    {
        reset();
        return *this;
    }

    _LIBCPP_INLINE_VISIBILITY typename add_lvalue_reference<_Tp>::type operator*() const
        {return *__ptr_.first();}
    _LIBCPP_INLINE_VISIBILITY pointer operator->() const _NOEXCEPT {return __ptr_.first();}
    _LIBCPP_INLINE_VISIBILITY pointer get() const _NOEXCEPT {return __ptr_.first();}
    _LIBCPP_INLINE_VISIBILITY       _Dp_reference get_deleter() _NOEXCEPT
        {return __ptr_.second();}
    _LIBCPP_INLINE_VISIBILITY _Dp_const_reference get_deleter() const _NOEXCEPT
        {return __ptr_.second();}
    _LIBCPP_INLINE_VISIBILITY
        _LIBCPP_EXPLICIT operator bool() const _NOEXCEPT
        {return __ptr_.first() != nullptr;}

    _LIBCPP_INLINE_VISIBILITY pointer release() _NOEXCEPT
    {
        pointer __t = __ptr_.first();
        __ptr_.first() = pointer();
        return __t;
    }

    _LIBCPP_INLINE_VISIBILITY void reset(pointer __p = pointer()) _NOEXCEPT
    {
        pointer __tmp = __ptr_.first();
        __ptr_.first() = __p;
        if (__tmp)
            __ptr_.second()(__tmp);
    }

    _LIBCPP_INLINE_VISIBILITY void swap(unique_ptr& __u) _NOEXCEPT
        {__ptr_.swap(__u.__ptr_);}
};

template <class _Tp, class _Dp>
class _LIBCPP_TYPE_VIS_ONLY unique_ptr<_Tp[], _Dp>
{
public:
    typedef _Tp element_type;
    typedef _Dp deleter_type;
    typedef typename __pointer_type<_Tp, deleter_type>::type pointer;
private:
    __compressed_pair<pointer, deleter_type> __ptr_;

#ifdef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    unique_ptr(unique_ptr&);
    template <class _Up>
        unique_ptr(unique_ptr<_Up>&);
    unique_ptr& operator=(unique_ptr&);
    template <class _Up>
        unique_ptr& operator=(unique_ptr<_Up>&);
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    struct __nat {int __for_bool_;};

    typedef       typename remove_reference<deleter_type>::type& _Dp_reference;
    typedef const typename remove_reference<deleter_type>::type& _Dp_const_reference;
public:
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR unique_ptr() _NOEXCEPT
        : __ptr_(pointer())
        {
            static_assert(!is_pointer<deleter_type>::value,
                "unique_ptr constructed with null function pointer deleter");
        }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR unique_ptr(nullptr_t) _NOEXCEPT
        : __ptr_(pointer())
        {
            static_assert(!is_pointer<deleter_type>::value,
                "unique_ptr constructed with null function pointer deleter");
        }
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY explicit unique_ptr(_Pp __p,
            typename enable_if<__same_or_less_cv_qualified<_Pp, pointer>::value, __nat>::type = __nat()) _NOEXCEPT
        : __ptr_(__p)
        {
            static_assert(!is_pointer<deleter_type>::value,
                "unique_ptr constructed with null function pointer deleter");
        }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY unique_ptr(_Pp __p, typename conditional<
                                       is_reference<deleter_type>::value,
                                       deleter_type,
                                       typename add_lvalue_reference<const deleter_type>::type>::type __d,
                                       typename enable_if<__same_or_less_cv_qualified<_Pp, pointer>::value, __nat>::type = __nat())
             _NOEXCEPT
        : __ptr_(__p, __d) {}

    _LIBCPP_INLINE_VISIBILITY unique_ptr(nullptr_t, typename conditional<
                                       is_reference<deleter_type>::value,
                                       deleter_type,
                                       typename add_lvalue_reference<const deleter_type>::type>::type __d)
             _NOEXCEPT
        : __ptr_(pointer(), __d) {}

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY unique_ptr(_Pp __p,
                                         typename remove_reference<deleter_type>::type&& __d,
                                         typename enable_if<__same_or_less_cv_qualified<_Pp, pointer>::value, __nat>::type = __nat())
             _NOEXCEPT
        : __ptr_(__p, _VSTD::move(__d))
        {
            static_assert(!is_reference<deleter_type>::value, "rvalue deleter bound to reference");
        }

    _LIBCPP_INLINE_VISIBILITY unique_ptr(nullptr_t, typename remove_reference<deleter_type>::type&& __d)
             _NOEXCEPT
        : __ptr_(pointer(), _VSTD::move(__d))
        {
            static_assert(!is_reference<deleter_type>::value, "rvalue deleter bound to reference");
        }

    _LIBCPP_INLINE_VISIBILITY unique_ptr(unique_ptr&& __u) _NOEXCEPT
        : __ptr_(__u.release(), _VSTD::forward<deleter_type>(__u.get_deleter())) {}

    _LIBCPP_INLINE_VISIBILITY unique_ptr& operator=(unique_ptr&& __u) _NOEXCEPT
        {
            reset(__u.release());
            __ptr_.second() = _VSTD::forward<deleter_type>(__u.get_deleter());
            return *this;
        }

    template <class _Up, class _Ep>
        _LIBCPP_INLINE_VISIBILITY
        unique_ptr(unique_ptr<_Up, _Ep>&& __u,
                   typename enable_if
                            <
                                is_array<_Up>::value &&
                                __same_or_less_cv_qualified<typename unique_ptr<_Up, _Ep>::pointer, pointer>::value
                                && is_convertible<_Ep, deleter_type>::value &&
                                (
                                    !is_reference<deleter_type>::value ||
                                    is_same<deleter_type, _Ep>::value
                                ),
                                __nat
                            >::type = __nat()
                  ) _NOEXCEPT
        : __ptr_(__u.release(), _VSTD::forward<deleter_type>(__u.get_deleter())) {}


        template <class _Up, class _Ep>
            _LIBCPP_INLINE_VISIBILITY
            typename enable_if
            <
                is_array<_Up>::value &&
                __same_or_less_cv_qualified<typename unique_ptr<_Up, _Ep>::pointer, pointer>::value &&
                is_assignable<deleter_type&, _Ep&&>::value,
                unique_ptr&
            >::type
            operator=(unique_ptr<_Up, _Ep>&& __u) _NOEXCEPT
            {
                reset(__u.release());
                __ptr_.second() = _VSTD::forward<_Ep>(__u.get_deleter());
                return *this;
            }
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    _LIBCPP_INLINE_VISIBILITY explicit unique_ptr(pointer __p)
        : __ptr_(__p)
        {
            static_assert(!is_pointer<deleter_type>::value,
                "unique_ptr constructed with null function pointer deleter");
        }

    _LIBCPP_INLINE_VISIBILITY unique_ptr(pointer __p, deleter_type __d)
        : __ptr_(__p, _VSTD::forward<deleter_type>(__d)) {}

    _LIBCPP_INLINE_VISIBILITY unique_ptr(nullptr_t, deleter_type __d)
        : __ptr_(pointer(), _VSTD::forward<deleter_type>(__d)) {}

    _LIBCPP_INLINE_VISIBILITY operator __rv<unique_ptr>()
    {
        return __rv<unique_ptr>(*this);
    }

    _LIBCPP_INLINE_VISIBILITY unique_ptr(__rv<unique_ptr> __u)
        : __ptr_(__u->release(), _VSTD::forward<deleter_type>(__u->get_deleter())) {}

    _LIBCPP_INLINE_VISIBILITY unique_ptr& operator=(__rv<unique_ptr> __u)
    {
        reset(__u->release());
        __ptr_.second() = _VSTD::forward<deleter_type>(__u->get_deleter());
        return *this;
    }

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY ~unique_ptr() {reset();}

    _LIBCPP_INLINE_VISIBILITY unique_ptr& operator=(nullptr_t) _NOEXCEPT
    {
        reset();
        return *this;
    }

    _LIBCPP_INLINE_VISIBILITY typename add_lvalue_reference<_Tp>::type operator[](size_t __i) const
        {return __ptr_.first()[__i];}
    _LIBCPP_INLINE_VISIBILITY pointer get() const _NOEXCEPT {return __ptr_.first();}
    _LIBCPP_INLINE_VISIBILITY       _Dp_reference get_deleter() _NOEXCEPT
        {return __ptr_.second();}
    _LIBCPP_INLINE_VISIBILITY _Dp_const_reference get_deleter() const _NOEXCEPT
        {return __ptr_.second();}
    _LIBCPP_INLINE_VISIBILITY
        _LIBCPP_EXPLICIT operator bool() const _NOEXCEPT
        {return __ptr_.first() != nullptr;}

    _LIBCPP_INLINE_VISIBILITY pointer release() _NOEXCEPT
    {
        pointer __t = __ptr_.first();
        __ptr_.first() = pointer();
        return __t;
    }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__same_or_less_cv_qualified<_Pp, pointer>::value, void>::type
    reset(_Pp __p) _NOEXCEPT
    {
        pointer __tmp = __ptr_.first();
        __ptr_.first() = __p;
        if (__tmp)
            __ptr_.second()(__tmp);
    }
    _LIBCPP_INLINE_VISIBILITY void reset(nullptr_t = nullptr) _NOEXCEPT
    {
        pointer __tmp = __ptr_.first();
        __ptr_.first() = nullptr;
        if (__tmp)
            __ptr_.second()(__tmp);
    }

    _LIBCPP_INLINE_VISIBILITY void swap(unique_ptr& __u) {__ptr_.swap(__u.__ptr_);}
private:

#ifdef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _Up>
        explicit unique_ptr(_Up);
    template <class _Up>
        unique_ptr(_Up __u,
                   typename conditional<
                                       is_reference<deleter_type>::value,
                                       deleter_type,
                                       typename add_lvalue_reference<const deleter_type>::type>::type,
                   typename enable_if
                      <
                         is_convertible<_Up, pointer>::value,
                         __nat
                      >::type = __nat());
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
};

template <class _Tp, class _Dp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if<
    __is_swappable<_Dp>::value,
    void
>::type
swap(unique_ptr<_Tp, _Dp>& __x, unique_ptr<_Tp, _Dp>& __y) _NOEXCEPT {__x.swap(__y);}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return __x.get() == __y.get();}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return !(__x == __y);}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator< (const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y)
{
    typedef typename unique_ptr<_T1, _D1>::pointer _P1;
    typedef typename unique_ptr<_T2, _D2>::pointer _P2;
    typedef typename common_type<_P1, _P2>::type _Vp;
    return less<_Vp>()(__x.get(), __y.get());
}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator> (const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return __y < __x;}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return !(__y < __x);}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return !(__x < __y);}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(const unique_ptr<_T1, _D1>& __x, nullptr_t) _NOEXCEPT
{
    return !__x;
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(nullptr_t, const unique_ptr<_T1, _D1>& __x) _NOEXCEPT
{
    return !__x;
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(const unique_ptr<_T1, _D1>& __x, nullptr_t) _NOEXCEPT
{
    return static_cast<bool>(__x);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(nullptr_t, const unique_ptr<_T1, _D1>& __x) _NOEXCEPT
{
    return static_cast<bool>(__x);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<(const unique_ptr<_T1, _D1>& __x, nullptr_t)
{
    typedef typename unique_ptr<_T1, _D1>::pointer _P1;
    return less<_P1>()(__x.get(), nullptr);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<(nullptr_t, const unique_ptr<_T1, _D1>& __x)
{
    typedef typename unique_ptr<_T1, _D1>::pointer _P1;
    return less<_P1>()(nullptr, __x.get());
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>(const unique_ptr<_T1, _D1>& __x, nullptr_t)
{
    return nullptr < __x;
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>(nullptr_t, const unique_ptr<_T1, _D1>& __x)
{
    return __x < nullptr;
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(const unique_ptr<_T1, _D1>& __x, nullptr_t)
{
    return !(nullptr < __x);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(nullptr_t, const unique_ptr<_T1, _D1>& __x)
{
    return !(__x < nullptr);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(const unique_ptr<_T1, _D1>& __x, nullptr_t)
{
    return !(__x < nullptr);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(nullptr_t, const unique_ptr<_T1, _D1>& __x)
{
    return !(nullptr < __x);
}

#ifdef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Tp, class _Dp>
inline _LIBCPP_INLINE_VISIBILITY
unique_ptr<_Tp, _Dp>
move(unique_ptr<_Tp, _Dp>& __t)
{
    return unique_ptr<_Tp, _Dp>(__rv<unique_ptr<_Tp, _Dp> >(__t));
}

#endif

#if _LIBCPP_STD_VER > 11

template<class _Tp>
struct __unique_if
{
    typedef unique_ptr<_Tp> __unique_single;
};

template<class _Tp>
struct __unique_if<_Tp[]>
{
    typedef unique_ptr<_Tp[]> __unique_array_unknown_bound;
};

template<class _Tp, size_t _Np>
struct __unique_if<_Tp[_Np]>
{
    typedef void __unique_array_known_bound;
};

template<class _Tp, class... _Args>
inline _LIBCPP_INLINE_VISIBILITY
typename __unique_if<_Tp>::__unique_single
make_unique(_Args&&... __args)
{
    return unique_ptr<_Tp>(new _Tp(_VSTD::forward<_Args>(__args)...));
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename __unique_if<_Tp>::__unique_array_unknown_bound
make_unique(size_t __n)
{
    typedef typename remove_extent<_Tp>::type _Up;
    return unique_ptr<_Tp>(new _Up[__n]());
}

template<class _Tp, class... _Args>
    typename __unique_if<_Tp>::__unique_array_known_bound
    make_unique(_Args&&...) = delete;

#endif  // _LIBCPP_STD_VER > 11

template <class _Size>
inline _LIBCPP_INLINE_VISIBILITY
_Size
__loadword(const void* __p)
{
    _Size __r;
    std::memcpy(&__r, __p, sizeof(__r));
    return __r;
}

// We use murmur2 when size_t is 32 bits, and cityhash64 when size_t
// is 64 bits.  This is because cityhash64 uses 64bit x 64bit
// multiplication, which can be very slow on 32-bit systems.
template <class _Size, size_t = sizeof(_Size)*__CHAR_BIT__>
struct __murmur2_or_cityhash;

template <class _Size>
struct __murmur2_or_cityhash<_Size, 32>
{
    _Size operator()(const void* __key, _Size __len);
};

// murmur2
template <class _Size>
_Size
__murmur2_or_cityhash<_Size, 32>::operator()(const void* __key, _Size __len) _LIBCPP_DISABLE_UBSAN_UNSIGNED_INTEGER_CHECK 
{
    const _Size __m = 0x5bd1e995;
    const _Size __r = 24;
    _Size __h = __len;
    const unsigned char* __data = static_cast<const unsigned char*>(__key);
    for (; __len >= 4; __data += 4, __len -= 4)
    {
        _Size __k = __loadword<_Size>(__data);
        __k *= __m;
        __k ^= __k >> __r;
        __k *= __m;
        __h *= __m;
        __h ^= __k;
    }
    switch (__len)
    {
    case 3:
        __h ^= __data[2] << 16;
    case 2:
        __h ^= __data[1] << 8;
    case 1:
        __h ^= __data[0];
        __h *= __m;
    }
    __h ^= __h >> 13;
    __h *= __m;
    __h ^= __h >> 15;
    return __h;
}

template <class _Size>
struct __murmur2_or_cityhash<_Size, 64>
{
    _Size operator()(const void* __key, _Size __len);

 private:
  // Some primes between 2^63 and 2^64.
  static const _Size __k0 = 0xc3a5c85c97cb3127ULL;
  static const _Size __k1 = 0xb492b66fbe98f273ULL;
  static const _Size __k2 = 0x9ae16a3b2f90404fULL;
  static const _Size __k3 = 0xc949d7c7509e6557ULL;

  static _Size __rotate(_Size __val, int __shift) {
    return __shift == 0 ? __val : ((__val >> __shift) | (__val << (64 - __shift)));
  }

  static _Size __rotate_by_at_least_1(_Size __val, int __shift) {
    return (__val >> __shift) | (__val << (64 - __shift));
  }

  static _Size __shift_mix(_Size __val) {
    return __val ^ (__val >> 47);
  }

  static _Size __hash_len_16(_Size __u, _Size __v) {
    const _Size __mul = 0x9ddfea08eb382d69ULL;
    _Size __a = (__u ^ __v) * __mul;
    __a ^= (__a >> 47);
    _Size __b = (__v ^ __a) * __mul;
    __b ^= (__b >> 47);
    __b *= __mul;
    return __b;
  }

  static _Size __hash_len_0_to_16(const char* __s, _Size __len) {
    if (__len > 8) {
      const _Size __a = __loadword<_Size>(__s);
      const _Size __b = __loadword<_Size>(__s + __len - 8);
      return __hash_len_16(__a, __rotate_by_at_least_1(__b + __len, __len)) ^ __b;
    }
    if (__len >= 4) {
      const uint32_t __a = __loadword<uint32_t>(__s);
      const uint32_t __b = __loadword<uint32_t>(__s + __len - 4);
      return __hash_len_16(__len + (__a << 3), __b);
    }
    if (__len > 0) {
      const unsigned char __a = __s[0];
      const unsigned char __b = __s[__len >> 1];
      const unsigned char __c = __s[__len - 1];
      const uint32_t __y = static_cast<uint32_t>(__a) +
                           (static_cast<uint32_t>(__b) << 8);
      const uint32_t __z = __len + (static_cast<uint32_t>(__c) << 2);
      return __shift_mix(__y * __k2 ^ __z * __k3) * __k2;
    }
    return __k2;
  }

  static _Size __hash_len_17_to_32(const char *__s, _Size __len) {
    const _Size __a = __loadword<_Size>(__s) * __k1;
    const _Size __b = __loadword<_Size>(__s + 8);
    const _Size __c = __loadword<_Size>(__s + __len - 8) * __k2;
    const _Size __d = __loadword<_Size>(__s + __len - 16) * __k0;
    return __hash_len_16(__rotate(__a - __b, 43) + __rotate(__c, 30) + __d,
                         __a + __rotate(__b ^ __k3, 20) - __c + __len);
  }

  // Return a 16-byte hash for 48 bytes.  Quick and dirty.
  // Callers do best to use "random-looking" values for a and b.
  static pair<_Size, _Size> __weak_hash_len_32_with_seeds(
      _Size __w, _Size __x, _Size __y, _Size __z, _Size __a, _Size __b) {
    __a += __w;
    __b = __rotate(__b + __a + __z, 21);
    const _Size __c = __a;
    __a += __x;
    __a += __y;
    __b += __rotate(__a, 44);
    return pair<_Size, _Size>(__a + __z, __b + __c);
  }

  // Return a 16-byte hash for s[0] ... s[31], a, and b.  Quick and dirty.
  static pair<_Size, _Size> __weak_hash_len_32_with_seeds(
      const char* __s, _Size __a, _Size __b) {
    return __weak_hash_len_32_with_seeds(__loadword<_Size>(__s),
                                         __loadword<_Size>(__s + 8),
                                         __loadword<_Size>(__s + 16),
                                         __loadword<_Size>(__s + 24),
                                         __a,
                                         __b);
  }

  // Return an 8-byte hash for 33 to 64 bytes.
  static _Size __hash_len_33_to_64(const char *__s, size_t __len) {
    _Size __z = __loadword<_Size>(__s + 24);
    _Size __a = __loadword<_Size>(__s) +
                (__len + __loadword<_Size>(__s + __len - 16)) * __k0;
    _Size __b = __rotate(__a + __z, 52);
    _Size __c = __rotate(__a, 37);
    __a += __loadword<_Size>(__s + 8);
    __c += __rotate(__a, 7);
    __a += __loadword<_Size>(__s + 16);
    _Size __vf = __a + __z;
    _Size __vs = __b + __rotate(__a, 31) + __c;
    __a = __loadword<_Size>(__s + 16) + __loadword<_Size>(__s + __len - 32);
    __z += __loadword<_Size>(__s + __len - 8);
    __b = __rotate(__a + __z, 52);
    __c = __rotate(__a, 37);
    __a += __loadword<_Size>(__s + __len - 24);
    __c += __rotate(__a, 7);
    __a += __loadword<_Size>(__s + __len - 16);
    _Size __wf = __a + __z;
    _Size __ws = __b + __rotate(__a, 31) + __c;
    _Size __r = __shift_mix((__vf + __ws) * __k2 + (__wf + __vs) * __k0);
    return __shift_mix(__r * __k0 + __vs) * __k2;
  }
};

// cityhash64
template <class _Size>
_Size
__murmur2_or_cityhash<_Size, 64>::operator()(const void* __key, _Size __len) _LIBCPP_DISABLE_UBSAN_UNSIGNED_INTEGER_CHECK 
{
  const char* __s = static_cast<const char*>(__key);
  if (__len <= 32) {
    if (__len <= 16) {
      return __hash_len_0_to_16(__s, __len);
    } else {
      return __hash_len_17_to_32(__s, __len);
    }
  } else if (__len <= 64) {
    return __hash_len_33_to_64(__s, __len);
  }

  // For strings over 64 bytes we hash the end first, and then as we
  // loop we keep 56 bytes of state: v, w, x, y, and z.
  _Size __x = __loadword<_Size>(__s + __len - 40);
  _Size __y = __loadword<_Size>(__s + __len - 16) +
              __loadword<_Size>(__s + __len - 56);
  _Size __z = __hash_len_16(__loadword<_Size>(__s + __len - 48) + __len,
                          __loadword<_Size>(__s + __len - 24));
  pair<_Size, _Size> __v = __weak_hash_len_32_with_seeds(__s + __len - 64, __len, __z);
  pair<_Size, _Size> __w = __weak_hash_len_32_with_seeds(__s + __len - 32, __y + __k1, __x);
  __x = __x * __k1 + __loadword<_Size>(__s);

  // Decrease len to the nearest multiple of 64, and operate on 64-byte chunks.
  __len = (__len - 1) & ~static_cast<_Size>(63);
  do {
    __x = __rotate(__x + __y + __v.first + __loadword<_Size>(__s + 8), 37) * __k1;
    __y = __rotate(__y + __v.second + __loadword<_Size>(__s + 48), 42) * __k1;
    __x ^= __w.second;
    __y += __v.first + __loadword<_Size>(__s + 40);
    __z = __rotate(__z + __w.first, 33) * __k1;
    __v = __weak_hash_len_32_with_seeds(__s, __v.second * __k1, __x + __w.first);
    __w = __weak_hash_len_32_with_seeds(__s + 32, __z + __w.second,
                                        __y + __loadword<_Size>(__s + 16));
    std::swap(__z, __x);
    __s += 64;
    __len -= 64;
  } while (__len != 0);
  return __hash_len_16(
      __hash_len_16(__v.first, __w.first) + __shift_mix(__y) * __k1 + __z,
      __hash_len_16(__v.second, __w.second) + __x);
}

template <class _Tp, size_t = sizeof(_Tp) / sizeof(size_t)>
struct __scalar_hash;

template <class _Tp>
struct __scalar_hash<_Tp, 0>
    : public unary_function<_Tp, size_t>
{
    _LIBCPP_INLINE_VISIBILITY
    size_t operator()(_Tp __v) const _NOEXCEPT
    {
        union
        {
            _Tp    __t;
            size_t __a;
        } __u;
        __u.__a = 0;
        __u.__t = __v;
        return __u.__a;
    }
};

template <class _Tp>
struct __scalar_hash<_Tp, 1>
    : public unary_function<_Tp, size_t>
{
    _LIBCPP_INLINE_VISIBILITY
    size_t operator()(_Tp __v) const _NOEXCEPT
    {
        union
        {
            _Tp    __t;
            size_t __a;
        } __u;
        __u.__t = __v;
        return __u.__a;
    }
};

template <class _Tp>
struct __scalar_hash<_Tp, 2>
    : public unary_function<_Tp, size_t>
{
    _LIBCPP_INLINE_VISIBILITY
    size_t operator()(_Tp __v) const _NOEXCEPT
    {
        union
        {
            _Tp __t;
            struct
            {
                size_t __a;
                size_t __b;
            } __s;
        } __u;
        __u.__t = __v;
        return __murmur2_or_cityhash<size_t>()(&__u, sizeof(__u));
    }
};

template <class _Tp>
struct __scalar_hash<_Tp, 3>
    : public unary_function<_Tp, size_t>
{
    _LIBCPP_INLINE_VISIBILITY
    size_t operator()(_Tp __v) const _NOEXCEPT
    {
        union
        {
            _Tp __t;
            struct
            {
                size_t __a;
                size_t __b;
                size_t __c;
            } __s;
        } __u;
        __u.__t = __v;
        return __murmur2_or_cityhash<size_t>()(&__u, sizeof(__u));
    }
};

template <class _Tp>
struct __scalar_hash<_Tp, 4>
    : public unary_function<_Tp, size_t>
{
    _LIBCPP_INLINE_VISIBILITY
    size_t operator()(_Tp __v) const _NOEXCEPT
    {
        union
        {
            _Tp __t;
            struct
            {
                size_t __a;
                size_t __b;
                size_t __c;
                size_t __d;
            } __s;
        } __u;
        __u.__t = __v;
        return __murmur2_or_cityhash<size_t>()(&__u, sizeof(__u));
    }
};

template<class _Tp>
struct _LIBCPP_TYPE_VIS_ONLY hash<_Tp*>
    : public unary_function<_Tp*, size_t>
{
    _LIBCPP_INLINE_VISIBILITY
    size_t operator()(_Tp* __v) const _NOEXCEPT
    {
        union
        {
            _Tp* __t;
            size_t __a;
        } __u;
        __u.__t = __v;
        return __murmur2_or_cityhash<size_t>()(&__u, sizeof(__u));
    }
};

template <class _Tp, class _Dp>
struct _LIBCPP_TYPE_VIS_ONLY hash<unique_ptr<_Tp, _Dp> >
{
    typedef unique_ptr<_Tp, _Dp> argument_type;
    typedef size_t               result_type;
    _LIBCPP_INLINE_VISIBILITY
    result_type operator()(const argument_type& __ptr) const _NOEXCEPT
    {
        typedef typename argument_type::pointer pointer;
        return hash<pointer>()(__ptr.get());
    }
};

struct __destruct_n
{
private:
    size_t size;

    template <class _Tp>
    _LIBCPP_INLINE_VISIBILITY void __process(_Tp* __p, false_type) _NOEXCEPT
        {for (size_t __i = 0; __i < size; ++__i, ++__p) __p->~_Tp();}

    template <class _Tp>
    _LIBCPP_INLINE_VISIBILITY void __process(_Tp*, true_type) _NOEXCEPT
        {}

    _LIBCPP_INLINE_VISIBILITY void __incr(false_type) _NOEXCEPT
        {++size;}
    _LIBCPP_INLINE_VISIBILITY void __incr(true_type) _NOEXCEPT
        {}

    _LIBCPP_INLINE_VISIBILITY void __set(size_t __s, false_type) _NOEXCEPT
        {size = __s;}
    _LIBCPP_INLINE_VISIBILITY void __set(size_t, true_type) _NOEXCEPT
        {}
public:
    _LIBCPP_INLINE_VISIBILITY explicit __destruct_n(size_t __s) _NOEXCEPT
        : size(__s) {}

    template <class _Tp>
    _LIBCPP_INLINE_VISIBILITY void __incr(_Tp*) _NOEXCEPT
        {__incr(integral_constant<bool, is_trivially_destructible<_Tp>::value>());}

    template <class _Tp>
    _LIBCPP_INLINE_VISIBILITY void __set(size_t __s, _Tp*) _NOEXCEPT
        {__set(__s, integral_constant<bool, is_trivially_destructible<_Tp>::value>());}

    template <class _Tp>
    _LIBCPP_INLINE_VISIBILITY void operator()(_Tp* __p) _NOEXCEPT
        {__process(__p, integral_constant<bool, is_trivially_destructible<_Tp>::value>());}
};

template <class _Alloc>
class __allocator_destructor
{
    typedef allocator_traits<_Alloc> __alloc_traits;
public:
    typedef typename __alloc_traits::pointer pointer;
    typedef typename __alloc_traits::size_type size_type;
private:
    _Alloc& __alloc_;
    size_type __s_;
public:
    _LIBCPP_INLINE_VISIBILITY __allocator_destructor(_Alloc& __a, size_type __s)
             _NOEXCEPT
        : __alloc_(__a), __s_(__s) {}
    _LIBCPP_INLINE_VISIBILITY
    void operator()(pointer __p) _NOEXCEPT
        {__alloc_traits::deallocate(__alloc_, __p, __s_);}
};

template <class _InputIterator, class _ForwardIterator>
_ForwardIterator
uninitialized_copy(_InputIterator __f, _InputIterator __l, _ForwardIterator __r)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
#ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __r;
    try
    {
#endif
        for (; __f != __l; ++__f, (void) ++__r)
            ::new (static_cast<void*>(_VSTD::addressof(*__r))) value_type(*__f);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __r; ++__s)
            __s->~value_type();
        throw;
    }
#endif
    return __r;
}

template <class _InputIterator, class _Size, class _ForwardIterator>
_ForwardIterator
uninitialized_copy_n(_InputIterator __f, _Size __n, _ForwardIterator __r)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
#ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __r;
    try
    {
#endif
        for (; __n > 0; ++__f, (void) ++__r, (void) --__n)
            ::new (static_cast<void*>(_VSTD::addressof(*__r))) value_type(*__f);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __r; ++__s)
            __s->~value_type();
        throw;
    }
#endif
    return __r;
}

template <class _ForwardIterator, class _Tp>
void
uninitialized_fill(_ForwardIterator __f, _ForwardIterator __l, const _Tp& __x)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
#ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __f;
    try
    {
#endif
        for (; __f != __l; ++__f)
            ::new (static_cast<void*>(_VSTD::addressof(*__f))) value_type(__x);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __f; ++__s)
            __s->~value_type();
        throw;
    }
#endif
}

template <class _ForwardIterator, class _Size, class _Tp>
_ForwardIterator
uninitialized_fill_n(_ForwardIterator __f, _Size __n, const _Tp& __x)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
#ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __f;
    try
    {
#endif
        for (; __n > 0; ++__f, (void) --__n)
            ::new (static_cast<void*>(_VSTD::addressof(*__f))) value_type(__x);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __f; ++__s)
            __s->~value_type();
        throw;
    }
#endif
    return __f;
}

class _LIBCPP_EXCEPTION_ABI bad_weak_ptr
    : public std::exception
{
public:
    virtual ~bad_weak_ptr() _NOEXCEPT;
    virtual const char* what() const  _NOEXCEPT;
};

template<class _Tp> class _LIBCPP_TYPE_VIS_ONLY weak_ptr;

class _LIBCPP_TYPE_VIS __shared_count
{
    __shared_count(const __shared_count&);
    __shared_count& operator=(const __shared_count&);

protected:
    long __shared_owners_;
    virtual ~__shared_count();
private:
    virtual void __on_zero_shared() _NOEXCEPT = 0;

public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __shared_count(long __refs = 0) _NOEXCEPT
        : __shared_owners_(__refs) {}

    void __add_shared() _NOEXCEPT;
    bool __release_shared() _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    long use_count() const _NOEXCEPT {
        return __libcpp_relaxed_load(&__shared_owners_) + 1;
    }
};

class _LIBCPP_TYPE_VIS __shared_weak_count
    : private __shared_count
{
    long __shared_weak_owners_;

public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __shared_weak_count(long __refs = 0) _NOEXCEPT
        : __shared_count(__refs),
          __shared_weak_owners_(__refs) {}
protected:
    virtual ~__shared_weak_count();

public:
    void __add_shared() _NOEXCEPT;
    void __add_weak() _NOEXCEPT;
    void __release_shared() _NOEXCEPT;
    void __release_weak() _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    long use_count() const _NOEXCEPT {return __shared_count::use_count();}
    __shared_weak_count* lock() _NOEXCEPT;

    // Define the function out only if we build static libc++ without RTTI.
    // Otherwise we may break clients who need to compile their projects with
    // -fno-rtti and yet link against a libc++.dylib compiled
    // without -fno-rtti.
#if !defined(_LIBCPP_NO_RTTI) || !defined(_LIBCPP_BUILD_STATIC)
    virtual const void* __get_deleter(const type_info&) const _NOEXCEPT;
#endif
private:
    virtual void __on_zero_shared_weak() _NOEXCEPT = 0;
};

template <class _Tp, class _Dp, class _Alloc>
class __shared_ptr_pointer
    : public __shared_weak_count
{
    __compressed_pair<__compressed_pair<_Tp, _Dp>, _Alloc> __data_;
public:
    _LIBCPP_INLINE_VISIBILITY
    __shared_ptr_pointer(_Tp __p, _Dp __d, _Alloc __a)
        :  __data_(__compressed_pair<_Tp, _Dp>(__p, _VSTD::move(__d)), _VSTD::move(__a)) {}

#ifndef _LIBCPP_NO_RTTI
    virtual const void* __get_deleter(const type_info&) const _NOEXCEPT;
#endif

private:
    virtual void __on_zero_shared() _NOEXCEPT;
    virtual void __on_zero_shared_weak() _NOEXCEPT;
};

#ifndef _LIBCPP_NO_RTTI

template <class _Tp, class _Dp, class _Alloc>
const void*
__shared_ptr_pointer<_Tp, _Dp, _Alloc>::__get_deleter(const type_info& __t) const _NOEXCEPT
{
    return __t == typeid(_Dp) ? _VSTD::addressof(__data_.first().second()) : 0;
}

#endif  // _LIBCPP_NO_RTTI

template <class _Tp, class _Dp, class _Alloc>
void
__shared_ptr_pointer<_Tp, _Dp, _Alloc>::__on_zero_shared() _NOEXCEPT
{
    __data_.first().second()(__data_.first().first());
    __data_.first().second().~_Dp();
}

template <class _Tp, class _Dp, class _Alloc>
void
__shared_ptr_pointer<_Tp, _Dp, _Alloc>::__on_zero_shared_weak() _NOEXCEPT
{
    typedef typename __allocator_traits_rebind<_Alloc, __shared_ptr_pointer>::type _Al;
    typedef allocator_traits<_Al> _ATraits;
    typedef pointer_traits<typename _ATraits::pointer> _PTraits;

    _Al __a(__data_.second());
    __data_.second().~_Alloc();
    __a.deallocate(_PTraits::pointer_to(*this), 1);
}

template <class _Tp, class _Alloc>
class __shared_ptr_emplace
    : public __shared_weak_count
{
    __compressed_pair<_Alloc, _Tp> __data_;
public:
#ifndef _LIBCPP_HAS_NO_VARIADICS

    _LIBCPP_INLINE_VISIBILITY
    __shared_ptr_emplace(_Alloc __a)
        :  __data_(_VSTD::move(__a)) {}

    template <class ..._Args>
        _LIBCPP_INLINE_VISIBILITY
        __shared_ptr_emplace(_Alloc __a, _Args&& ...__args)
            :  __data_(piecewise_construct, _VSTD::forward_as_tuple(__a),
                   _VSTD::forward_as_tuple(_VSTD::forward<_Args>(__args)...)) {}

#else  // _LIBCPP_HAS_NO_VARIADICS

    _LIBCPP_INLINE_VISIBILITY
    __shared_ptr_emplace(_Alloc __a)
        :  __data_(__a) {}

    template <class _A0>
        _LIBCPP_INLINE_VISIBILITY
        __shared_ptr_emplace(_Alloc __a, _A0& __a0)
            :  __data_(__a, _Tp(__a0)) {}

    template <class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY
        __shared_ptr_emplace(_Alloc __a, _A0& __a0, _A1& __a1)
            :  __data_(__a, _Tp(__a0, __a1)) {}

    template <class _A0, class _A1, class _A2>
        _LIBCPP_INLINE_VISIBILITY
        __shared_ptr_emplace(_Alloc __a, _A0& __a0, _A1& __a1, _A2& __a2)
            :  __data_(__a, _Tp(__a0, __a1, __a2)) {}

#endif  // _LIBCPP_HAS_NO_VARIADICS

private:
    virtual void __on_zero_shared() _NOEXCEPT;
    virtual void __on_zero_shared_weak() _NOEXCEPT;
public:
    _LIBCPP_INLINE_VISIBILITY
    _Tp* get() _NOEXCEPT {return &__data_.second();}
};

template <class _Tp, class _Alloc>
void
__shared_ptr_emplace<_Tp, _Alloc>::__on_zero_shared() _NOEXCEPT
{
    __data_.second().~_Tp();
}

template <class _Tp, class _Alloc>
void
__shared_ptr_emplace<_Tp, _Alloc>::__on_zero_shared_weak() _NOEXCEPT
{
    typedef typename __allocator_traits_rebind<_Alloc, __shared_ptr_emplace>::type _Al;
    typedef allocator_traits<_Al> _ATraits;
    typedef pointer_traits<typename _ATraits::pointer> _PTraits;
    _Al __a(__data_.first());
    __data_.first().~_Alloc();
    __a.deallocate(_PTraits::pointer_to(*this), 1);
}

template<class _Tp> class _LIBCPP_TYPE_VIS_ONLY enable_shared_from_this;

template<class _Tp>
class _LIBCPP_TYPE_VIS_ONLY shared_ptr
{
public:
    typedef _Tp element_type;
private:
    element_type*      __ptr_;
    __shared_weak_count* __cntrl_;

    struct __nat {int __for_bool_;};
public:
    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_CONSTEXPR shared_ptr() _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_CONSTEXPR shared_ptr(nullptr_t) _NOEXCEPT;
    template<class _Yp>
        explicit shared_ptr(_Yp* __p,
                            typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type = __nat());
    template<class _Yp, class _Dp>
        shared_ptr(_Yp* __p, _Dp __d,
                   typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type = __nat());
    template<class _Yp, class _Dp, class _Alloc>
        shared_ptr(_Yp* __p, _Dp __d, _Alloc __a,
                   typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type = __nat());
    template <class _Dp> shared_ptr(nullptr_t __p, _Dp __d);
    template <class _Dp, class _Alloc> shared_ptr(nullptr_t __p, _Dp __d, _Alloc __a);
    template<class _Yp> _LIBCPP_INLINE_VISIBILITY shared_ptr(const shared_ptr<_Yp>& __r, element_type* __p) _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    shared_ptr(const shared_ptr& __r) _NOEXCEPT;
    template<class _Yp>
        _LIBCPP_INLINE_VISIBILITY
        shared_ptr(const shared_ptr<_Yp>& __r,
                   typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat>::type = __nat())
                       _NOEXCEPT;
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    shared_ptr(shared_ptr&& __r) _NOEXCEPT;
    template<class _Yp> _LIBCPP_INLINE_VISIBILITY  shared_ptr(shared_ptr<_Yp>&& __r,
                   typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat>::type = __nat())
                       _NOEXCEPT;
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template<class _Yp> explicit shared_ptr(const weak_ptr<_Yp>& __r,
                   typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat>::type= __nat());
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template<class _Yp>
        shared_ptr(auto_ptr<_Yp>&& __r,
                   typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type = __nat());
#else
    template<class _Yp>
        shared_ptr(auto_ptr<_Yp> __r,
                   typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type = __nat());
#endif
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _Yp, class _Dp>
        shared_ptr(unique_ptr<_Yp, _Dp>&&,
                   typename enable_if
                   <
                       !is_lvalue_reference<_Dp>::value &&
                       !is_array<_Yp>::value &&
                       is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type*>::value,
                       __nat
                   >::type = __nat());
    template <class _Yp, class _Dp>
        shared_ptr(unique_ptr<_Yp, _Dp>&&,
                   typename enable_if
                   <
                       is_lvalue_reference<_Dp>::value &&
                       !is_array<_Yp>::value &&
                       is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type*>::value,
                       __nat
                   >::type = __nat());
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _Yp, class _Dp>
        shared_ptr(unique_ptr<_Yp, _Dp>,
                   typename enable_if
                   <
                       !is_lvalue_reference<_Dp>::value &&
                       !is_array<_Yp>::value &&
                       is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type*>::value,
                       __nat
                   >::type = __nat());
    template <class _Yp, class _Dp>
        shared_ptr(unique_ptr<_Yp, _Dp>,
                   typename enable_if
                   <
                       is_lvalue_reference<_Dp>::value &&
                       !is_array<_Yp>::value &&
                       is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type*>::value,
                       __nat
                   >::type = __nat());
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    ~shared_ptr();

    _LIBCPP_INLINE_VISIBILITY
    shared_ptr& operator=(const shared_ptr& __r) _NOEXCEPT;
    template<class _Yp>
        typename enable_if
        <
            is_convertible<_Yp*, element_type*>::value,
            shared_ptr&
        >::type
        _LIBCPP_INLINE_VISIBILITY
        operator=(const shared_ptr<_Yp>& __r) _NOEXCEPT;
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    shared_ptr& operator=(shared_ptr&& __r) _NOEXCEPT;
    template<class _Yp>
        typename enable_if
        <
            is_convertible<_Yp*, element_type*>::value,
            shared_ptr<_Tp>&
        >::type
        _LIBCPP_INLINE_VISIBILITY
        operator=(shared_ptr<_Yp>&& __r);
    template<class _Yp>
        _LIBCPP_INLINE_VISIBILITY
        typename enable_if
        <
            !is_array<_Yp>::value &&
            is_convertible<_Yp*, element_type*>::value,
            shared_ptr
        >::type&
        operator=(auto_ptr<_Yp>&& __r);
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template<class _Yp>
        _LIBCPP_INLINE_VISIBILITY
        typename enable_if
        <
            !is_array<_Yp>::value &&
            is_convertible<_Yp*, element_type*>::value,
            shared_ptr&
        >::type
        operator=(auto_ptr<_Yp> __r);
#endif
    template <class _Yp, class _Dp>
        typename enable_if
        <
            !is_array<_Yp>::value &&
            is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type*>::value,
            shared_ptr&
        >::type
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
        _LIBCPP_INLINE_VISIBILITY
        operator=(unique_ptr<_Yp, _Dp>&& __r);
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
        _LIBCPP_INLINE_VISIBILITY
        operator=(unique_ptr<_Yp, _Dp> __r);
#endif

    _LIBCPP_INLINE_VISIBILITY
    void swap(shared_ptr& __r) _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    void reset() _NOEXCEPT;
    template<class _Yp>
        typename enable_if
        <
            is_convertible<_Yp*, element_type*>::value,
            void
        >::type
        _LIBCPP_INLINE_VISIBILITY
        reset(_Yp* __p);
    template<class _Yp, class _Dp>
        typename enable_if
        <
            is_convertible<_Yp*, element_type*>::value,
            void
        >::type
        _LIBCPP_INLINE_VISIBILITY
        reset(_Yp* __p, _Dp __d);
    template<class _Yp, class _Dp, class _Alloc>
        typename enable_if
        <
            is_convertible<_Yp*, element_type*>::value,
            void
        >::type
        _LIBCPP_INLINE_VISIBILITY
        reset(_Yp* __p, _Dp __d, _Alloc __a);

    _LIBCPP_INLINE_VISIBILITY
    element_type* get() const _NOEXCEPT {return __ptr_;}
    _LIBCPP_INLINE_VISIBILITY
    typename add_lvalue_reference<element_type>::type operator*() const _NOEXCEPT
        {return *__ptr_;}
    _LIBCPP_INLINE_VISIBILITY
    element_type* operator->() const _NOEXCEPT {return __ptr_;}
    _LIBCPP_INLINE_VISIBILITY
    long use_count() const _NOEXCEPT {return __cntrl_ ? __cntrl_->use_count() : 0;}
    _LIBCPP_INLINE_VISIBILITY
    bool unique() const _NOEXCEPT {return use_count() == 1;}
    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_EXPLICIT operator bool() const _NOEXCEPT {return get() != 0;}
    template <class _Up>
        _LIBCPP_INLINE_VISIBILITY
        bool owner_before(shared_ptr<_Up> const& __p) const
        {return __cntrl_ < __p.__cntrl_;}
    template <class _Up>
        _LIBCPP_INLINE_VISIBILITY
        bool owner_before(weak_ptr<_Up> const& __p) const
        {return __cntrl_ < __p.__cntrl_;}
    _LIBCPP_INLINE_VISIBILITY
    bool
    __owner_equivalent(const shared_ptr& __p) const
        {return __cntrl_ == __p.__cntrl_;}

#ifndef _LIBCPP_NO_RTTI
    template <class _Dp>
        _LIBCPP_INLINE_VISIBILITY
        _Dp* __get_deleter() const _NOEXCEPT
            {return (_Dp*)(__cntrl_ ? __cntrl_->__get_deleter(typeid(_Dp)) : 0);}
#endif  // _LIBCPP_NO_RTTI

#ifndef _LIBCPP_HAS_NO_VARIADICS

    template<class ..._Args>
        static
        shared_ptr<_Tp>
        make_shared(_Args&& ...__args);

    template<class _Alloc, class ..._Args>
        static
        shared_ptr<_Tp>
        allocate_shared(const _Alloc& __a, _Args&& ...__args);

#else  // _LIBCPP_HAS_NO_VARIADICS

    static shared_ptr<_Tp> make_shared();

    template<class _A0>
        static shared_ptr<_Tp> make_shared(_A0&);

    template<class _A0, class _A1>
        static shared_ptr<_Tp> make_shared(_A0&, _A1&);

    template<class _A0, class _A1, class _A2>
        static shared_ptr<_Tp> make_shared(_A0&, _A1&, _A2&);

    template<class _Alloc>
        static shared_ptr<_Tp>
        allocate_shared(const _Alloc& __a);

    template<class _Alloc, class _A0>
        static shared_ptr<_Tp>
        allocate_shared(const _Alloc& __a, _A0& __a0);

    template<class _Alloc, class _A0, class _A1>
        static shared_ptr<_Tp>
        allocate_shared(const _Alloc& __a, _A0& __a0, _A1& __a1);

    template<class _Alloc, class _A0, class _A1, class _A2>
        static shared_ptr<_Tp>
        allocate_shared(const _Alloc& __a, _A0& __a0, _A1& __a1, _A2& __a2);

#endif  // _LIBCPP_HAS_NO_VARIADICS

private:

    template <class _Yp>
        _LIBCPP_INLINE_VISIBILITY
        void
        __enable_weak_this(const enable_shared_from_this<_Yp>* __e) _NOEXCEPT
        {
            if (__e)
            {
                __e->__weak_this_.__ptr_ = const_cast<_Yp*>(static_cast<const _Yp*>(__e));
                __e->__weak_this_.__cntrl_ = __cntrl_;
                __cntrl_->__add_weak();
            }
        }

    _LIBCPP_INLINE_VISIBILITY
    void __enable_weak_this(const volatile void*) _NOEXCEPT {}

    template <class _Up> friend class _LIBCPP_TYPE_VIS_ONLY shared_ptr;
    template <class _Up> friend class _LIBCPP_TYPE_VIS_ONLY weak_ptr;
};

template<class _Tp>
inline
_LIBCPP_CONSTEXPR
shared_ptr<_Tp>::shared_ptr() _NOEXCEPT
    : __ptr_(0),
      __cntrl_(0)
{
}

template<class _Tp>
inline
_LIBCPP_CONSTEXPR
shared_ptr<_Tp>::shared_ptr(nullptr_t) _NOEXCEPT
    : __ptr_(0),
      __cntrl_(0)
{
}

template<class _Tp>
template<class _Yp>
shared_ptr<_Tp>::shared_ptr(_Yp* __p,
                            typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type)
    : __ptr_(__p)
{
    unique_ptr<_Yp> __hold(__p);
    typedef __shared_ptr_pointer<_Yp*, default_delete<_Yp>, allocator<_Yp> > _CntrlBlk;
    __cntrl_ = new _CntrlBlk(__p, default_delete<_Yp>(), allocator<_Yp>());
    __hold.release();
    __enable_weak_this(__p);
}

template<class _Tp>
template<class _Yp, class _Dp>
shared_ptr<_Tp>::shared_ptr(_Yp* __p, _Dp __d,
                            typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type)
    : __ptr_(__p)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        typedef __shared_ptr_pointer<_Yp*, _Dp, allocator<_Yp> > _CntrlBlk;
        __cntrl_ = new _CntrlBlk(__p, __d, allocator<_Yp>());
        __enable_weak_this(__p);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        __d(__p);
        throw;
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template<class _Tp>
template<class _Dp>
shared_ptr<_Tp>::shared_ptr(nullptr_t __p, _Dp __d)
    : __ptr_(0)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        typedef __shared_ptr_pointer<nullptr_t, _Dp, allocator<_Tp> > _CntrlBlk;
        __cntrl_ = new _CntrlBlk(__p, __d, allocator<_Tp>());
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        __d(__p);
        throw;
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template<class _Tp>
template<class _Yp, class _Dp, class _Alloc>
shared_ptr<_Tp>::shared_ptr(_Yp* __p, _Dp __d, _Alloc __a,
                            typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type)
    : __ptr_(__p)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        typedef __shared_ptr_pointer<_Yp*, _Dp, _Alloc> _CntrlBlk;
        typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _A2;
        typedef __allocator_destructor<_A2> _D2;
        _A2 __a2(__a);
        unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
        ::new(static_cast<void*>(_VSTD::addressof(*__hold2.get())))
            _CntrlBlk(__p, __d, __a);
        __cntrl_ = _VSTD::addressof(*__hold2.release());
        __enable_weak_this(__p);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        __d(__p);
        throw;
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template<class _Tp>
template<class _Dp, class _Alloc>
shared_ptr<_Tp>::shared_ptr(nullptr_t __p, _Dp __d, _Alloc __a)
    : __ptr_(0)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        typedef __shared_ptr_pointer<nullptr_t, _Dp, _Alloc> _CntrlBlk;
        typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _A2;
        typedef __allocator_destructor<_A2> _D2;
        _A2 __a2(__a);
        unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
        ::new(static_cast<void*>(_VSTD::addressof(*__hold2.get())))
            _CntrlBlk(__p, __d, __a);
        __cntrl_ = _VSTD::addressof(*__hold2.release());
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        __d(__p);
        throw;
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template<class _Tp>
template<class _Yp>
inline
shared_ptr<_Tp>::shared_ptr(const shared_ptr<_Yp>& __r, element_type *__p) _NOEXCEPT
    : __ptr_(__p),
      __cntrl_(__r.__cntrl_)
{
    if (__cntrl_)
        __cntrl_->__add_shared();
}

template<class _Tp>
inline
shared_ptr<_Tp>::shared_ptr(const shared_ptr& __r) _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    if (__cntrl_)
        __cntrl_->__add_shared();
}

template<class _Tp>
template<class _Yp>
inline
shared_ptr<_Tp>::shared_ptr(const shared_ptr<_Yp>& __r,
                            typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat>::type)
         _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    if (__cntrl_)
        __cntrl_->__add_shared();
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
inline
shared_ptr<_Tp>::shared_ptr(shared_ptr&& __r) _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    __r.__ptr_ = 0;
    __r.__cntrl_ = 0;
}

template<class _Tp>
template<class _Yp>
inline
shared_ptr<_Tp>::shared_ptr(shared_ptr<_Yp>&& __r,
                            typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat>::type)
         _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    __r.__ptr_ = 0;
    __r.__cntrl_ = 0;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
template<class _Yp>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
shared_ptr<_Tp>::shared_ptr(auto_ptr<_Yp>&& __r,
#else
shared_ptr<_Tp>::shared_ptr(auto_ptr<_Yp> __r,
#endif
                            typename enable_if<is_convertible<_Yp*, element_type*>::value, __nat>::type)
    : __ptr_(__r.get())
{
    typedef __shared_ptr_pointer<_Yp*, default_delete<_Yp>, allocator<_Yp> > _CntrlBlk;
    __cntrl_ = new _CntrlBlk(__r.get(), default_delete<_Yp>(), allocator<_Yp>());
    __enable_weak_this(__r.get());
    __r.release();
}

template<class _Tp>
template <class _Yp, class _Dp>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
shared_ptr<_Tp>::shared_ptr(unique_ptr<_Yp, _Dp>&& __r,
#else
shared_ptr<_Tp>::shared_ptr(unique_ptr<_Yp, _Dp> __r,
#endif
                            typename enable_if
                            <
                                !is_lvalue_reference<_Dp>::value &&
                                !is_array<_Yp>::value &&
                                is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type*>::value,
                                __nat
                            >::type)
    : __ptr_(__r.get())
{
#if _LIBCPP_STD_VER > 11
    if (__ptr_ == nullptr)
        __cntrl_ = nullptr;
    else
#endif
    {
        typedef __shared_ptr_pointer<_Yp*, _Dp, allocator<_Yp> > _CntrlBlk;
        __cntrl_ = new _CntrlBlk(__r.get(), __r.get_deleter(), allocator<_Yp>());
        __enable_weak_this(__r.get());
    }
    __r.release();
}

template<class _Tp>
template <class _Yp, class _Dp>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
shared_ptr<_Tp>::shared_ptr(unique_ptr<_Yp, _Dp>&& __r,
#else
shared_ptr<_Tp>::shared_ptr(unique_ptr<_Yp, _Dp> __r,
#endif
                            typename enable_if
                            <
                                is_lvalue_reference<_Dp>::value &&
                                !is_array<_Yp>::value &&
                                is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type*>::value,
                                __nat
                            >::type)
    : __ptr_(__r.get())
{
#if _LIBCPP_STD_VER > 11
    if (__ptr_ == nullptr)
        __cntrl_ = nullptr;
    else
#endif
    {
        typedef __shared_ptr_pointer<_Yp*,
                                     reference_wrapper<typename remove_reference<_Dp>::type>,
                                     allocator<_Yp> > _CntrlBlk;
        __cntrl_ = new _CntrlBlk(__r.get(), ref(__r.get_deleter()), allocator<_Yp>());
        __enable_weak_this(__r.get());
    }
    __r.release();
}

#ifndef _LIBCPP_HAS_NO_VARIADICS

template<class _Tp>
template<class ..._Args>
shared_ptr<_Tp>
shared_ptr<_Tp>::make_shared(_Args&& ...__args)
{
    typedef __shared_ptr_emplace<_Tp, allocator<_Tp> > _CntrlBlk;
    typedef allocator<_CntrlBlk> _A2;
    typedef __allocator_destructor<_A2> _D2;
    _A2 __a2;
    unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
    ::new(__hold2.get()) _CntrlBlk(__a2, _VSTD::forward<_Args>(__args)...);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = __hold2.release();
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

template<class _Tp>
template<class _Alloc, class ..._Args>
shared_ptr<_Tp>
shared_ptr<_Tp>::allocate_shared(const _Alloc& __a, _Args&& ...__args)
{
    typedef __shared_ptr_emplace<_Tp, _Alloc> _CntrlBlk;
    typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _A2;
    typedef __allocator_destructor<_A2> _D2;
    _A2 __a2(__a);
    unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
    ::new(static_cast<void*>(_VSTD::addressof(*__hold2.get())))
        _CntrlBlk(__a, _VSTD::forward<_Args>(__args)...);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = _VSTD::addressof(*__hold2.release());
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

#else  // _LIBCPP_HAS_NO_VARIADICS

template<class _Tp>
shared_ptr<_Tp>
shared_ptr<_Tp>::make_shared()
{
    typedef __shared_ptr_emplace<_Tp, allocator<_Tp> > _CntrlBlk;
    typedef allocator<_CntrlBlk> _Alloc2;
    typedef __allocator_destructor<_Alloc2> _D2;
    _Alloc2 __alloc2;
    unique_ptr<_CntrlBlk, _D2> __hold2(__alloc2.allocate(1), _D2(__alloc2, 1));
    ::new(__hold2.get()) _CntrlBlk(__alloc2);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = __hold2.release();
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

template<class _Tp>
template<class _A0>
shared_ptr<_Tp>
shared_ptr<_Tp>::make_shared(_A0& __a0)
{
    typedef __shared_ptr_emplace<_Tp, allocator<_Tp> > _CntrlBlk;
    typedef allocator<_CntrlBlk> _Alloc2;
    typedef __allocator_destructor<_Alloc2> _D2;
    _Alloc2 __alloc2;
    unique_ptr<_CntrlBlk, _D2> __hold2(__alloc2.allocate(1), _D2(__alloc2, 1));
    ::new(__hold2.get()) _CntrlBlk(__alloc2, __a0);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = __hold2.release();
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

template<class _Tp>
template<class _A0, class _A1>
shared_ptr<_Tp>
shared_ptr<_Tp>::make_shared(_A0& __a0, _A1& __a1)
{
    typedef __shared_ptr_emplace<_Tp, allocator<_Tp> > _CntrlBlk;
    typedef allocator<_CntrlBlk> _Alloc2;
    typedef __allocator_destructor<_Alloc2> _D2;
    _Alloc2 __alloc2;
    unique_ptr<_CntrlBlk, _D2> __hold2(__alloc2.allocate(1), _D2(__alloc2, 1));
    ::new(__hold2.get()) _CntrlBlk(__alloc2, __a0, __a1);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = __hold2.release();
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

template<class _Tp>
template<class _A0, class _A1, class _A2>
shared_ptr<_Tp>
shared_ptr<_Tp>::make_shared(_A0& __a0, _A1& __a1, _A2& __a2)
{
    typedef __shared_ptr_emplace<_Tp, allocator<_Tp> > _CntrlBlk;
    typedef allocator<_CntrlBlk> _Alloc2;
    typedef __allocator_destructor<_Alloc2> _D2;
    _Alloc2 __alloc2;
    unique_ptr<_CntrlBlk, _D2> __hold2(__alloc2.allocate(1), _D2(__alloc2, 1));
    ::new(__hold2.get()) _CntrlBlk(__alloc2, __a0, __a1, __a2);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = __hold2.release();
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

template<class _Tp>
template<class _Alloc>
shared_ptr<_Tp>
shared_ptr<_Tp>::allocate_shared(const _Alloc& __a)
{
    typedef __shared_ptr_emplace<_Tp, _Alloc> _CntrlBlk;
    typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _Alloc2;
    typedef __allocator_destructor<_Alloc2> _D2;
    _Alloc2 __alloc2(__a);
    unique_ptr<_CntrlBlk, _D2> __hold2(__alloc2.allocate(1), _D2(__alloc2, 1));
    ::new(static_cast<void*>(_VSTD::addressof(*__hold2.get())))
        _CntrlBlk(__a);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = _VSTD::addressof(*__hold2.release());
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

template<class _Tp>
template<class _Alloc, class _A0>
shared_ptr<_Tp>
shared_ptr<_Tp>::allocate_shared(const _Alloc& __a, _A0& __a0)
{
    typedef __shared_ptr_emplace<_Tp, _Alloc> _CntrlBlk;
    typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _Alloc2;
    typedef __allocator_destructor<_Alloc2> _D2;
    _Alloc2 __alloc2(__a);
    unique_ptr<_CntrlBlk, _D2> __hold2(__alloc2.allocate(1), _D2(__alloc2, 1));
    ::new(static_cast<void*>(_VSTD::addressof(*__hold2.get())))
        _CntrlBlk(__a, __a0);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = _VSTD::addressof(*__hold2.release());
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

template<class _Tp>
template<class _Alloc, class _A0, class _A1>
shared_ptr<_Tp>
shared_ptr<_Tp>::allocate_shared(const _Alloc& __a, _A0& __a0, _A1& __a1)
{
    typedef __shared_ptr_emplace<_Tp, _Alloc> _CntrlBlk;
    typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _Alloc2;
    typedef __allocator_destructor<_Alloc2> _D2;
    _Alloc2 __alloc2(__a);
    unique_ptr<_CntrlBlk, _D2> __hold2(__alloc2.allocate(1), _D2(__alloc2, 1));
    ::new(static_cast<void*>(_VSTD::addressof(*__hold2.get())))
        _CntrlBlk(__a, __a0, __a1);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = _VSTD::addressof(*__hold2.release());
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

template<class _Tp>
template<class _Alloc, class _A0, class _A1, class _A2>
shared_ptr<_Tp>
shared_ptr<_Tp>::allocate_shared(const _Alloc& __a, _A0& __a0, _A1& __a1, _A2& __a2)
{
    typedef __shared_ptr_emplace<_Tp, _Alloc> _CntrlBlk;
    typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _Alloc2;
    typedef __allocator_destructor<_Alloc2> _D2;
    _Alloc2 __alloc2(__a);
    unique_ptr<_CntrlBlk, _D2> __hold2(__alloc2.allocate(1), _D2(__alloc2, 1));
    ::new(static_cast<void*>(_VSTD::addressof(*__hold2.get())))
        _CntrlBlk(__a, __a0, __a1, __a2);
    shared_ptr<_Tp> __r;
    __r.__ptr_ = __hold2.get()->get();
    __r.__cntrl_ = _VSTD::addressof(*__hold2.release());
    __r.__enable_weak_this(__r.__ptr_);
    return __r;
}

#endif  // _LIBCPP_HAS_NO_VARIADICS

template<class _Tp>
shared_ptr<_Tp>::~shared_ptr()
{
    if (__cntrl_)
        __cntrl_->__release_shared();
}

template<class _Tp>
inline
shared_ptr<_Tp>&
shared_ptr<_Tp>::operator=(const shared_ptr& __r) _NOEXCEPT
{
    shared_ptr(__r).swap(*this);
    return *this;
}

template<class _Tp>
template<class _Yp>
inline
typename enable_if
<
    is_convertible<_Yp*, _Tp*>::value,
    shared_ptr<_Tp>&
>::type
shared_ptr<_Tp>::operator=(const shared_ptr<_Yp>& __r) _NOEXCEPT
{
    shared_ptr(__r).swap(*this);
    return *this;
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
inline
shared_ptr<_Tp>&
shared_ptr<_Tp>::operator=(shared_ptr&& __r) _NOEXCEPT
{
    shared_ptr(_VSTD::move(__r)).swap(*this);
    return *this;
}

template<class _Tp>
template<class _Yp>
inline
typename enable_if
<
    is_convertible<_Yp*, _Tp*>::value,
    shared_ptr<_Tp>&
>::type
shared_ptr<_Tp>::operator=(shared_ptr<_Yp>&& __r)
{
    shared_ptr(_VSTD::move(__r)).swap(*this);
    return *this;
}

template<class _Tp>
template<class _Yp>
inline
typename enable_if
<
    !is_array<_Yp>::value &&
    is_convertible<_Yp*, _Tp*>::value,
    shared_ptr<_Tp>
>::type&
shared_ptr<_Tp>::operator=(auto_ptr<_Yp>&& __r)
{
    shared_ptr(_VSTD::move(__r)).swap(*this);
    return *this;
}

template<class _Tp>
template <class _Yp, class _Dp>
inline
typename enable_if
<
    !is_array<_Yp>::value &&
    is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, _Tp*>::value,
    shared_ptr<_Tp>&
>::type
shared_ptr<_Tp>::operator=(unique_ptr<_Yp, _Dp>&& __r)
{
    shared_ptr(_VSTD::move(__r)).swap(*this);
    return *this;
}

#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
template<class _Yp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    !is_array<_Yp>::value &&
    is_convertible<_Yp*, _Tp*>::value,
    shared_ptr<_Tp>&
>::type
shared_ptr<_Tp>::operator=(auto_ptr<_Yp> __r)
{
    shared_ptr(__r).swap(*this);
    return *this;
}

template<class _Tp>
template <class _Yp, class _Dp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    !is_array<_Yp>::value &&
    is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, _Tp*>::value,
    shared_ptr<_Tp>&
>::type
shared_ptr<_Tp>::operator=(unique_ptr<_Yp, _Dp> __r)
{
    shared_ptr(_VSTD::move(__r)).swap(*this);
    return *this;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
inline
void
shared_ptr<_Tp>::swap(shared_ptr& __r) _NOEXCEPT
{
    _VSTD::swap(__ptr_, __r.__ptr_);
    _VSTD::swap(__cntrl_, __r.__cntrl_);
}

template<class _Tp>
inline
void
shared_ptr<_Tp>::reset() _NOEXCEPT
{
    shared_ptr().swap(*this);
}

template<class _Tp>
template<class _Yp>
inline
typename enable_if
<
    is_convertible<_Yp*, _Tp*>::value,
    void
>::type
shared_ptr<_Tp>::reset(_Yp* __p)
{
    shared_ptr(__p).swap(*this);
}

template<class _Tp>
template<class _Yp, class _Dp>
inline
typename enable_if
<
    is_convertible<_Yp*, _Tp*>::value,
    void
>::type
shared_ptr<_Tp>::reset(_Yp* __p, _Dp __d)
{
    shared_ptr(__p, __d).swap(*this);
}

template<class _Tp>
template<class _Yp, class _Dp, class _Alloc>
inline
typename enable_if
<
    is_convertible<_Yp*, _Tp*>::value,
    void
>::type
shared_ptr<_Tp>::reset(_Yp* __p, _Dp __d, _Alloc __a)
{
    shared_ptr(__p, __d, __a).swap(*this);
}

#ifndef _LIBCPP_HAS_NO_VARIADICS

template<class _Tp, class ..._Args>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    !is_array<_Tp>::value,
    shared_ptr<_Tp>
>::type
make_shared(_Args&& ...__args)
{
    return shared_ptr<_Tp>::make_shared(_VSTD::forward<_Args>(__args)...);
}

template<class _Tp, class _Alloc, class ..._Args>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    !is_array<_Tp>::value,
    shared_ptr<_Tp>
>::type
allocate_shared(const _Alloc& __a, _Args&& ...__args)
{
    return shared_ptr<_Tp>::allocate_shared(__a, _VSTD::forward<_Args>(__args)...);
}

#else  // _LIBCPP_HAS_NO_VARIADICS

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
make_shared()
{
    return shared_ptr<_Tp>::make_shared();
}

template<class _Tp, class _A0>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
make_shared(_A0& __a0)
{
    return shared_ptr<_Tp>::make_shared(__a0);
}

template<class _Tp, class _A0, class _A1>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
make_shared(_A0& __a0, _A1& __a1)
{
    return shared_ptr<_Tp>::make_shared(__a0, __a1);
}

template<class _Tp, class _A0, class _A1, class _A2>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
make_shared(_A0& __a0, _A1& __a1, _A2& __a2)
{
    return shared_ptr<_Tp>::make_shared(__a0, __a1, __a2);
}

template<class _Tp, class _Alloc>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
allocate_shared(const _Alloc& __a)
{
    return shared_ptr<_Tp>::allocate_shared(__a);
}

template<class _Tp, class _Alloc, class _A0>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
allocate_shared(const _Alloc& __a, _A0& __a0)
{
    return shared_ptr<_Tp>::allocate_shared(__a, __a0);
}

template<class _Tp, class _Alloc, class _A0, class _A1>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
allocate_shared(const _Alloc& __a, _A0& __a0, _A1& __a1)
{
    return shared_ptr<_Tp>::allocate_shared(__a, __a0, __a1);
}

template<class _Tp, class _Alloc, class _A0, class _A1, class _A2>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
allocate_shared(const _Alloc& __a, _A0& __a0, _A1& __a1, _A2& __a2)
{
    return shared_ptr<_Tp>::allocate_shared(__a, __a0, __a1, __a2);
}

#endif  // _LIBCPP_HAS_NO_VARIADICS

template<class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(const shared_ptr<_Tp>& __x, const shared_ptr<_Up>& __y) _NOEXCEPT
{
    return __x.get() == __y.get();
}

template<class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(const shared_ptr<_Tp>& __x, const shared_ptr<_Up>& __y) _NOEXCEPT
{
    return !(__x == __y);
}

template<class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<(const shared_ptr<_Tp>& __x, const shared_ptr<_Up>& __y) _NOEXCEPT
{
    typedef typename common_type<_Tp*, _Up*>::type _Vp;
    return less<_Vp>()(__x.get(), __y.get());
}

template<class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>(const shared_ptr<_Tp>& __x, const shared_ptr<_Up>& __y) _NOEXCEPT
{
    return __y < __x;
}

template<class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(const shared_ptr<_Tp>& __x, const shared_ptr<_Up>& __y) _NOEXCEPT
{
    return !(__y < __x);
}

template<class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(const shared_ptr<_Tp>& __x, const shared_ptr<_Up>& __y) _NOEXCEPT
{
    return !(__x < __y);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(const shared_ptr<_Tp>& __x, nullptr_t) _NOEXCEPT
{
    return !__x;
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(nullptr_t, const shared_ptr<_Tp>& __x) _NOEXCEPT
{
    return !__x;
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(const shared_ptr<_Tp>& __x, nullptr_t) _NOEXCEPT
{
    return static_cast<bool>(__x);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(nullptr_t, const shared_ptr<_Tp>& __x) _NOEXCEPT
{
    return static_cast<bool>(__x);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<(const shared_ptr<_Tp>& __x, nullptr_t) _NOEXCEPT
{
    return less<_Tp*>()(__x.get(), nullptr);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<(nullptr_t, const shared_ptr<_Tp>& __x) _NOEXCEPT
{
    return less<_Tp*>()(nullptr, __x.get());
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>(const shared_ptr<_Tp>& __x, nullptr_t) _NOEXCEPT
{
    return nullptr < __x;
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>(nullptr_t, const shared_ptr<_Tp>& __x) _NOEXCEPT
{
    return __x < nullptr;
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(const shared_ptr<_Tp>& __x, nullptr_t) _NOEXCEPT
{
    return !(nullptr < __x);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(nullptr_t, const shared_ptr<_Tp>& __x) _NOEXCEPT
{
    return !(__x < nullptr);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(const shared_ptr<_Tp>& __x, nullptr_t) _NOEXCEPT
{
    return !(__x < nullptr);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(nullptr_t, const shared_ptr<_Tp>& __x) _NOEXCEPT
{
    return !(nullptr < __x);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(shared_ptr<_Tp>& __x, shared_ptr<_Tp>& __y) _NOEXCEPT
{
    __x.swap(__y);
}

template<class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    !is_array<_Tp>::value && !is_array<_Up>::value,
    shared_ptr<_Tp>
>::type
static_pointer_cast(const shared_ptr<_Up>& __r) _NOEXCEPT
{
    return shared_ptr<_Tp>(__r, static_cast<_Tp*>(__r.get()));
}

template<class _Tp, class _Up>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    !is_array<_Tp>::value && !is_array<_Up>::value,
    shared_ptr<_Tp>
>::type
dynamic_pointer_cast(const shared_ptr<_Up>& __r) _NOEXCEPT
{
    _Tp* __p = dynamic_cast<_Tp*>(__r.get());
    return __p ? shared_ptr<_Tp>(__r, __p) : shared_ptr<_Tp>();
}

template<class _Tp, class _Up>
typename enable_if
<
    is_array<_Tp>::value == is_array<_Up>::value,
    shared_ptr<_Tp>
>::type
const_pointer_cast(const shared_ptr<_Up>& __r) _NOEXCEPT
{
    typedef typename remove_extent<_Tp>::type _RTp;
    return shared_ptr<_Tp>(__r, const_cast<_RTp*>(__r.get()));
}

#ifndef _LIBCPP_NO_RTTI

template<class _Dp, class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Dp*
get_deleter(const shared_ptr<_Tp>& __p) _NOEXCEPT
{
    return __p.template __get_deleter<_Dp>();
}

#endif  // _LIBCPP_NO_RTTI

template<class _Tp>
class _LIBCPP_TYPE_VIS_ONLY weak_ptr
{
public:
    typedef _Tp element_type;
private:
    element_type*        __ptr_;
    __shared_weak_count* __cntrl_;

public:
    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_CONSTEXPR weak_ptr() _NOEXCEPT;
    template<class _Yp> _LIBCPP_INLINE_VISIBILITY weak_ptr(shared_ptr<_Yp> const& __r,
                   typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat*>::type = 0)
                        _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    weak_ptr(weak_ptr const& __r) _NOEXCEPT;
    template<class _Yp> _LIBCPP_INLINE_VISIBILITY weak_ptr(weak_ptr<_Yp> const& __r,
                   typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat*>::type = 0)
                         _NOEXCEPT;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    weak_ptr(weak_ptr&& __r) _NOEXCEPT;
    template<class _Yp> _LIBCPP_INLINE_VISIBILITY weak_ptr(weak_ptr<_Yp>&& __r,
                   typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat*>::type = 0)
                         _NOEXCEPT;
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~weak_ptr();

    _LIBCPP_INLINE_VISIBILITY
    weak_ptr& operator=(weak_ptr const& __r) _NOEXCEPT;
    template<class _Yp>
        typename enable_if
        <
            is_convertible<_Yp*, element_type*>::value,
            weak_ptr&
        >::type
        _LIBCPP_INLINE_VISIBILITY
        operator=(weak_ptr<_Yp> const& __r) _NOEXCEPT;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    _LIBCPP_INLINE_VISIBILITY
    weak_ptr& operator=(weak_ptr&& __r) _NOEXCEPT;
    template<class _Yp>
        typename enable_if
        <
            is_convertible<_Yp*, element_type*>::value,
            weak_ptr&
        >::type
        _LIBCPP_INLINE_VISIBILITY
        operator=(weak_ptr<_Yp>&& __r) _NOEXCEPT;

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    template<class _Yp>
        typename enable_if
        <
            is_convertible<_Yp*, element_type*>::value,
            weak_ptr&
        >::type
        _LIBCPP_INLINE_VISIBILITY
        operator=(shared_ptr<_Yp> const& __r) _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    void swap(weak_ptr& __r) _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    void reset() _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    long use_count() const _NOEXCEPT
        {return __cntrl_ ? __cntrl_->use_count() : 0;}
    _LIBCPP_INLINE_VISIBILITY
    bool expired() const _NOEXCEPT
        {return __cntrl_ == 0 || __cntrl_->use_count() == 0;}
    shared_ptr<_Tp> lock() const _NOEXCEPT;
    template<class _Up>
        _LIBCPP_INLINE_VISIBILITY
        bool owner_before(const shared_ptr<_Up>& __r) const
        {return __cntrl_ < __r.__cntrl_;}
    template<class _Up>
        _LIBCPP_INLINE_VISIBILITY
        bool owner_before(const weak_ptr<_Up>& __r) const
        {return __cntrl_ < __r.__cntrl_;}

    template <class _Up> friend class _LIBCPP_TYPE_VIS_ONLY weak_ptr;
    template <class _Up> friend class _LIBCPP_TYPE_VIS_ONLY shared_ptr;
};

template<class _Tp>
inline
_LIBCPP_CONSTEXPR
weak_ptr<_Tp>::weak_ptr() _NOEXCEPT
    : __ptr_(0),
      __cntrl_(0)
{
}

template<class _Tp>
inline
weak_ptr<_Tp>::weak_ptr(weak_ptr const& __r) _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    if (__cntrl_)
        __cntrl_->__add_weak();
}

template<class _Tp>
template<class _Yp>
inline
weak_ptr<_Tp>::weak_ptr(shared_ptr<_Yp> const& __r,
                        typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat*>::type)
                         _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    if (__cntrl_)
        __cntrl_->__add_weak();
}

template<class _Tp>
template<class _Yp>
inline
weak_ptr<_Tp>::weak_ptr(weak_ptr<_Yp> const& __r,
                        typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat*>::type)
         _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    if (__cntrl_)
        __cntrl_->__add_weak();
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
inline
weak_ptr<_Tp>::weak_ptr(weak_ptr&& __r) _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    __r.__ptr_ = 0;
    __r.__cntrl_ = 0;
}

template<class _Tp>
template<class _Yp>
inline
weak_ptr<_Tp>::weak_ptr(weak_ptr<_Yp>&& __r,
                        typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat*>::type)
         _NOEXCEPT
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_)
{
    __r.__ptr_ = 0;
    __r.__cntrl_ = 0;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
weak_ptr<_Tp>::~weak_ptr()
{
    if (__cntrl_)
        __cntrl_->__release_weak();
}

template<class _Tp>
inline
weak_ptr<_Tp>&
weak_ptr<_Tp>::operator=(weak_ptr const& __r) _NOEXCEPT
{
    weak_ptr(__r).swap(*this);
    return *this;
}

template<class _Tp>
template<class _Yp>
inline
typename enable_if
<
    is_convertible<_Yp*, _Tp*>::value,
    weak_ptr<_Tp>&
>::type
weak_ptr<_Tp>::operator=(weak_ptr<_Yp> const& __r) _NOEXCEPT
{
    weak_ptr(__r).swap(*this);
    return *this;
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
inline
weak_ptr<_Tp>&
weak_ptr<_Tp>::operator=(weak_ptr&& __r) _NOEXCEPT
{
    weak_ptr(_VSTD::move(__r)).swap(*this);
    return *this;
}

template<class _Tp>
template<class _Yp>
inline
typename enable_if
<
    is_convertible<_Yp*, _Tp*>::value,
    weak_ptr<_Tp>&
>::type
weak_ptr<_Tp>::operator=(weak_ptr<_Yp>&& __r) _NOEXCEPT
{
    weak_ptr(_VSTD::move(__r)).swap(*this);
    return *this;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template<class _Tp>
template<class _Yp>
inline
typename enable_if
<
    is_convertible<_Yp*, _Tp*>::value,
    weak_ptr<_Tp>&
>::type
weak_ptr<_Tp>::operator=(shared_ptr<_Yp> const& __r) _NOEXCEPT
{
    weak_ptr(__r).swap(*this);
    return *this;
}

template<class _Tp>
inline
void
weak_ptr<_Tp>::swap(weak_ptr& __r) _NOEXCEPT
{
    _VSTD::swap(__ptr_, __r.__ptr_);
    _VSTD::swap(__cntrl_, __r.__cntrl_);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(weak_ptr<_Tp>& __x, weak_ptr<_Tp>& __y) _NOEXCEPT
{
    __x.swap(__y);
}

template<class _Tp>
inline
void
weak_ptr<_Tp>::reset() _NOEXCEPT
{
    weak_ptr().swap(*this);
}

template<class _Tp>
template<class _Yp>
shared_ptr<_Tp>::shared_ptr(const weak_ptr<_Yp>& __r,
                            typename enable_if<is_convertible<_Yp*, _Tp*>::value, __nat>::type)
    : __ptr_(__r.__ptr_),
      __cntrl_(__r.__cntrl_ ? __r.__cntrl_->lock() : __r.__cntrl_)
{
    if (__cntrl_ == 0)
#ifndef _LIBCPP_NO_EXCEPTIONS
        throw bad_weak_ptr();
#else
        assert(!"bad_weak_ptr");
#endif
}

template<class _Tp>
shared_ptr<_Tp>
weak_ptr<_Tp>::lock() const _NOEXCEPT
{
    shared_ptr<_Tp> __r;
    __r.__cntrl_ = __cntrl_ ? __cntrl_->lock() : __cntrl_;
    if (__r.__cntrl_)
        __r.__ptr_ = __ptr_;
    return __r;
}

#if _LIBCPP_STD_VER > 14
template <class _Tp = void> struct owner_less;
#else
template <class _Tp> struct owner_less;
#endif

template <class _Tp>
struct _LIBCPP_TYPE_VIS_ONLY owner_less<shared_ptr<_Tp> >
    : binary_function<shared_ptr<_Tp>, shared_ptr<_Tp>, bool>
{
    typedef bool result_type;
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(shared_ptr<_Tp> const& __x, shared_ptr<_Tp> const& __y) const
        {return __x.owner_before(__y);}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(shared_ptr<_Tp> const& __x,   weak_ptr<_Tp> const& __y) const
        {return __x.owner_before(__y);}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(  weak_ptr<_Tp> const& __x, shared_ptr<_Tp> const& __y) const
        {return __x.owner_before(__y);}
};

template <class _Tp>
struct _LIBCPP_TYPE_VIS_ONLY owner_less<weak_ptr<_Tp> >
    : binary_function<weak_ptr<_Tp>, weak_ptr<_Tp>, bool>
{
    typedef bool result_type;
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(  weak_ptr<_Tp> const& __x,   weak_ptr<_Tp> const& __y) const
        {return __x.owner_before(__y);}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(shared_ptr<_Tp> const& __x,   weak_ptr<_Tp> const& __y) const
        {return __x.owner_before(__y);}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(  weak_ptr<_Tp> const& __x, shared_ptr<_Tp> const& __y) const
        {return __x.owner_before(__y);}
};

#if _LIBCPP_STD_VER > 14
template <>
struct _LIBCPP_TYPE_VIS_ONLY owner_less<void>
{
    template <class _Tp, class _Up>
    _LIBCPP_INLINE_VISIBILITY
    bool operator()( shared_ptr<_Tp> const& __x, shared_ptr<_Up> const& __y) const
        {return __x.owner_before(__y);}
    template <class _Tp, class _Up>
    _LIBCPP_INLINE_VISIBILITY
    bool operator()( shared_ptr<_Tp> const& __x,  weak_ptr<_Up> const& __y) const
        {return __x.owner_before(__y);}
    template <class _Tp, class _Up>
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(   weak_ptr<_Tp> const& __x, shared_ptr<_Up> const& __y) const
        {return __x.owner_before(__y);}
    template <class _Tp, class _Up>
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(   weak_ptr<_Tp> const& __x,   weak_ptr<_Up> const& __y) const
        {return __x.owner_before(__y);}
    typedef void is_transparent;
};
#endif

template<class _Tp>
class _LIBCPP_TYPE_VIS_ONLY enable_shared_from_this
{
    mutable weak_ptr<_Tp> __weak_this_;
protected:
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR
    enable_shared_from_this() _NOEXCEPT {}
    _LIBCPP_INLINE_VISIBILITY
    enable_shared_from_this(enable_shared_from_this const&) _NOEXCEPT {}
    _LIBCPP_INLINE_VISIBILITY
    enable_shared_from_this& operator=(enable_shared_from_this const&) _NOEXCEPT
        {return *this;}
    _LIBCPP_INLINE_VISIBILITY
    ~enable_shared_from_this() {}
public:
    _LIBCPP_INLINE_VISIBILITY
    shared_ptr<_Tp> shared_from_this()
        {return shared_ptr<_Tp>(__weak_this_);}
    _LIBCPP_INLINE_VISIBILITY
    shared_ptr<_Tp const> shared_from_this() const
        {return shared_ptr<const _Tp>(__weak_this_);}

    template <class _Up> friend class shared_ptr;
};

template <class _Tp>
struct _LIBCPP_TYPE_VIS_ONLY hash<shared_ptr<_Tp> >
{
    typedef shared_ptr<_Tp>      argument_type;
    typedef size_t               result_type;
    _LIBCPP_INLINE_VISIBILITY
    result_type operator()(const argument_type& __ptr) const _NOEXCEPT
    {
        return hash<_Tp*>()(__ptr.get());
    }
};

template<class _CharT, class _Traits, class _Yp>
inline _LIBCPP_INLINE_VISIBILITY
basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, shared_ptr<_Yp> const& __p);

// TODO(EricWF): Enable this for both Clang and GCC. Currently it is only
// enabled with clang.
#if defined(_LIBCPP_HAS_C_ATOMIC_IMP) && !defined(_LIBCPP_HAS_NO_THREADS)

class _LIBCPP_TYPE_VIS __sp_mut
{
    void* __lx;
public:
    void lock() _NOEXCEPT;
    void unlock() _NOEXCEPT;

private:
    _LIBCPP_CONSTEXPR __sp_mut(void*) _NOEXCEPT;
    __sp_mut(const __sp_mut&);
    __sp_mut& operator=(const __sp_mut&);

    friend _LIBCPP_FUNC_VIS __sp_mut& __get_sp_mut(const void*);
};

_LIBCPP_FUNC_VIS __sp_mut& __get_sp_mut(const void*);

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_is_lock_free(const shared_ptr<_Tp>*)
{
    return false;
}

template <class _Tp>
shared_ptr<_Tp>
atomic_load(const shared_ptr<_Tp>* __p)
{
    __sp_mut& __m = __get_sp_mut(__p);
    __m.lock();
    shared_ptr<_Tp> __q = *__p;
    __m.unlock();
    return __q;
}
  
template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
atomic_load_explicit(const shared_ptr<_Tp>* __p, memory_order)
{
    return atomic_load(__p);
}

template <class _Tp>
void
atomic_store(shared_ptr<_Tp>* __p, shared_ptr<_Tp> __r)
{
    __sp_mut& __m = __get_sp_mut(__p);
    __m.lock();
    __p->swap(__r);
    __m.unlock();
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
atomic_store_explicit(shared_ptr<_Tp>* __p, shared_ptr<_Tp> __r, memory_order)
{
    atomic_store(__p, __r);
}

template <class _Tp>
shared_ptr<_Tp>
atomic_exchange(shared_ptr<_Tp>* __p, shared_ptr<_Tp> __r)
{
    __sp_mut& __m = __get_sp_mut(__p);
    __m.lock();
    __p->swap(__r);
    __m.unlock();
    return __r;
}
  
template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
shared_ptr<_Tp>
atomic_exchange_explicit(shared_ptr<_Tp>* __p, shared_ptr<_Tp> __r, memory_order)
{
    return atomic_exchange(__p, __r);
}

template <class _Tp>
bool
atomic_compare_exchange_strong(shared_ptr<_Tp>* __p, shared_ptr<_Tp>* __v, shared_ptr<_Tp> __w)
{
    shared_ptr<_Tp> __temp;
    __sp_mut& __m = __get_sp_mut(__p);
    __m.lock();
    if (__p->__owner_equivalent(*__v))
    {
        _VSTD::swap(__temp, *__p);
        *__p = __w;
        __m.unlock();
        return true;
    }
    _VSTD::swap(__temp, *__v);
    *__v = *__p;
    __m.unlock();
    return false;
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_weak(shared_ptr<_Tp>* __p, shared_ptr<_Tp>* __v, shared_ptr<_Tp> __w)
{
    return atomic_compare_exchange_strong(__p, __v, __w);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_strong_explicit(shared_ptr<_Tp>* __p, shared_ptr<_Tp>* __v,
                                        shared_ptr<_Tp> __w, memory_order, memory_order)
{
    return atomic_compare_exchange_strong(__p, __v, __w);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_weak_explicit(shared_ptr<_Tp>* __p, shared_ptr<_Tp>* __v,
                                      shared_ptr<_Tp> __w, memory_order, memory_order)
{
    return atomic_compare_exchange_weak(__p, __v, __w);
}

#endif  // defined(_LIBCPP_HAS_C_ATOMIC_IMP) && !defined(_LIBCPP_HAS_NO_THREADS)

//enum class
struct _LIBCPP_TYPE_VIS pointer_safety
{
    enum __lx
    {
        relaxed,
        preferred,
        strict
    };

    __lx __v_;

    _LIBCPP_INLINE_VISIBILITY
    pointer_safety(__lx __v) : __v_(__v) {}
    _LIBCPP_INLINE_VISIBILITY
    operator int() const {return __v_;}
};

_LIBCPP_FUNC_VIS void declare_reachable(void* __p);
_LIBCPP_FUNC_VIS void declare_no_pointers(char* __p, size_t __n);
_LIBCPP_FUNC_VIS void undeclare_no_pointers(char* __p, size_t __n);
_LIBCPP_FUNC_VIS pointer_safety get_pointer_safety() _NOEXCEPT;
_LIBCPP_FUNC_VIS void* __undeclare_reachable(void* __p);

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
undeclare_reachable(_Tp* __p)
{
    return static_cast<_Tp*>(__undeclare_reachable(__p));
}

_LIBCPP_FUNC_VIS void* align(size_t __align, size_t __sz, void*& __ptr, size_t& __space);

// --- Helper for container swap --
template <typename _Alloc>
inline _LIBCPP_INLINE_VISIBILITY
void __swap_allocator(_Alloc & __a1, _Alloc & __a2)
#if _LIBCPP_STD_VER >= 14
    _NOEXCEPT
#else
    _NOEXCEPT_(__is_nothrow_swappable<_Alloc>::value)
#endif
{
    __swap_allocator(__a1, __a2, 
      integral_constant<bool, _VSTD::allocator_traits<_Alloc>::propagate_on_container_swap::value>());
}

template <typename _Alloc>
_LIBCPP_INLINE_VISIBILITY
void __swap_allocator(_Alloc & __a1, _Alloc & __a2, true_type)
#if _LIBCPP_STD_VER >= 14
    _NOEXCEPT
#else
    _NOEXCEPT_(__is_nothrow_swappable<_Alloc>::value)
#endif
{
    using _VSTD::swap;
    swap(__a1, __a2);
}

template <typename _Alloc>
inline _LIBCPP_INLINE_VISIBILITY
void __swap_allocator(_Alloc &, _Alloc &, false_type) _NOEXCEPT {}

template <typename _Alloc, typename _Traits=allocator_traits<_Alloc> >
struct __noexcept_move_assign_container : public integral_constant<bool, 
    _Traits::propagate_on_container_move_assignment::value
#if _LIBCPP_STD_VER > 14
        || _Traits::is_always_equal::value
#else
        && is_nothrow_move_assignable<_Alloc>::value
#endif
    > {};

_LIBCPP_END_NAMESPACE_STD

#endif  // _LIBCPP_MEMORY
