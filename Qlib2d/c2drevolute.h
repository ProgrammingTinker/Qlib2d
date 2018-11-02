//
// Project: clib2d
// Created by bajdcc
//

#ifndef CLIB2D_C2DREVOLUTE_H
#define CLIB2D_C2DREVOLUTE_H

#include "c2djoint.h"

class Q2dHelper;

namespace clib {
    // ��ת�ؽ�
    class c2d_revolute_joint : public c2d_joint {
    public:
        void prepare(const v2 &gravity) override;

        void update(const v2 &gravity) override;

        void draw(Q2dHelper * helper) override;

        v2 world_anchor_a() const;

        v2 world_anchor_b() const;

        c2d_revolute_joint(c2d_body *_a, c2d_body *_b, const v2 &_anchor);

        c2d_revolute_joint(const c2d_revolute_joint &) = delete; // ��ֹ����
        c2d_revolute_joint &operator=(const c2d_revolute_joint &) = delete; // ��ֹ��ֵ

        // ��������-ê�� World anchor
        v2 anchor;
        // ����A����-ê��-�������� Local anchor relative to centroid of body a
        v2 local_anchor_a;
        // ����B����-ê��-�������� Local anchor relative to centroid of body b
        v2 local_anchor_b;

        // �ϴ�״̬ Cached status in prev step
        // ê������A����-������� Anchor point to body a' centroid
        v2 ra;
        // ê������B����-������� Anchor point to body b' centroid
        v2 rb;
        // �������� The combined mass
        m2 mass;
        // ���� Accumulated impulse
        v2 p;
        // �����ۼ� Accumulated impulse
        v2 p_acc;
        // ���� The bias for position correction
        v2 bias;
    };
}

#endif //CLIB2D_C2DREVOLUTE_H
