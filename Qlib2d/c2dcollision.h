//
// Project: clib2d
// Created by bajdcc
//

#ifndef CLIB2D_C2DCOLLISION_H
#define CLIB2D_C2DCOLLISION_H

#include "c2dcontact.h"
#include "c2dpolygon.h"
#include "c2dcircle.h"

namespace clib {
    // ��ײ�ṹ
    struct collision {
        std::vector<contact> contacts; // �Ӵ����б�
        c2d_body *bodyA{nullptr}, *bodyB{nullptr}; // ��ײ����������
        union intern {
            struct {
                size_t idx;
                decimal sat;
            } polygon;
            struct {
                // none
            } circle;
        } A{0}, B{0};
        v2 N; // ����
    };

    // ��ײ���-SAT�����ᶨ��
    // �����͹���Ƿ��ཻ
    // �������������͹�����û���ཻ����ô����������������һ�����ϵ�ͶӰ���ص���
    // �᣺ֻ���������͹����ÿ��������⼴��
    // ֻҪ����϶�����㣬��Ϊ���ཻ
    // separation������϶
    // idx������϶����
    // �ο�Box2D��https://github.com/erincatto/Box2D/blob/master/Box2D/Collision/b2CollidePolygon.cpp#L23
    int max_separating_axis_polygon(c2d_polygon *a, c2d_polygon *b, collision::intern &c);

    int max_separating_axis_polygon_circle(c2d_polygon *a, c2d_circle *b, collision::intern &c);

    int max_separating_axis_circle(c2d_circle *a, c2d_circle *b);

    int max_separating_axis(c2d_body *a, c2d_body *b, collision::intern &c);

    // ���ð�Χ�з��������ж���ײ
    bool AABB_collide(c2d_body *a, c2d_body *b);

    // �ο�Box2D��https://github.com/erincatto/Box2D/blob/master/Box2D/Collision/b2CollidePolygon.cpp#L64
    static size_t incident_edge(const v2 &N, c2d_polygon *body);

    // Sutherland-Hodgman������βü���
    // �ο�Box2D��https://github.com/erincatto/Box2D/blob/master/Box2D/Collision/b2Collision.cpp#L201
    size_t clip(std::vector<contact> &out,
                const std::vector<contact> &in,
                size_t i,
                const v2 &p1, const v2 &p2);

    // ������ײ������Σ�
    bool solve_collision_polygon(collision &c);

    // ������ײ���������Բ�����������Ƚ϶ࣩ
    bool solve_collision_polygon_circle(collision &c);

    // ������ײ��Բ��
    bool solve_collision_circle(collision &c);

    // ������ײ�������Ƿ���ײ��
    bool solve_collision(collision &c);

    // ��ײ����
    void collision_update(collision &c, const collision &old_c);
}

#endif //CLIB2D_C2DCOLLISION_H
