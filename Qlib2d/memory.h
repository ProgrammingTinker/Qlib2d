//
// Project: cliblisp
// Author: bajdcc
//

#ifndef CLIBLISP_MEMORY_H
#define CLIBLISP_MEMORY_H

#include "types.h"

namespace clib {
    // Ĭ�ϵ��ڴ�������
    template<size_t DefaultSize = 0x10000>
    class default_allocator {
    public:
        static const size_t DEFAULT_ALLOC_BLOCK_SIZE = DefaultSize;

        template<class T>
        T *__alloc() {
            return new T;
        }

        template<class T>
        T *__alloc_array(uint size) {
            return new T[size];
        }

        template<class T, class ... TArgs>
        T *__alloc_args(const TArgs &&... args) {
            return new T(std::forward(args)...);
        }

        template<class T, class ... TArgs>
        T *__alloc_array_args(uint size, const TArgs &&... args) {
            return new T[size];
        }

        template<class T>
        bool __free(T *t) {
            delete t;
            return true;
        }

        template<class T>
        bool __free_array(T *t) {
            delete[] t;
            return true;
        }
    };

    // ԭʼ�ڴ��
    template<class Allocator, size_t DefaultSize = Allocator::DEFAULT_ALLOC_BLOCK_SIZE>
    class legacy_memory_pool {
    public:
        // ��
        struct block {
            size_t size; // ���ݲ��ֵĴ�С
            uint flag;   // ����
            block *prev; // ǰָ��
            block *next; // ��ָ��
        };

        // �����
        enum block_flag {
            BLOCK_USING = 0,
            BLOCK_MARK = 1,
        };

        // ���Ԫ��Ϣ���ֵĴ�С
        static const size_t BLOCK_SIZE = sizeof(block);
        // ���С����
        static const uint BLOCK_SIZE_MASK = BLOCK_SIZE - 1;

    private:
        // �ڴ����ӿ�
        Allocator allocator;
        // ������ͷָ��
        block *block_head{nullptr};
        // ����ѭ��������ָ��
        block *block_current{nullptr};
        // ���п���
        size_t block_available_size{0};

        // ------------------------ //

        // ���С����
        static size_t block_align(size_t size) {
            if ((size & BLOCK_SIZE_MASK) == 0)
                return size / BLOCK_SIZE;
            return (size / BLOCK_SIZE) + 1;
        }

        // ���ʼ��
        static void block_init(block *blk, size_t size) {
            blk->size = size;
            blk->flag = 0;
            blk->prev = nullptr;
            blk->next = nullptr;
        }

        // ������
        static void block_connect(block *blk, block *new_blk) {
            new_blk->prev = blk;
            new_blk->next = blk->next;
            new_blk->next->prev = new_blk;
            blk->next = new_blk;
        }

        // ����ϲ�
        static size_t block_merge(block *blk, block *next, bool flag) {
            if (flag) { // prev(USING) - blk(TO FREE) - next(FREE)
                auto tmp = blk->size + 1;
                next->next->prev = blk;
                blk->size += next->size + 1;
                blk->next = next->next;
                return tmp;
            } else { // blk(FREE) - next(TO FREE) - next(USING)
                next->next->prev = blk;
                blk->size += next->size + 1;
                blk->next = next->next;
                return next->size + 1;
            }
        }

        // ����ϲ�
        static size_t block_merge(block *prev, block *blk, block *next) {
            next->next->prev = prev;
            prev->size += blk->size + next->size + 2;
            prev->next = next->next;
            return blk->size + 1;
        }

        // �����ò���
        static void block_set_flag(block *blk, block_flag flag, uint value) {
            if (value) {
                blk->flag |= 1 << flag;
            } else {
                blk->flag &= ~(1 << flag);
            }
        }

        // ���ȡ����
        static uint block_get_flag(block *blk, block_flag flag) {
            return (blk->flag & (1 << flag)) != 0 ? 1 : 0;
        }

        // ------------------------ //

        // �����ڴ��
        void _create() {
            block_head = allocator.template __alloc_array<block>(DEFAULT_ALLOC_BLOCK_SIZE);
            assert(block_head);
            _init();
        }

        // ��ʼ���ڴ��
        void _init() {
            block_available_size = DEFAULT_ALLOC_BLOCK_SIZE - 1;
            block_init(block_head, block_available_size);
            block_head->prev = block_head->next = block_head;
            block_current = block_head;
        }

        // �����ڴ��
        void _destroy() {
            allocator.__free_array(block_head);
        }

        // �����ڴ�
        void *_alloc(size_t size) {
            if (size == 0)
                return nullptr;
            auto old_size = size;
            size = block_align(size);
            if (size >= block_available_size)
                return nullptr;
            if (block_current == block_head)
                return alloc_free_block(size);
            auto blk = block_current;
            do {
                if (block_get_flag(blk, BLOCK_USING) == 0 && blk->size >= size) {
                    block_current = blk;
                    return alloc_free_block(size);
                }
                blk = blk->next;
            } while (blk != block_current);
            return nullptr;
        }

        // ���ҿ��п�
        void *alloc_free_block(size_t size) {
            if (block_current->size == size) // ����Ĵ�С�����ǿ��п��С
            {
                return alloc_cur_block(size + 1);
            }
            // ����Ŀռ�С�ڿ��п��С�������п����
            auto new_size = block_current->size - size - 1;
            if (new_size == 0)
                return alloc_cur_block(size); // ���Ѻ���¿�ռ���ͣ���������
            block *new_blk = block_current + size + 1;
            block_init(new_blk, new_size);
            block_connect(block_current, new_blk);
            return alloc_cur_block(size);
        }

        // ֱ��ʹ�õ�ǰ�Ŀ��п�
        void *alloc_cur_block(size_t size) {
            // ֱ��ʹ�ÿ��п�
            block_set_flag(block_current, BLOCK_USING, 1); // ���ñ�־Ϊ����
            block_current->size = size;
            block_available_size -= size + 1;
            auto cur = static_cast<void *>(block_current + 1);
            block_current = block_current->next; // ָ���һ����
            return cur;
        }

        // �ͷ��ڴ�
        bool _free(void *p) {
            auto blk = static_cast<block *>(p);
            --blk; // �Լ��õ����Ԫ��Ϣͷ
            if (!verify_address(blk))
                return false;
            if (blk->next == blk) // ֻ��һ����
            {
                block_set_flag(blk, BLOCK_USING, 0);
                return true;
            }
            if (blk->prev == blk->next && block_get_flag(blk->prev, BLOCK_USING) == 0) // ֻ��������
            {
                _init(); // �����鶼���У�ֱ�ӳ�ʼ��
                return true;
            }
            auto is_prev_free = block_get_flag(blk->prev, BLOCK_USING) == 0 && blk->prev < blk;
            auto is_next_free = block_get_flag(blk->next, BLOCK_USING) == 0 && blk < blk->next;
            auto bit = (is_prev_free << 1) + is_next_free;
            switch (bit) {
                case 0:
                    block_available_size += blk->size + 1;
                    block_set_flag(blk, BLOCK_USING, 0);
                    break;
                case 1:
                    if (block_current == blk->next)
                        block_current = blk;
                    block_available_size += block_merge(blk, blk->next, true);
                    block_set_flag(blk, BLOCK_USING, 0);
                    break;
                case 2:
                    block_available_size += block_merge(blk->prev, blk, false);
                    break;
                case 3:
                    if (block_current == blk->next)
                        block_current = blk->prev;
                    block_available_size += block_merge(blk->prev, blk, blk->next);
                    break;
                default:
                    break;
            }
            return true;
        }

        // ��֤��ַ�Ƿ�Ϸ�
        bool verify_address(block *blk) {
            if (blk < block_head || blk > block_head + DEFAULT_ALLOC_MEMORY_SIZE - 1)
                return false;
            return (blk->next->prev == blk) && (blk->prev->next == blk) && (block_get_flag(blk, BLOCK_USING) == 1);
        }

        // ���·����ڴ�
        void *_realloc(void *p, uint newSize, uint clsSize) {
            auto blk = static_cast<block *>(p);
            --blk; // �Լ��õ����Ԫ��Ϣͷ
            if (!verify_address(blk))
                return nullptr;
            auto size = block_align(newSize * clsSize); // �����µ��ڴ��С
            auto _new = _alloc(size);
            if (!_new) {
                // �ռ䲻��
                _free(blk);
                return nullptr;
            }
            auto oldSize = blk->size;
            memmove(_new, p, sizeof(block) * __min(oldSize, size)); // �ƶ��ڴ�
            _free(p);
            return _new;
        }

    public:
        // Ĭ�ϵĿ�����
        static const size_t DEFAULT_ALLOC_BLOCK_SIZE = DefaultSize;
        // Ĭ�ϵ��ڴ�����
        static const size_t DEFAULT_ALLOC_MEMORY_SIZE = BLOCK_SIZE * DEFAULT_ALLOC_BLOCK_SIZE;

        legacy_memory_pool() {
            _create();
        }

        ~legacy_memory_pool() {
            _destroy();
        }

        template<class T>
        T *alloc() {
            return static_cast<T *>(_alloc(sizeof(T)));
        }

        template<class T>
        T *alloc_array(size_t count) {
            return static_cast<T *>(_alloc(count * sizeof(T)));
        }

        template<class T, class ...TArgs>
        T *alloc_args(const TArgs &&... args) {
            T *obj = static_cast<T *>(_alloc(sizeof(T)));
            (*obj)(std::forward(args)...);
            return obj;
        }

        template<class T, class ...TArgs>
        T *alloc_array_args(uint count, const TArgs &&... args) {
            T *obj = static_cast<T *>(_alloc(count * sizeof(T)));
            for (uint i = 0; i < count; ++i) {
                (obj[i])(std::forward(args)...);
            }
            return obj;
        }

        template<class T>
        T *realloc(T *obj, uint newSize) {
            return static_cast<T *>(_realloc(obj, newSize, sizeof(T)));
        }

        template<class T>
        bool free(T *obj) {
            return _free(obj);
        }

        template<class T>
        bool free_array(T *obj) {
            return _free(obj);
        }

        size_t available() const {
            return block_available_size;
        }

        void clear() {
            _init();
        }

        void dump(std::ostream &os) {
            auto ptr = block_head;
            printf("[DEBUG] MEM   | Available: %zu\n", block_available_size);
            if (ptr->next == ptr) {
                if (block_get_flag(ptr, BLOCK_USING)) {
                    dump_block(ptr, os);
                } else {
                    os << "[DEBUG] MEM   | All Free." << std::endl;
                }
            } else {
                dump_block(ptr, os);
                ptr = ptr->next;
                while (ptr != block_head) {
                    dump_block(ptr, os);
                    ptr = ptr->next;
                }
            }
        }

    private:
        static void dump_block(block *blk, std::ostream &os) {
            printf("[DEBUG] MEM   | [%p-%p] Size: %8lu, State: %s\n", blk, blk + blk->size, blk->size, block_get_flag(blk, BLOCK_USING) ? "Using" : "Free");
        }
    };

    // ����ԭʼ�ڴ�ص��ڴ�������
    template<class Allocator = default_allocator<>, size_t DefaultSize = Allocator::DEFAULT_ALLOC_BLOCK_SIZE>
    class legacy_memory_pool_allocator {
        legacy_memory_pool<Allocator, DefaultSize> memory_pool;

    public:
        static const size_t DEFAULT_ALLOC_BLOCK_SIZE = DefaultSize - 2;

        template<class T>
        T *__alloc() {
            return memory_pool.template alloc<T>();
        }

        template<class T>
        T *__alloc_array(uint count) {
            return memory_pool.template alloc_array<T>(count);
        }

        template<class T, class ... TArgs>
        T *__alloc_args(const TArgs &&... args) {
            return memory_pool.template alloc_args<T>(std::forward(args)...);
        }

        template<class T, class ... TArgs>
        T *__alloc_array_args(uint count, const TArgs &&... args) {
            return memory_pool.template alloc_array_args<T>(count, std::forward(args)...);
        }

        template<class T>
        T *__realloc(T *t, uint newSize) {
            return memory_pool.template realloc<T>(t, newSize);
        }

        template<class T>
        bool __free(T *t) {
            return memory_pool.free(t);
        }

        template<class T>
        bool __free_array(T *t) {
            return memory_pool.free_array(t);
        }
    };

    template<size_t DefaultSize = default_allocator<>::DEFAULT_ALLOC_BLOCK_SIZE>
    using memory_pool = legacy_memory_pool<legacy_memory_pool_allocator<default_allocator<>, DefaultSize>>;
}

#endif //CLIBLISP_MEMORY_H
