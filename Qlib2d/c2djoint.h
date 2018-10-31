//
// Project: clib2d
// Created by bajdcc
//

#ifndef CLIB2D_C2DJOINT_H
#define CLIB2D_C2DJOINT_H

#include "c2dbody.h"

namespace clib {
    // �ؽ�
    class c2d_joint {
    public:
        using ptr = std::unique_ptr<c2d_joint>;

        virtual void prepare(const v2 &gravity) = 0; // Ԥ����
        virtual void update(const v2 &gravity) = 0; // ����
        virtual void draw() = 0; // ����

        c2d_joint(c2d_body *_a, c2d_body *_b);

        c2d_joint(const c2d_body &) = delete; // ��ֹ����
        c2d_joint &operator=(const c2d_joint &) = delete; // ��ֹ��ֵ

        c2d_body *a, *b; // �ؽ��漰����������
    };
}

#endif //CLIB2D_C2DJOINT_H
