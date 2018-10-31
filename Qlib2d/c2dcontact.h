//
// Project: clib2d
// Created by bajdcc
//

#ifndef CLIB2D_C2DCONTACT_H
#define CLIB2D_C2DCONTACT_H

#include "c2dbody.h"

namespace clib {
    // �Ӵ���
    struct contact {
        v2 pos; // λ��
        v2 ra, rb; // �������ĵ��Ӵ��������
        c2d_body_t ta, tb; // ���������
        decimal sep{0}; // ����ͶӰ���ص����룩
        decimal mass_normal{0};
        decimal mass_tangent{0};
        decimal bias{0};
        decimal pn{0}; // �������
        decimal pt{0}; // �������
        union {
            struct {
                int idx; // ���������ڵģ�����a��b�ı�����+1��Ϊ��������B��Ϊ������A
            } polygon;
            struct {
                // none
            } circle;
        } A{0}, B{0};

        contact(v2 _pos);

        contact(v2 _pos, size_t index);

        bool operator==(const contact &other) const;

        bool operator!=(const contact &other) const;
    };
}

#endif //CLIB2D_C2DCONTACT_H
