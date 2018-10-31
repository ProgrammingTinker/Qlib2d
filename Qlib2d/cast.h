//
// Project: cliblisp
// Created by bajdcc
//

#ifndef CLIBLISP_CAST_H
#define CLIBLISP_CAST_H

#include "memory.h"

#define AST_NODE_MEM (32 * 1024)
#define AST_STR_MEM (16 * 1024)

namespace clib {

    enum ast_t {
        ast_root,
        ast_env,
        ast_sub,
        ast_lambda,
        ast_sexpr,
        ast_qexpr,
        ast_literal,
        ast_string,
        ast_char,
        ast_uchar,
        ast_short,
        ast_ushort,
        ast_int,
        ast_uint,
        ast_long,
        ast_ulong,
        ast_float,
        ast_double,
    };

    enum ast_to_t {
        to_parent,
        to_prev,
        to_next,
        to_child,
    };

    // ���
    struct ast_node {
        // ����
        uint32_t flag;

        union {
#define DEFINE_NODE_DATA(t) LEX_T(t) _##t;
            DEFINE_NODE_DATA(char)
            DEFINE_NODE_DATA(uchar)
            DEFINE_NODE_DATA(short)
            DEFINE_NODE_DATA(ushort)
            DEFINE_NODE_DATA(int)
            DEFINE_NODE_DATA(uint)
            DEFINE_NODE_DATA(long)
            DEFINE_NODE_DATA(ulong)
            DEFINE_NODE_DATA(float)
            DEFINE_NODE_DATA(double)
#undef DEFINE_NODE_DATA
            const char *_string;
        } data; // ����

        // �������ݽṹ�������
        ast_node *parent; // ����
        ast_node *prev; // ���ֵ�
        ast_node *next; // ���ֵ�
        ast_node *child; // �������
    };

    class cast {
    public:
        cast();
        ~cast() = default;

        cast(const cast &) = delete;
        cast &operator=(const cast &) = delete;

        ast_node *get_root() const;

        ast_node *new_node(ast_t type);
        ast_node *new_child(ast_t type, bool step = true);
        ast_node *new_sibling(ast_t type, bool step = true);

        ast_node *add_child(ast_node*);
        static ast_node *set_child(ast_node*, ast_node*);
        static ast_node *set_sibling(ast_node*, ast_node*);
        static int children_size(ast_node*);

        void set_str(ast_node *node, const string_t &str);
        static std::string display_str(const char *str);

        void to(ast_to_t type);

        static void print(ast_node *node, int level, std::ostream &os);
        static const string_t &ast_str(ast_t type);

        static ast_node *index(ast_node *node, int index);
        static ast_node *index(ast_node *node, const string_t &index);

        void reset();
    private:
        void init();

    private:
        memory_pool<AST_NODE_MEM> nodes; // ȫ��AST����ڴ����
        memory_pool<AST_STR_MEM> strings; // ȫ���ַ�������
        ast_node *root; // �����
        ast_node *current; // ��ǰ���
    };
}

#endif //CLIBLISP_CAST_H
