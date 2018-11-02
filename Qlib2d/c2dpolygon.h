//
// Project: clib2d
// Created by bajdcc
//

#ifndef CLIB2D_C2DPOLYGON_H
#define CLIB2D_C2DPOLYGON_H

#include "c2dbody.h"

class Q2dHelper;

namespace clib {
    // ����θ��壨��֧��͹����Σ��ҵ㼯Ϊ�������У�
    class c2d_polygon : public c2d_body {
    public:
        using ptr = std::unique_ptr<c2d_polygon>;

        c2d_polygon(uint16_t _id, decimal _mass, const std::vector<v2> &_vertices);

        // �����������
        static decimal calc_polygon_area(const std::vector<v2> &vertices);

        // ������������
        static v2 calc_polygon_centroid(const std::vector<v2> &vertices);

        // ��������ת������
        static decimal calc_polygon_inertia(decimal mass, const std::vector<v2> &vertices);

        // ����߽磨���ΰ�Χ��
        void calc_bounds();

        // �ж��ڱ߽���
        bool contains_in_bound(const v2 &pt);

        // �ж��ڶ�����ڣ��ж��������������һ���ԣ�Ҫ��͹����������������ʱ�����У�
        bool contains_in_polygon(const v2 &pt);

        bool contains(const v2 &pt) override;

        void init();

        void refresh();

        void impulse(const v2 &p, const v2 &r) override;

        v2 world() const override;

        c2d_body_t type() const override;

        v2 min() const override;

        v2 max() const override;

        void update(v2 gravity, int n) override;

        void pass0();

        void pass1();

        void pass2();

        void pass3(const v2 &gravity);

        void pass4();

        void pass5();

        // ��ק����
        void drag(const v2 &pt, const v2 &offset) override;

        void draw(Q2dHelper * helper) override;

        // ��idxΪ��㣬��һ����Ϊ�յ������
        v2 edge(size_t idx) const;

        v2 &vertex(size_t idx);

        size_t index(size_t idx) const;

        size_t edges() const;

        v2 center; // ����
        m2 R; // ��ת����
        std::vector<v2> vertices; // ����εĶ��㣨�������꣩
        std::vector<v2> verticesWorld; // ����εĶ��㣨�������꣩
        v2 boundMin, boundMax; // �������
    };
}

#endif //CLIB2D_C2DPOLYGON_H
