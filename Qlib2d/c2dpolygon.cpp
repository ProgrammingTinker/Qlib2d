//
// Project: clib2d
// Created by bajdcc
//

#include "stdafx.h"
#include "c2dpolygon.h"
#include "c2dworld.h"
#include "q2dhelper.h"

namespace clib {

    c2d_polygon::c2d_polygon(uint16_t _id, decimal _mass, const std::vector<v2> &_vertices)
        : c2d_body(_id, _mass), vertices(_vertices), verticesWorld(_vertices) {
        init();
    }

    decimal c2d_polygon::calc_polygon_area(const std::vector<v2> &vertices) {
        decimal area = 0;
        auto size = vertices.size();
        // �����������ε����֮��
        for (size_t i = 0; i < size; ++i) {
            auto j = (i + 1) % size;
            // �������������������ƽ���ı������
            // ����Ҫ�������������Ҫ����2
            area += vertices[i].cross(vertices[j]);
        }
        return area / 2;
    }

    v2 c2d_polygon::calc_polygon_centroid(const std::vector<v2> &vertices) {
        v2 gc;
        auto size = vertices.size();
        // ���� = (������������ * �����) / �����
        // ���������� = ������֮�� / 3
        for (size_t i = 0; i < size; ++i) {
            auto j = (i + 1) % size;
            gc += (vertices[i] + vertices[j]) * vertices[i].cross(vertices[j]);
        }
        return gc / 6.0 / calc_polygon_area(vertices);
    }

    decimal c2d_polygon::calc_polygon_inertia(decimal mass, const std::vector<v2> &vertices) {
        if (std::isinf(mass))
            return mass;
        decimal acc0 = 0, acc1 = 0;
        auto size = vertices.size();
        // ת������ = m / 6 * (����������� * ��(a*a+a*b+b*b)) / (�����)
        for (size_t i = 0; i < size; ++i) {
            auto a = vertices[i], b = vertices[(i + 1) % size];
            auto _cross = std::abs(a.cross(b));
            acc0 += _cross * (a.dot(a) + b.dot(b) + a.dot(b));
            acc1 += _cross;
        }
        return mass * acc0 / 6 / acc1;
    }

    void c2d_polygon::calc_bounds() {
        boundMin = boundMax = vertex(0);
        for (size_t i = 1; i < verticesWorld.size(); ++i) {
            boundMin.x = std::min(boundMin.x, vertex(i).x);
            boundMin.y = std::min(boundMin.y, vertex(i).y);
            boundMax.x = std::max(boundMax.x, vertex(i).x);
            boundMax.y = std::max(boundMax.y, vertex(i).y);
        }
    }

    bool c2d_polygon::contains_in_bound(const v2 &pt) {
        return boundMin.x < pt.x &&
               boundMax.x > pt.x &&
               boundMin.y < pt.y &&
               boundMax.y > pt.y;
    }

    bool c2d_polygon::contains_in_polygon(const v2 &pt) {
        const auto size = verticesWorld.size();
        if (size < 3) return false;
        if ((pt - vertex(0)).cross(vertex(1) - vertex(0)) > 0)
            return false;
        if ((pt - vertex(0)).cross(vertex(size - 1) - vertex(0)) < 0)
            return false;

        // �ж�ʣ�µ����߷����һ����
        size_t i = 2, j = size - 1;
        auto line = SIZE_MAX;

        // ���ַ�
        while (i <= j) {
            auto mid = (i + j) >> 1;
            if ((pt - vertex(0)).cross(vertex(mid) - vertex(0)) > 0) {
                line = mid;
                j = mid - 1;
            } else i = mid + 1;
        }
        return (pt - vertex(line - 1)).cross(vertex(line) - vertex(line - 1)) < 0;
    }

    bool c2d_polygon::contains(const v2 &pt) {
        // �ȿ����ж��Ƿ��ڰ�Χ��������Σ���
        // ��������ж��ڶ�����У��ⲽ�����
        return contains_in_bound(pt) && contains_in_polygon(pt);
    }

    void c2d_polygon::init() {
        inertia.set(calc_polygon_inertia(mass.value, vertices));
        center = calc_polygon_centroid(vertices);
        refresh();
    }

    void c2d_polygon::refresh() {
        R.rotate(angle);
        for (size_t i = 0; i < edges(); ++i) {
            auto v = R.rotate(vertices[i] - center) + center;
            vertex(i) = pos + v; // ��������ת��Ϊ��������
        }
        calc_bounds();
    }

    void c2d_polygon::impulse(const v2 &p, const v2 &r) {
        if (statics) return;
        auto _p = p * c2d_world::dt_inv;
        F += _p;
        Fa += _p;
        M += r.cross(_p);
    }

    v2 c2d_polygon::world() const {
        return pos + center;
    }

    c2d_body_t c2d_polygon::type() const {
        return C2D_POLYGON;
    }

    v2 c2d_polygon::min() const {
        return boundMin;
    }

    v2 c2d_polygon::max() const {
        return boundMax;
    }

    void c2d_polygon::update(v2 gravity, int n) {
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

    void c2d_polygon::pass0() {
        F.x = F.y = 0;
        M = 0;
    }

    void c2d_polygon::pass1() {
        V += F * mass.inv * c2d_world::dt;
        angleV += M * inertia.inv * c2d_world::dt;
    }

    void c2d_polygon::pass2() {
        pos += V * c2d_world::dt;
        angle += angleV * c2d_world::dt;
        R.rotate(angle);
        for (size_t i = 0; i < edges(); ++i) {
            auto v = R.rotate(vertices[i] - center) + center;
            vertex(i) = pos + v; // ��������ת��Ϊ��������
        }
        calc_bounds();
    }

    void c2d_polygon::pass3(const v2 &gravity) {
        F += gravity * mass.value * c2d_world::dt;
        Fa += F;
    }

    void c2d_polygon::pass4() {
        Fa.x = Fa.y = 0;
    }

    void c2d_polygon::pass5() {
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

    void c2d_polygon::drag(const v2 &pt, const v2 &offset) {
        V += mass.inv * offset;
        angleV += inertia.inv * (pt - pos - center).cross(offset);
    }

    void c2d_polygon::draw(Q2dHelper * helper) {
        if (statics) { // ����̬����
            helper->paint_polygon(verticesWorld, Q2dHelper::PAINT_TYPE::Static);
            return;
        }
        auto p = pos + center;
        if (!text.isEmpty()) {
            helper->paint_text(p, angle, text, Q2dHelper::PAINT_TYPE::AnimationText);
            return;
        }
#if ENABLE_SLEEP
        if (sleep) { // ����������
            helper->paint_polygon(verticesWorld, Q2dHelper::PAINT_TYPE::Sleep);
            helper->paint_point(pos + center, Q2dHelper::PAINT_TYPE::Center);
            return;
        }
#endif
        helper->paint_bound(boundMin, boundMax, Q2dHelper::PAINT_TYPE::Bound);
        if (collision > 0)
            helper->paint_polygon(verticesWorld, Q2dHelper::PAINT_TYPE::Collision);
        else
            helper->paint_polygon(verticesWorld, Q2dHelper::PAINT_TYPE::Normal);
        // ����Ĭ�����������ĶԳƵģ����ľ������ģ�������������
        auto F = v2((Fa.x >= 0 ? 0.05 : -0.05) * std::log10(1 + std::abs(Fa.x) * 5),
            (Fa.y >= 0 ? 0.05 : -0.05) * std::log10(1 + std::abs(Fa.y) * 5)); // ������
        auto D = v2(R.x1 * 0.02, R.x2 * 0.02);
        helper->paint_line(p, p + F, Q2dHelper::PAINT_TYPE::Force); // ������
        helper->paint_line(p, p + V * 0.2, Q2dHelper::PAINT_TYPE::Velocity); // �ٶ�����
        helper->paint_line(p, p + D, Q2dHelper::PAINT_TYPE::Direction); // ��������
        helper->paint_point(p, Q2dHelper::PAINT_TYPE::Center);
    }

    v2 c2d_polygon::edge(size_t idx) const {
        return verticesWorld[index(idx + 1)] - verticesWorld[index(idx)];
    }

    v2 &c2d_polygon::vertex(size_t idx) {
        return verticesWorld[index(idx)];
    }

    size_t c2d_polygon::index(size_t idx) const {
        return idx % verticesWorld.size();
    }

    size_t c2d_polygon::edges() const {
        return verticesWorld.size();
    }
}