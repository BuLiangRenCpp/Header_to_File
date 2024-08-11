#pragma once
#include "assertions.h"

namespace htf
{

// * [0]: buffer   [1]: peek
// * 因为 原Token_stream::peek() 会将下一个 token 放入 buffer 中，如果 buffer
// 只能装一个，那么以下代码会报错
// *    peek();   putback(token);
// * 因此建立此类在逻辑上保留 buffer 只有一个，但是 不会发生上述报错
template <typename T> class Buffer
{
public:
    Buffer() : _buffer{}, _peek{} {}

public:
    // 只是清空 buffer
    void sync() { _buffer = T{}; }
    // 清空 buffer、peek
    void clear()
    {
        _buffer = T{};
        _peek   = T{};
    }
    /**
     * peek and get 不能改变 [0] [1] 的含义
     */
    // 返回的是 下一个 token，并不是 _buffer[1]
    T peek() { return (full()) ? _buffer : _peek; }
    T get()
    {
        T t;
        // * 将输出的token清空
        if (full()) {
            t       = _buffer;
            _buffer = T{};
        }
        else {
            t     = _peek;
            _peek = T{};
        }
        return t;
    }
    void putback(const T& t)
    {
        HTF_DEV_ASSERT_MESSAGE(!full(), "Buffer::putback(const T&): buffer is full"
            << "\nbuffer = " << _buffer.val << "  " << _buffer.line << ":" << _buffer.col
            << "\nnext   = " << t.val << "  " << t.line << ":" << t.col);
        _buffer = t;
    }
    void push_peek(const T& t)
    {
        HTF_DEV_ASSERT_MESSAGE(empty_peek(), "Buffer::push_peek: peek is full"
            << "\npeek = " << _peek.val << "  " << _peek.line << ":" << _peek.col
            << "\nnext = " << t.val << "  " << t.line << ":" << t.col);
        _peek = t;
    }
    // 实际的缓冲区[0]是否满了，用于 putback 判断缓冲区是否满了
    bool full() const { return !_buffer.empty(); }
    // [1]是否为空
    bool empty_peek() const { return _peek.empty(); }
    // [0][1] 是否都为空，用于查看是否有数据
    bool empty() const { return (!full()) && empty_peek(); }

private:
    /**
     * _buffer: 用于存放 putback (实际的缓冲区)
     * _peek: 用于存放 peek 刷入缓存区的token
     */
    T _buffer;
    T _peek;
};

}   // namespace htf