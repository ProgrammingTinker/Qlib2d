//
// Project: clib2d
// Created by bajdcc
//

#include "stdafx.h"
#include "c2dcollision.h"

namespace clib {

    int max_separating_axis(c2d_body *a, c2d_body *b, collision::intern &c) {
        c.polygon.sat = -inf;
        // ������������A�����ж���
        for (size_t i = 0; i < a->edges(); ++i) {
            // ���A���������������
            auto va = a->vertex(i);
            // ��õ�ǰ���㵽��һ����ıߵĵ�λ������
            auto N = a->edge(i).normal();
            // ��С��������
            auto min_sep = inf;
            // ������������B
            for (size_t j = 0; j < b->edges(); ++j) {
                // ���B���������������
                auto vb = b->vertex(j);
                // vb - va = �Ӷ���A������B������
                // normal  = �Ӷ���A������A'�ĵ�λ����
                // dot(vb - va, normal) = ����B����AA'ͶӰΪP�����ΪAP�ĳ���
                // ��������ȡ��Сֵ�����
                // min_sep = ��AA'�ߵķ�����NΪ�ᣬ��B�������������ͶӰ����С����
                min_sep = std::min(min_sep, (vb - va).dot(N));
            }
            if (min_sep > c.polygon.sat) {
                c.polygon.sat = min_sep; // Ѱ������϶
                c.polygon.idx = i; // ��
            }
        }
        return c.polygon.sat > 0 ? 0 : 1; // 0���ཻ
    }

    bool AABB_collide(c2d_body *a, c2d_body *b) {
        const auto boundMinA = a->min();
        const auto boundMinB = b->min();
        const auto boundMaxA = a->max();
        const auto boundMaxB = b->max();
        auto centerA = (boundMaxA + boundMinA) / 2; // ���ΰ�ΧȦ���ĵ�
        auto centerB = (boundMaxB + boundMinB) / 2;
        auto sizeA = (boundMaxA - boundMinA) / 2; // ���ΰ�ΧȦ��С�Ķ���֮һ
        auto sizeB = (boundMaxB - boundMinB) / 2;
        return std::abs(centerB.x - centerA.x) <= (sizeA.x + sizeB.x) &&
               std::abs(centerB.y - centerA.y) <= (sizeA.y + sizeB.y);
    }

    size_t incident_edge(const v2 &N, c2d_body *body) {
        size_t idx = SIZE_MAX;
        auto min_dot = inf;
        // ����B����ı�
        for (size_t i = 0; i < body->edges(); ++i) {
            // ��ñ��ϵķ�����
            auto edge_normal = body->edge(i).normal();
            // ��÷�������SAT���ϵ�ͶӰ����
            auto dot = edge_normal.dot(N);
            // �ҳ���СͶӰ������С��϶
            if (dot < min_dot) {
                min_dot = dot; // ��С��϶
                idx = i; // ��������
            }
        }
        return idx;
    }

    size_t clip(std::vector<contact> &out, const std::vector<contact> &in, size_t i, const v2 &p1, const v2 &p2) {
        size_t num_out = 0;
        auto N = (p2 - p1).normal();
        // ����ͶӰ
        auto dist0 = N.dot(in[0].pos - p1);
        auto dist1 = N.dot(in[1].pos - p1);

        // ���ͶӰ��С���㣬��B�����㶼��A��
        if (dist0 <= 0) out[num_out++] = in[0];
        if (dist1 <= 0) out[num_out++] = in[1];

        // ��������һ����A�ڣ�һ����A��
        if (dist0 * dist1 < 0) {
            // �������
            auto interp = dist0 / (dist0 - dist1);
            // ����p1,p2��in1,in2����
            out[num_out].pos = in[0].pos + interp * (in[1].pos - in[0].pos);
            out[num_out].A.polygon.idx = -(int) i - 1;
            ++num_out;
        }

        return num_out;
    }

    bool solve_collision_internal(collision &c) {
        auto bodyA = dynamic_cast<c2d_body *>(c.bodyA);
        auto bodyB = dynamic_cast<c2d_body *>(c.bodyB);
        // ����SAT���ᷨ��
        // edge = A������B��������ı�
        // N = edge�ķ��ߣ�ָ��B����
        c.N = bodyA->edge(c.A.polygon.idx).normal();
        // ��ʱҪ�ҵ�B��������A��������ı�
        c.B.polygon.idx = incident_edge(c.N, bodyB);

        decltype(c.contacts) contacts;
        // �ٶ������Ӵ��㣨��idxB���˵㣩
        contacts.emplace_back(bodyB->vertex(c.B.polygon.idx), bodyB->index(c.B.polygon.idx) + 1);
        contacts.emplace_back(bodyB->vertex(c.B.polygon.idx + 1), bodyB->index(c.B.polygon.idx + 1) + 1);
        auto tmp = contacts;

        // ��idxB�߶ΰ�bodyA���ж���βü�
        for (size_t i = 0; i < bodyA->edges(); ++i) {
            if (i == c.A.polygon.idx)
                continue;
            if (clip(tmp, contacts, i, bodyA->vertex(i), bodyA->vertex(i + 1)) < 2)
                return false;
            contacts = tmp;
        }

        auto va = bodyA->vertex(c.A.polygon.idx);

        auto &pos0 = contacts[0].pos;
        auto &pos1 = contacts[1].pos;
        const auto CO = bodyA->CO * bodyB->CO;
        auto dist = std::abs((va - pos0).dot(c.N));
        auto bias = std::log10(1 + dist) * CO;
        pos0 -= c.N * bias;
        dist = std::abs((va - pos1).dot(c.N));
        bias = std::log10(1 + dist) * CO;
        pos1 -= c.N * bias;

        // ɸѡ����
        for (auto &contact : contacts) {
            // ���㣺contact.pos
            // �ο��㣺�Ӵ��߶˵�va
            // �Ӵ��߷�������ָ������B��
            auto sep = (contact.pos - va).dot(c.N);
            if (sep <= 0) { // ����idxA��bodyAһ��ģ�bodyA�ڵĽӴ��㣩
                contact.sep = sep; // sepԽС���˵�va������pos�����߶ε�б��Խ�ӽ�����N
                contact.ra = contact.pos - c.bodyA->world();
                contact.rb = contact.pos - c.bodyB->world();
                c.contacts.push_back(contact);
            }
        }

        return true;
    }

    bool solve_collision(collision &c) {
        if (c.A.polygon.sat < c.B.polygon.sat) { // ���У�A��B��SAT���󣬸��ӽ���
            std::swap(c.bodyA, c.bodyB);
            std::swap(c.A, c.B);
        }
        return solve_collision_internal(c);
    }

    void collision_update(collision &c, const collision &old_c) {
        auto &a = *c.bodyA;
        auto &b = *c.bodyB;
        const auto &old_contacts = old_c.contacts;
        for (auto &new_contact : c.contacts) {
            auto old_contact = std::find(old_contacts.begin(), old_contacts.end(), new_contact);
            if (old_contact != old_contacts.end()) { // ͬһ����ײ��ĸ���
                new_contact.pn = old_contact->pn;
                new_contact.pt = old_contact->pt;

                auto tangent = c.N.normal(); // �µ�����
                auto p = new_contact.pn * c.N + new_contact.pt * tangent; // �µĳ���
                a.impulse(-p, new_contact.ra); // ʩ������
                b.impulse(p, new_contact.rb);
            }
        }
    }
}