//
// Project: clib2d
// Created by bajdcc
//

#include "stdafx.h"
#include "c2dcircle.h"
#include "c2dworld.h"
#include "q2dhelper.h"

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
        auto n = int(std::floor(std::pow(2.0, std::log2(PI2 * r.value * CIRCLE_N))));
        auto delta = 2.0 / n;
        for (auto i = 0; i < n; i++) {
            vertices.push_back(
                v2(r.value * std::cos(i * delta * M_PI),
                   r.value * std::sin(i * delta * M_PI)));
        }
        for (size_t i = 0; i < vertices.size(); ++i) {
            verticesWorld.push_back(vertices[i] + pos); // ��������ת��Ϊ��������
        }
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
        for (size_t i = 0; i < vertices.size(); ++i) {
            verticesWorld[i] = vertices[i] + pos; // ��������ת��Ϊ��������
        }
    }

    void c2d_circle::pass3(const v2 &gravity) {
        if (!std::isinf(life)) {
            life_tick -= c2d_world::dt;
        }
        if (snow_k != 0.0 && !V.zero(EPSILON_V)) {
            angleV *= SNOW_ANGLE;
            auto cases = (V.x > 0 ? 1 : 0) | (V.y > 0 ? 2 : 0);
            switch (cases)
            {
            case 0: // �������ޣ�������Ʈ�䣬���ķ���Ϊ-90��
                F += V.rotate(-M_PI_2) * snow_k;
                break;
            case 1: // �������ޣ�������Ʈ�䣬���ķ���Ϊ90��
                F += V.rotate(M_PI_2) * snow_k;
                break;
            case 2: // �ڶ����ޣ�������Ʈ�䣬���ķ���Ϊ-90��
                F += V.rotate(-M_PI_2) * snow_k * 0.1;
                F -= V * snow_k * 0.9;
                break;
            case 3: // ��һ���ޣ�������Ʈ�䣬���ķ���Ϊ90��
                F += V.rotate(M_PI_2) * snow_k * 0.1;
                F -= V * snow_k * 0.9;
                break;
            default:
                break;
            }
        }
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

    void c2d_circle::draw(Q2dHelper * helper) {
        if (statics) { // ����̬����
            helper->paint_circle(pos, r.value, Q2dHelper::PAINT_TYPE::Static);
            return;
        }
        auto &p = pos;
        if (!text.isEmpty()) {
            if (std::isinf(life) && life != 0.0)
                helper->paint_text(p, angle, text, Q2dHelper::PAINT_TYPE::AnimationText);
            else if (life_tick > 0) {
                auto lightness = ((int)std::floor(255.0 * life_tick / life)) & 0xff;
                helper->paint_text(p, angle, text, Q2dHelper::PAINT_TYPE::AnimationText, lightness);
            }
            return;
        }
#if ENABLE_SLEEP
        if (sleep) { // ����������
            helper->paint_circle(pos, r.value, Q2dHelper::PAINT_TYPE::Sleep);
            helper->paint_point(pos, Q2dHelper::PAINT_TYPE::Center);
            return;
        }
#endif
        const auto boundMin = pos - r.value;
        const auto boundMax = pos + r.value;
        helper->paint_bound(boundMin, boundMax, Q2dHelper::PAINT_TYPE::Bound);
        if (collision > 0)
            helper->paint_circle(pos, r.value, Q2dHelper::PAINT_TYPE::Collision);
        else
            helper->paint_circle(pos, r.value, Q2dHelper::PAINT_TYPE::Normal);
        // ����Ĭ�����������ĶԳƵģ����ľ������ģ�������������
        auto F = v2((Fa.x >= 0 ? 0.05 : -0.05) * std::log10(1 + std::abs(Fa.x) * 5),
            (Fa.y >= 0 ? 0.05 : -0.05) * std::log10(1 + std::abs(Fa.y) * 5)); // ������
        auto D = v2(std::cos(angle) * 0.02, std::sin(angle) * 0.02);
        helper->paint_line(p, p + F, Q2dHelper::PAINT_TYPE::Force); // ������
        helper->paint_line(p, p + V * 0.2, Q2dHelper::PAINT_TYPE::Velocity); // �ٶ�����
        helper->paint_line(p, p + D, Q2dHelper::PAINT_TYPE::Direction); // ��������
        helper->paint_point(p, Q2dHelper::PAINT_TYPE::Center);
    }

    v2 c2d_circle::edge(size_t idx) const {
        return verticesWorld[index(idx + 1)] - verticesWorld[index(idx)];
    }

    v2 &c2d_circle::vertex(size_t idx) {
        return verticesWorld[index(idx)];
    }

    size_t c2d_circle::index(size_t idx) const {
        return idx % verticesWorld.size();
    }

    size_t c2d_circle::edges() const {
        return verticesWorld.size();
    }
}