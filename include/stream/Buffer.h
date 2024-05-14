#pragma once
#include "Excep_dev.h"

namespace htf {
    namespace stream {
        template <typename T>
        // * [0]: buffer   [1]: peek
        // * 因为 peek() 会将下一个 token 放入 buffer 中，如果 buffer 只有一个，那么以下代码会报错
        // *    this->peek();   this->putback(token);
        class Buffer {
        public:
            Buffer() :_buffer{ std::vector<T>(2, T{}) } { }

            // 只是清空 buffer
            void sync() { _buffer[0] =  T{}; }

            // 清空 buffer、peek
            void clear() { _buffer[0] = T{}; _buffer[1] = T{}; }

            /**
             * peek and get 不能改变 [0] [1] 的含义
            */

            // 返回的是 下一个 token，并不是 _buffer[1]
            T peek() { return (this->full()) ? _buffer[0] : _buffer[1]; }

            T get()
            {
                T t;
                // * 将输出的token清空
                if (this->full()) {
                    t = _buffer[0];
                    _buffer[0] = T{};
                } else {
                    t =_buffer[1];
                    _buffer[1] = T{};
                }
                return t;
		    }

            void putback(const T& t)
            {
                if (this->full()) 
                    throw exception::Excep_dev{"Buffer::putback", "buffer is full"};
                _buffer[0] = t;
            }

            void push_peek(const T& t)
            {
                if (!this->empty_peek()) 
                    throw exception::Excep_dev{"Buffer::push_peek", "peek is full"};
                _buffer[1] = t;
            }

            // 实际的缓冲区[0]是否满了，用于 putback 判断缓冲区是否满了
            bool full() const
            {
                return !_buffer[0].empty();
            }

            // [1]是否为空
            bool empty_peek() const
            {
                return _buffer[1].empty();
            }

            // [0][1] 是否都为空，用于查看是否有数据
            bool empty() const 
            {
                return (!this->full()) && empty_peek();
            }

        private:
            /**
             * [0]: 用于存放 putback (实际的缓冲区)
             * [1]: 用于存放 peek 刷入缓存区的token
            */
           std::vector<T> _buffer;   // 
        };
    }
}