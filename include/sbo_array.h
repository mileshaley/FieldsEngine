//=====================================================================================================================
// MIT License
//
// Copyright (c) 2025 Cory Simonich
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//=====================================================================================================================

//=====================================================================================================================
//
// Small Buffer Optimized Array
//
//
// interface acts as a drop in std::vector replacement 
// BUT, for when you have an unknown amount of things, but you can maybe guess the upper bound
//
// you set the threshold and it uses stack space as long as count_ < threshold, then it goes to heap. 
// the "initial_size" is the size_threshold
//     b/c it is the size of the stack buffer, the container will not allocate upon construction
//     use reserve to force the allocation or push more than size_threshold items 
//
// it is useful in avoiding allocations, but wasteful in memory space (see union Storage {} )
//     it has some optimizations for plain data types vs non trivial types
//     initial testing shows comparable peformance to std::vector with improvements in small buffer situations
//
// I mainly use it for gameplay code where i need most of the time small containers of id's and other pod's 
// I also use it as the base container for a PriorityQueue 
//      this can make pathfinding for shorter paths/smaller graphs almost always on the stack
//
// Example: this code is "slideware" (not real code)
//
//      SboArray entities_to_process;
//      for (const u32 e : game.entities)
//      {
//          // we need to know now that this trait is active here
//          // the code in the loop may invalidate the flags, but we still need to process that state this frame
//          if(EntitySystem::HasFlag(e, SOME_ENTITY_FLAG & SOME_OTHER_ENTITY_FLAG)) { entities_to_process.push_back(e); }
//      
//          // ... a bunch of gameplay code that needs to get done before processing the entities
//          // ...
//      }
//
//      for(auto e : entities_to_process)
//      {
//          // now do the thing to those entities    
//          EntitySystem::DoTheThing(e);
//      }
// 
//
//
// currently, the allocator is just directly using malloc and free, with placement new as needed
//     constructors and destructors are called manually if it is not "plain_old_data_"
//
// Plain Old Data == trivially_copyable && 
//                   trivially_default_constructible && 
//                   trivially_destructible && 
//                   trivially_move_constructible
//
// Todo List
//         @todo:: continue basic usage via use in gameplay code and watch usage stats for trends 
//         @todo:: output benchmarking results to file, so i can process in matplotlib or something graphical
//         @todo:: support user provided allocators
//         @todo:: test usage as underlying storage type for other containers (maps, queues, heaps, etc)
//         @todo:: back port to earlier standards, c++17 and c++20 are working, will need mods to go earlier 
//     
//=====================================================================================================================


#ifndef SBOARRAY_H
#define SBOARRAY_H

// @todo:: see what kind of support can be ported in
#if __cplusplus == 202002L
    #define CPP_STANDARD 2020
#elif __cplusplus == 201703L
    #define CPP_STANDARD 2017
#elif __cplusplus == 201402L
    #define CPP_STANDARD 2014
#elif __cplusplus == 201103L
    #define CPP_STANDARD 2011
#elif __cplusplus == 199711L
    #define CPP_STANDARD 1998
#else
    #define CPP_STANDARD 1989
#endif

// compiles on clang but not gcc currently, didn't look into it much yet (may be something else tbh)
#if CPP_STANDARD > 2014
    #define if_constexpr if constexpr
#else
    #define if_constexpr if 
#endif

#include <initializer_list> // for EmplaceBack
#include <type_traits>      // trivially_...
#include <new>              // std::launder, c++17 removes some UB when using char buffers for object storage
#include <stdexcept>        // to provide exception safety for .at(index)
#include <cassert>          // assert
#include <cstring>          // memcpy, memmove

template <typename T, size_t size_threshold = 64>
class SboArray
{

//=====================================================================================================================
// Public Api
//=====================================================================================================================
public:

    // ctor/copy/move/dtor
    SboArray()                                          { ContainerConstructor(); }
    SboArray(size_t size)                               { ContainerConstructor_Size(size); }
    SboArray(size_t size, const T& value)               { ContainerConstructor_SizeValue(size, value); }
    SboArray(const SboArray& rhs)                       { ContainerConstructor_Copy(rhs); }
    SboArray(SboArray&& rhs)                            { ContainerConstructor_Move(std::move(rhs)); }
    SboArray(std::initializer_list<T> init)             { ContainerConstructor_List(init); }
    SboArray& operator=(const SboArray& rhs)            { return ContainerAssignment_Copy(rhs); }
    SboArray& operator=(SboArray&& rhs) noexcept        { return ContainerAssignment_Move(rhs); }
    ~SboArray()                                         { ContainerDestructor(); }
    
    // push/pop/grow/shrink
    template <typename... Args> 
    void emplace_back(Args&&... args)                   { EmplaceBack(std::forward<Args>(args)...); }
    void reserve(size_t new_cap)                        { Reserve(new_cap); }
    void shrink_to_fit()                                { ShrinkToFit(); }
    void push_back(const T& value)                      { PushBack_Copy(value); }
    void push_back(T&& value) noexcept                  { PushBack_Move(std::move(value)); }
    void pop_back() noexcept                            { PopBack(); }
    void clear() noexcept                               { CallDestructors(); count_ = 0; }

    // query
    bool empty() const noexcept                         { return count_ == 0; }
    size_t size() const noexcept                        { return count_; }
    size_t capacity() const noexcept                    { return capacity_; }
    inline bool using_stack_buffer() const noexcept     { return !using_heap_; }
    
    // accessors
    T* data() noexcept                                  { return data_ptr(); }
    T& front() noexcept                                 { assert(!empty()); return data_ptr()[0]; }
    T& back() noexcept                                  { assert(!empty()); return data_ptr()[count_ - 1]; }
    T& at(size_t i)                                     { return At(i); }
    T& operator[](size_t i) noexcept                    { assert(i< count_); return data_ptr()[i]; }
    
    const T* data() const noexcept                      { return data_ptr(); }
    const T& front() const noexcept                     { assert(!empty()); return data_ptr()[0]; }
    const T& back() const noexcept                      { assert(!empty()); return data_ptr()[count_ - 1]; }
    const T& at(size_t i) const                         { return At(i);  }
    const T& operator[](size_t i) const noexcept        { assert(i < count_); return data_ptr()[i]; }



    // iterators
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    using reference = T&;
    using const_reference = const T&;
    
    iterator begin() noexcept                           { return data_ptr(); }
    iterator end() noexcept                             { return data_ptr() + count_; }
    const_iterator begin() const noexcept               { return data_ptr(); }
    const_iterator end() const noexcept                 { return data_ptr() + count_; }

#if CPP_STANDARD > 2011
    constexpr const_iterator cbegin() const noexcept    { return data_ptr(); }
    constexpr const_iterator cend() const noexcept      { return data_ptr() + count_; }
#endif

    template <typename In> 
    iterator insert(iterator pos, In first, In last)    { return InsertRange(pos, first, last); }
    iterator erase(iterator pos)                        { return Erase(pos); }
    iterator erase(iterator first, iterator last)       { return EraseRange(first, last); }
    iterator insert(iterator pos, const T& value)       { return Insert(pos, value); }
    iterator insert(iterator pos, T&& value)            { return Insert(pos, std::move(value)); }

//=====================================================================================================================
// Underlying Data
//=====================================================================================================================
    
private:
    // Data is a union between a pre known sized array and a T*
    union Storage 
    {
        char stack_buffer[size_threshold * sizeof(T)] = {0};
        T* heap_ptr;
    };
    Storage storage_;
    size_t count_ = 0;
    size_t capacity_ = size_threshold;
    bool using_heap_ = false;
    T* cached_data_ptr_ = nullptr;
    static constexpr bool plain_old_data_ = std::is_trivially_copyable_v<T> &&
                                            std::is_trivially_default_constructible_v<T> &&
                                            std::is_trivially_destructible_v<T> &&
                                            std::is_trivially_move_constructible_v<T>;
                                     

    static_assert
    (   
        std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>, 
        "SboArray requires T to be either move-constructible or copy-constructible"
    );
    
    static_assert
    (
        std::is_destructible_v<T>,
        "SboArray requires T to be destructible"
    );
    
    static_assert
    (   
        std::is_nothrow_move_constructible_v<T> || plain_old_data_,
        "SboArray strongly recommends T to be nothrow move-constructible for exception safety during reallocation"
    );
    
    static_assert
    (
        !std::is_abstract_v<T>,
        "SboArray cannot be used with abstract class types"
    );
    
    static_assert
    (
        !std::is_reference_v<T>,
        "SboArray cannot be used with reference types"
    );
                                  
                                  
//=====================================================================================================================
// Implementation
//=====================================================================================================================
 
// Ctor's
private:
    inline void ContainerConstructor() 
    { 
        count_ = 0; 
        capacity_ = size_threshold; 
        UpdateDataPointer(); 
    }
    inline void ContainerConstructor_Size(size_t size) 
    { 
        count_ = 0; 
        capacity_ = ((size <= size_threshold) ? size_threshold : size);
        UpdateDataPointer();
        
        // default construct objects that require it
        if_constexpr(!std::is_trivially_default_constructible_v<T>)
        {
            for (size_t i = 0; i < size; ++i) { new (data_ptr() + i) T(); }
        } 
        count_ = size;
    } 
    inline void ContainerConstructor_SizeValue(size_t size, const T& value) 
    { 
        count_ = 0; 
        capacity_ = ((size <= size_threshold) ? size_threshold : size);
        UpdateDataPointer();
        for (size_t i = 0; i < size; ++i) { new (data_ptr() + i) T(value); }
        count_ = size;
    } 
    inline void ContainerConstructor_Copy(const SboArray& rhs)
    {
        count_ = (rhs.count_);
        capacity_ = (rhs.capacity_);
        using_heap_ = capacity_ > size_threshold;
        if (using_heap_) { storage_.heap_ptr = Malloc(capacity_); }
        UpdateDataPointer();
        CopyElements(data_ptr(), rhs.data_ptr(), count_);
        UpdateDataPointer();
    }
    inline void ContainerConstructor_Move(SboArray&& rhs) 
    {
        count_ = 0;
        capacity_ = rhs.capacity_;
        using_heap_ = rhs.using_heap_; 
        UpdateDataPointer();
        
        if (using_heap_) { storage_.heap_ptr = rhs.storage_.heap_ptr; }
        UpdateDataPointer();
        if (!using_heap_) { MoveElements(data_ptr(), rhs.data_ptr(), rhs.count_); }
        
        count_ = rhs.count_;
        
        rhs.count_ = 0;
        rhs.capacity_ = size_threshold; 
        rhs.using_heap_ = false;
        UpdateDataPointer();
    }
    inline void ContainerConstructor_List(std::initializer_list<T> init) 
    {
        count_ = (init.size());
        capacity_ = (init.size());
        using_heap_ = capacity_ > size_threshold;
        UpdateDataPointer();
        
        if (using_heap_) { storage_.heap_ptr = Malloc(capacity_); }
        size_t i = 0;
        for (const T& val : init) { Construct(data_ptr() + i, val); ++i; }
        UpdateDataPointer();
    }
    
    
    // copy-and-swap idiom
    inline SboArray& ContainerAssignment_Copy(const SboArray& rhs)
    {
        if (this != &rhs) 
        {
            SboArray temp(rhs);
            Swap(temp);
        }
        UpdateDataPointer();
        return *this;
    }

    inline SboArray& ContainerAssignment_Move(SboArray&& rhs) noexcept
    {
        if (this != &rhs) 
        {
            CallDestructors();
            
            if (using_heap_) { Free(storage_.heap_ptr); }
            
            storage_ = rhs.storage_;
            count_ = rhs.count_;
            capacity_ = rhs.capacity_;
            using_heap_ = rhs.using_heap_;
            
            rhs.storage_.heap_ptr = nullptr;
            rhs.count_ = 0;
            rhs.capacity_ = size_threshold;
            rhs.using_heap_ = false;
        }
        UpdateDataPointer();
        return *this;
    }

    inline void ContainerDestructor() 
    { 
        CallDestructors(); 
        if (using_heap_) { Free(storage_.heap_ptr); }
        UpdateDataPointer();
    }
    
// Access
    inline T* data_ptr() { return cached_data_ptr_; }
    inline const T* data_ptr() const { return cached_data_ptr_; }
    
    inline T& At(size_t i) 
    {
        if (i >= count_) throw std::out_of_range("SboArray::at index out of range");
        return data_ptr()[i];
    }
    inline const T& At(size_t i) const 
    {
        if (i >= count_) throw std::out_of_range("SboArray::at index out of range");
        return data_ptr()[i];
    }

          
// Mutate
    inline void Reserve(size_t new_cap) { if (new_cap > capacity_) { Resize(new_cap); } }
    inline void ShrinkToFit() { if (count_ < capacity_) { Resize(count_); } }
    inline void PushBack_Copy(const T& value) { CheckSize(); Construct(data_ptr() + count_, value); ++count_; }
    inline void PushBack_Move(T&& value) { CheckSize(); MoveConstruct(data_ptr() + count_, std::move(value)); ++count_; }
    inline void PopBack() noexcept { assert(count_ > 0); if_constexpr (!plain_old_data_) { data_ptr()[count_ - 1].~T(); } --count_; }
    
    template <typename... Args> 
    inline void EmplaceBack(Args&&... args) { CheckSize(); new (data_ptr() + count_) T(std::forward<Args>(args)...); ++count_; }
          
// Helper Functions
    T* Malloc(size_t n)
    {
        if_constexpr (plain_old_data_) { return static_cast<T*>(malloc(n * sizeof(T))); }
        else { return static_cast<T*>(::operator new(n * sizeof(T))); }
    }
    void Free(T* ptr)
    {
        if (!ptr) return;
        if_constexpr (plain_old_data_) { free(ptr); }
        else { ::operator delete(ptr); }
    }
    void CheckSize()
    {
        if (count_ == capacity_) 
        {   
            // @consider:: the user may want to tune the growth for memory constraints or faster growth
            constexpr float growth_factor = 2.0f;
            size_t new_cap = (capacity_ == 0) ? size_threshold : capacity_ * growth_factor;
            Resize(new_cap);
        }
    }
    
#if CPP_STANDARD > 2017
    void UpdateDataPointer() { cached_data_ptr_ = !using_heap_ ? std::launder(reinterpret_cast<T*>(storage_.stack_buffer)) : storage_.heap_ptr; }
#else
    void UpdateDataPointer() { cached_data_ptr_ = !using_heap_ ? (reinterpret_cast<T*>(storage_.stack_buffer)) : storage_.heap_ptr; }
#endif

    void Swap(SboArray& other) noexcept 
    {
        std::swap(storage_, other.storage_);
        std::swap(count_, other.count_);
        std::swap(capacity_, other.capacity_);
        std::swap(using_heap_, other.using_heap_);
        UpdateDataPointer();
        other.UpdateDataPointer();
    }
    
    
    // follows copy-and-swap idiom
    void Resize(size_t new_cap)
    {
        size_t new_capacity = std::max(new_cap, size_threshold);
        size_t number_of_elements_to_move = std::min(count_, new_capacity);
        bool will_use_heap = new_capacity > size_threshold;
        
        // already there
        if ((new_capacity == capacity_) && (will_use_heap == using_heap_)) { return; }
        
        UpdateDataPointer();
        T* new_data = nullptr;
        T* old_data = data_ptr();

        // copy
        {
            if(will_use_heap)
            {
                new_data = Malloc(new_capacity);    
            }
            else
            {
            #if CPP_STANDARD > 2017
                new_data = std::launder(reinterpret_cast<T*>(storage_.stack_buffer));
            #else
                new_data = (reinterpret_cast<T*>(storage_.stack_buffer));
            #endif
            }
            
            MoveElements(new_data, old_data, number_of_elements_to_move);
        }
                    
        // we got them all copied, now commit to the swap
        CallDestructors();
        if(using_heap_) { Free(old_data); }
        if(will_use_heap) { storage_.heap_ptr = new_data; }

        using_heap_ = will_use_heap;
        capacity_ = new_capacity;
        count_ = number_of_elements_to_move;
        UpdateDataPointer();
    }
        
    void Construct(T* dest, const T& val)
    {
        if_constexpr (plain_old_data_) { *dest = val; }
        else { new (dest) T(val); }
    }
    void MoveConstruct(T* dest, T&& val)
    {
        if_constexpr (plain_old_data_) { *dest = val; }
        else { new (dest) T(std::move(val)); }
    }
    
    void CopyElements(T* dest, const T* src, size_t n)
    {
        if_constexpr (plain_old_data_)
        {
            std::memcpy(dest, src, n * sizeof(T));
        }
        else 
        {
            for (size_t i = 0; i < n; ++i)
            {
                new (dest + i) T(src[i]);
            }
        }
    }
    void MoveElements(T* dest, T* src, size_t n)
    {
        if_constexpr (plain_old_data_)
        {
            std::memmove(dest, src, n * sizeof(T)); 
        }
        else
        {
            if (dest == src) return; // nothing to do
            
            for (size_t i = n; i > 0; --i) 
            {
                size_t index = i - 1;
                new (dest + index) T(std::move(src[index]));
                src[index].~T();
            }
            
        }
    }
    void CallDestructors()
    {
        if_constexpr (!plain_old_data_) 
        {
            UpdateDataPointer();
            T* ptr = data_ptr();
            for (size_t i = 0; i < count_; ++i)
            {
                ptr[i].~T();
            }
        }
    }

//=====================================================================================================================
// Iterator
//=====================================================================================================================
 
    iterator Erase(iterator pos) 
    {
        if (pos < begin() || pos >= end()) return end();
        
        if_constexpr (plain_old_data_)
        {
            size_t n = count_ - (pos - begin()) - 1;
            if (n > 0) { std::memmove(pos, pos + 1, n * sizeof(T)); }
        }
        else
        {   
            // move assignment here
            std::move(pos + 1, end(), pos);
            (end() - 1)->~T();
        }
        
        --count_;
        return pos;
    }
 
    iterator EraseRange(iterator first, iterator last) 
    {
        if (first < begin() || last > end() || first > last) { return end(); }
        if (first == last) { return first; }
        size_t n = last - first;
        iterator new_end = std::move(last, end(), first);
        
        // call destructors after the move
        if_constexpr (!plain_old_data_) { for(iterator it = new_end; it != end(); ++it) { it->~T(); } }        
        
        count_ -= n;
        return first;
    }
    
    template <typename Arg>
    iterator Insert(iterator pos, Arg&& arg) 
    {
        size_t index = pos - begin();
        if (count_ == capacity_) CheckSize();
        pos = begin() + index; 
        
        if_constexpr (plain_old_data_) { std::memmove(pos + 1, pos, (count_ - index) * sizeof(T)); } 
        else { std::move_backward(pos, end(), end() + 1); }
        
        Construct(pos, std::forward<Arg>(arg)); 
        ++count_;
        return pos;
    }

    template <typename InputIt>
    iterator InsertRange(iterator pos, InputIt first, InputIt last) 
    {
        size_t index = pos - begin();
        auto n = std::distance(first, last);
        if(n <= 0) { return pos; }
        
        if (count_ + n > capacity_) 
        {
            reserve(count_ + n);
            pos = begin() + index; 
        }
        
        if_constexpr (plain_old_data_) { std::memmove(pos + n, pos, (count_ - index) * sizeof(T)); } 
        else { std::move_backward(pos, end(), end() + n); }
        
        for (iterator it = pos; first != last; ++first, ++it) { Construct(it, *first); }  
        
        count_ += n;
        return pos;
    }
    
}; 


#endif // SBOARRAY_H
