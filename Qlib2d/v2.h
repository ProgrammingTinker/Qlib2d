//
// Project: clib2d
// Created by bajdcc
//

#ifndef CLIB2D_V2_H
#define CLIB2D_V2_H

#include "c2d.h"

namespace clib {
    // ��ά����
    struct v2 {
        decimal x{0}, y{0}; // X��Y����

        // ���캯��
        v2() = default;

        v2(decimal _x, decimal _y);

        v2(const v2 &v) = default;

        v2 &operator=(const v2 &v) = default;

        v2 operator*(decimal d) const;

        v2 operator/(decimal d) const;

        v2 operator+(const v2 &v) const;

        v2 operator-(const v2 &v) const;

        v2 operator+(decimal d) const;

        v2 operator-(decimal d) const;

        v2 &operator+=(const v2 &v);

        v2 &operator-=(const v2 &v);

        friend v2 operator*(decimal d, const v2 &v);

        v2 operator-() const;

        // ���
        decimal cross(const v2 &v) const;

        // ���
        decimal dot(const v2 &v) const;

        decimal magnitude() const;

        decimal magnitude_square() const;

        v2 normalize() const;

        // ��������
        v2 normal() const;

        v2 N() const;

        bool zero(decimal d) const;

        v2 rotate(decimal theta) const;

        v2 clamp(decimal d) const;
    };
}

#endif //CLIB2D_V2_H
