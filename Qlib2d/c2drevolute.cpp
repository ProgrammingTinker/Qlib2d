//
// Project: clib2d
// Created by bajdcc
//

#include "stdafx.h"
#include "c2drevolute.h"
#include "c2dworld.h"
#include "q2dhelper.h"

namespace clib {

    void c2d_revolute_joint::prepare(const v2 &gravity) {
        static const auto kBiasFactor = 0.2;
        auto &a = *this->a;
        auto &b = *this->b;
        ra = a.rotate(local_anchor_a);
        rb = b.rotate(local_anchor_b);
        auto k = m2(a.mass.inv + b.mass.inv) +
                 (a.inertia.inv * m2(ra.y * ra.y, -ra.y * ra.x, -ra.y * ra.x, ra.x * ra.x)) +
                 (b.inertia.inv * m2(rb.y * rb.y, -rb.y * rb.x, -rb.y * rb.x, rb.x * rb.x));
        mass = k.inv();
        bias = -kBiasFactor * c2d_world::dt_inv * (b.world() + rb - a.world() - ra);

        a.update(gravity, 0);
        b.update(gravity, 0); // ��ʼ����������

        a.impulse(-p, ra);
        b.impulse(p, rb);

        a.update(gravity, 1);
        b.update(gravity, 1); // �����������أ��ó��ٶȺͽ��ٶ�
    }

    void c2d_revolute_joint::update(const v2 &gravity) {
        auto &a = *this->a;
        auto &b = *this->b;
        auto dv = (a.V + (-a.angleV * ra.N())) -
                  (b.V + (-b.angleV * rb.N()));
        p = mass * (dv + bias);
        if (!p.zero(EPSILON)) {
            p_acc = p;

            a.update(gravity, 0);
            b.update(gravity, 0); // ��ʼ����������

            a.impulse(-p, ra);
            b.impulse(p, rb);

            a.update(gravity, 1);
            b.update(gravity, 1); // �����������أ��ó��ٶȺͽ��ٶ�
        }
    }

    void c2d_revolute_joint::draw(Q2dHelper * helper) {
        auto centerA = a->world();
        auto anchorA = world_anchor_a();
        auto centerB = b->world();
        auto anchorB = world_anchor_b();

        auto str = std::min(std::log2(1 + p_acc.magnitude()), 10.0) * 0.08 * 255;
        QColor color(255 - str, 255 * 0.2, 255 * 0.2 + str);
        if (!a->statics) {
            helper->paint_line(centerA, anchorA, color);
        }
        if (!b->statics) {
            helper->paint_line(centerB, anchorB, color);
        }
        glEnd();
    }

    v2 c2d_revolute_joint::world_anchor_a() const {
        return a->rotate(local_anchor_a) + a->world();
    }

    v2 c2d_revolute_joint::world_anchor_b() const {
        return b->rotate(local_anchor_b) + b->world();
    }

    c2d_revolute_joint::c2d_revolute_joint(c2d_body *_a, c2d_body *_b, const v2 &_anchor) :
        c2d_joint(_a, _b), anchor(_anchor) {
        local_anchor_a = m2().rotate(-a->angle).rotate(anchor - a->world());
        local_anchor_b = m2().rotate(-b->angle).rotate(anchor - b->world());
    }
}