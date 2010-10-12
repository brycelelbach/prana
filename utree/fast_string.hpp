/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_UTREE_DETAIL1)
#define BOOST_SPIRIT_UTREE_DETAIL1

#include <boost/type_traits/alignment_of.hpp>

    ///////////////////////////////////////////////////////////////////////////
    // Our POD fast string. This implementation is very primitive and is not
    // meant to be used stand-alone. This is the internal data representation
    // of strings in our utree. This is deliberately a POD to allow it to be
    // placed in a union. This POD fast string specifically utilizes
    // (sizeof(list) * alignment_of(list)) - (2 * sizeof(char)). In a 32 bit
    // system, this is 14 bytes. The two extra bytes are used by utree to store
    // management info.
    //
    // It is a const string (i.e. immutable). It stores the characters directly
    // if possible and only uses the heap if the string does not fit. Null
    // characters are allowed, making it suitable to encode raw binary. The
    // string length is encoded in the first byte if the string is placed in-situ,
    // else, the length plus a pointer to the string in the heap are stored.
    ///////////////////////////////////////////////////////////////////////////
    struct fast_string // Keep this a POD!
    {
        static std::size_t const
            buff_size = (sizeof(list) + boost::alignment_of<list>::value)
                / sizeof(char);

        static std::size_t const
            small_string_size = buff_size-sizeof(char);

        static std::size_t const
            max_string_len = small_string_size - 1;

        struct heap_store
        {
            char* str;
            std::size_t size;
        };

        union
        {
            char buff[buff_size];
            heap_store heap;
        };

        int get_type() const;
        void set_type(int t);
        bool is_heap_allocated() const;

        std::size_t size() const;
        char const* str() const;

        template <typename Iterator>
        void construct(Iterator f, Iterator l);

        void swap(fast_string& other);
        void free();
        void copy(fast_string const& other);

        char& info();
        char info() const;

        short tag() const;
        void tag(short tag);
    };

    inline char& fast_string::info()
    {
        return buff[small_string_size];
    }

    inline char fast_string::info() const
    {
        return buff[small_string_size];
    }

    inline int fast_string::get_type() const
    {
        return info() >> 1;
    }

    inline void fast_string::set_type(int t)
    {
        info() = (t << 1) | (info() & 1);
    }

    inline short fast_string::tag() const
    {
        // warning the tag is not allowed for fast_string!!! it's only
        // placed here to avoid excess padding.
        return (int(buff[small_string_size-2]) << 8) + buff[small_string_size-1];
    }

    inline void fast_string::tag(short tag)
    {
        // warning the tag is not allowed for fast_string!!! it's only
        // placed here to avoid excess padding.
        buff[small_string_size-2] = tag >> 8;
        buff[small_string_size-1] = tag & 0xff;
    }

    inline bool fast_string::is_heap_allocated() const
    {
        return info() & 1;
    }

    inline std::size_t fast_string::size() const
    {
        if (is_heap_allocated())
            return heap.size;
        else
            return max_string_len - buff[small_string_size - 1];
    }

    inline char const* fast_string::str() const
    {
        if (is_heap_allocated())
            return heap.str;
        else
            return buff;
    }

    template <typename Iterator>
    inline void fast_string::construct(Iterator f, Iterator l)
    {
        unsigned const size = l-f;
        char* str;
        if (size < small_string_size)
        {
            // if it fits, store it in-situ; small_string_size minus the length
            // of the string is placed in buff[small_string_size - 1]
            str = buff;
            buff[small_string_size - 1] = static_cast<char>(max_string_len - size);
            info() &= ~0x1;
        }
        else
        {
            // else, store it in the heap
            str = new char[size + 1]; // add one for the null char
            heap.str = str;
            heap.size = size;
            info() |= 0x1;
        }
        for (std::size_t i = 0; i != size; ++i)
        {
            *str++ = *f++;
        }
        *str = '\0'; // add the null char
    }

    inline void fast_string::swap(fast_string& other)
    {
        std::swap(*this, other);
    }

    inline void fast_string::free()
    {
        if (is_heap_allocated())
        {
            delete [] heap.str;
            heap.str = 0;
        }
    }

    inline void fast_string::copy(fast_string const& other)
    {
        construct(other.str(), other.str() + other.size());
    }

#endif
