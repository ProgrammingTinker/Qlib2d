//
// Project: clib2d
// Created by bajdcc
//

#ifndef CLIB2D_C2DBODY_H
#define CLIB2D_C2DBODY_H

#include "c2d.h"
#include "v2.h"
#include "m2.h"

class Q2dHelper;

namespace clib {
    enum c2d_body_t {
        C2D_POLYGON,
        C2D_CIRCLE,
    };

    // ������࣬���ڱ�ȻҪ��̬����˲�����struct
    // ����Ϊ��̬����������Ҫ��unique_ptr�е��ڴ����
    class c2d_body {
    public:
        using ptr = std::unique_ptr<c2d_body>;

        c2d_body(uint16_t _id, decimal _mass);

        c2d_body(const c2d_body &) = delete; // ��ֹ����
        c2d_body &operator=(const c2d_body &) = delete; // ��ֹ��ֵ

        virtual void drag(const v2 &pt, const v2 &offset) = 0;  // �϶���ʩ������
        virtual bool contains(const v2 &pt) = 0;  // �Ƿ��������������

        virtual void impulse(const v2 &p, const v2 &r) = 0;  // �������

        virtual v2 world() const = 0; // ��������
        virtual c2d_body_t type() const = 0; // ����

        virtual v2 min() const = 0; // �±߽�
        virtual v2 max() const = 0; // �ϱ߽�

        // �ֽ׶�
        // i=0����������
        // i=1����һ�׶Σ������ٶȡ����ٶ�
        // i=2���ڶ��׶Σ�����λ�õ�������
        virtual void update(v2 gravity, int) = 0; // ״̬����
        virtual void draw(Q2dHelper * helper) = 0; // ����

        v2 rotate(const v2 &v) const;

        // ��idxΪ��㣬��һ����Ϊ�յ������
        virtual v2 edge(size_t idx) const = 0;

        virtual v2 &vertex(size_t idx) = 0;

        virtual size_t index(size_t idx) const = 0;

        virtual size_t edges() const = 0;

        // ����д��ô��get/set����public����
#if ENABLE_SLEEP
        bool sleep{false}; // �Ƿ�����
#endif
        bool statics{false}; // �Ƿ�Ϊ��̬����
        int collision{0}; // ������ײ�Ĵ���
        uint16_t id{0}; // ID
        decimal_inv mass{1}; // ����
        v2 pos; // λ�ã��������꣬����δע����Ϊ�������꣩
        v2 V; // �ٶ�
        decimal angle{0}; // �Ƕ�
        decimal angleV{0}; // ���ٶ�
        decimal_inv inertia{0}; // ת������
        decimal f{0.2}; // ����/��Ħ��ϵ��
        v2 F; // ����
        v2 Fa; // �������ۼƣ�
        decimal M{0}; // ����
        decimal CO{COLL_CO}; // ������ײϵ��
        QString text;
    };
}

#endif //CLIB2D_C2DBODY_H
