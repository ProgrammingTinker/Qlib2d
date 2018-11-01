//
// Project: clib2d
// Created by bajdcc
//

#include "stdafx.h"
#include "c2dcircle.h"
#include "c2dworld.h"

namespace clib {

    c2d_circle::c2d_circle(uint16_t _id, decimal _mass, decimal _r)
        : c2d_body(_id, _mass), r(_r) {
        init();
    }

    bool c2d_circle::contains(const v2 &pt) {
        const auto delta = pos - pt;
        return delta.magnitude_square() < r.square;
    }

    void c2d_circle::init() {
        inertia.set(mass.value * r.square * 0.5);
    }

    void c2d_circle::impulse(const v2 &p, const v2 &r) {
        if (statics) return;
        auto _p = p * c2d_world::dt_inv;
        F += _p;
        Fa += _p;
        M += r.cross(_p);
    }

    v2 c2d_circle::world() const {
        return pos;
    }

    c2d_body_t c2d_circle::type() const {
        return C2D_CIRCLE;
    }

    v2 c2d_circle::min() const {
        return pos - r.value;
    }

    v2 c2d_circle::max() const {
        return pos + r.value;
    }

    void c2d_circle::update(v2 gravity, int n) {
        if (statics) return;
#if ENABLE_SLEEP
        if (sleep) return;
#endif
        switch (n) {
            case 0:
                pass0();
                break;
            case 1:
                pass1();
                break;
            case 2:
                pass2();
                break;
            case 3:
                pass3(gravity);
                break;
            case 4:
                pass4();
                break;
            case 5:
                pass5();
                break;
            default:
                break;
        }
    }

    void c2d_circle::pass0() {
        F.x = F.y = 0;
        M = 0;
    }

    void c2d_circle::pass1() {
        V += F * mass.inv * c2d_world::dt;
        angleV += M * inertia.inv * c2d_world::dt;
    }

    void c2d_circle::pass2() {
        pos += V * c2d_world::dt;
        angle += angleV * c2d_world::dt;
    }

    void c2d_circle::pass3(const v2 &gravity) {
        F += gravity * mass.value * c2d_world::dt;
        Fa += F;
    }

    void c2d_circle::pass4() {
        Fa.x = Fa.y = 0;
    }

    void c2d_circle::pass5() {
#if ENABLE_SLEEP
        // �����������ٶ�Ϊ��ʱ���ж�����
        if (Fa.zero(EPSILON_FORCE) && V.zero(EPSILON_V) && std::abs(angleV) < EPSILON_ANGLE_V) {
            V.x = 0;
            V.y = 0;
            angleV = 0;
            pass0();
            pass4();
            collision = 0;
            sleep = true;
        }
#endif
    }

    void c2d_circle::drag(const v2 &pt, const v2 &offset) {
        V += mass.inv * offset;
        angleV += inertia.inv * (pt - pos).cross(offset);
    }

    void c2d_circle::draw() {
        
    }
}