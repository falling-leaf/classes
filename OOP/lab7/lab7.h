//use a linked list to compose a memory pool.

#ifndef LAB7_H
#define LAB7_H
#include <bits/stdc++.h>
#define INF 20000
using namespace std;

template <class T>
class alloc_mem_pool
{
public:
    //set the typedef
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;//an unsigned long long int
    typedef true_type propagate_on_container_move_assignment;
    template <class U>
    struct rebind
    {
        typedef alloc_mem_pool<U> other;
    };
    typedef true_type is_always_equal;

    alloc_mem_pool();
    ~alloc_mem_pool();

    pointer address(reference x)const noexcept;//return the address of x
    [[nodiscard]] pointer allocate(size_t n);//get one unit
    void deallocate(pointer p, size_t n);//recycle one unit
    //size_type max_size()const noexcept;

    template< class U, class... Args >
    void construct( U* p, Args&&... args )//construct for the system
    {
        new((void*)p)U(std::forward<Args>(args)...);
    }

    template<class U>
    void destroy(U* p)//destroy for the system
    {
        p->~U();
    }

private:

    struct unitblock//one unitblock contains one unit and a next pointer
    {
        T element[512];
        unitblock* next;//只在freelist中有用到
    };
    unitblock* freelist;//the free unitblock
    //对于realaddress和realend的类型考虑了很久
    //现在用链表做分配器，就要尽可能避免去做"new unitblock"，否则可能时间会更长
    //如果二者用T*的话，那么在deallocate函数就必须要再分配一个给next的指针，否则会影响到下一个地址中的数据
    //因此，必须做出牺牲，将申请的内存分配一部分给next，用户只用其中一部分
    unitblock* boxlist[INF];
    int boxindex;
    unitblock* realaddress;
    unitblock* realend;
    T* extralist[INF];
    int extraindex;
};

#endif