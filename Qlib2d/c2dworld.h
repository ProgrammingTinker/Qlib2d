//
// Project: clib2d
// Created by bajdcc
//

#ifndef CLIB2D_C2DWORLD_H
#define CLIB2D_C2DWORLD_H

#include "c2dbody.h"
#include "c2djoint.h"
#include "c2dpolygon.h"
#include "c2dcircle.h"
#include "c2drevolute.h"
#include "c2dcollision.h"
#include "cvm.h"
#include "cparser.h"


class Q2dHelper;

namespace clib {

    class c2d_world {
    public:
        c2d_world() = default;
        ~c2d_world() = default;

        c2d_polygon *make_polygon(decimal mass, const std::vector<v2> &vertices, const v2 &pos, bool statics = false);
        c2d_polygon *make_rect(decimal mass, decimal w, decimal h, const v2 &pos, bool statics = false);
        c2d_circle *make_circle(decimal mass, decimal r, const v2 &pos, bool statics = false);
        c2d_revolute_joint *make_revolute_joint(c2d_body *a, c2d_body *b, const v2 &anchor);

        // ����λ���ҵ�����
        c2d_body *find_body(const v2 &pos);

        static uint32_t make_id(uint16_t a, uint16_t b);
        bool collision_detection(const c2d_body::ptr &a, c2d_body::ptr &b);
        decltype(auto) sleep_bodies() const;
        // ��ײ���
        void collision_detection();
        // https://github.com/erincatto/Box2D/blob/master/Box2D/Dynamics/Contacts/b2ContactSolver.cpp#L127
        // ��ײ����׼��
        void collision_prepare(collision &c);

        // ������ײ���
        void draw_collision(Q2dHelper * helper, const collision &c);

        // https://github.com/erincatto/Box2D/blob/master/Box2D/Dynamics/Contacts/b2ContactSolver.cpp#L324
        // ��ײ����
        void collision_update(collision &c);

#if ENABLE_SLEEP
        // ȥ�������������ײ
        void collision_remove_sleep();
#endif

        void step(Q2dHelper * helper);
        void move(const v2 &v);
        void rotate(decimal d);
        void offset(const v2 &pt, const v2 &offset);
        void mouse(const v2 &pt, bool down);
        void motion(const v2 &pt);

        // �����������
        void clear();

        // �������ܱ߽�
        void make_bound();

        // ����
        void scene(int id);

        // ��ʼ��
        void init();

        void exec(QString & str);

        void toggle_pause();
        void toggle_gravity();

        size_t get_collision_size() const;
        size_t get_sleeping_size() const;
        void invert_gravity();
        void set_gravity(const v2 & v);
        void set_cycle(int cycle);

        void set_helper(Q2dHelper * helper);

    private:
        void start_animation(uint32_t id);
        void stop_animation();
        void run_animation();

        static QString scene_7();

    public:
        static QTime last_clock;
        static decimal dt;
        static decimal dt_inv;
        static bool paused; // �Ƿ���ͣ
        static QString title; // ����

    private:
        uint32_t animation_id{0};
        QString animation_code;
        QQueue<QString> animation_queue;
        cvm vm;
        std::unique_ptr<cparser> parser;

        Q2dHelper *helper;
        int cycle{ LISP_CYCLE };

        bool mouse_drag{false};
        v2 global_drag; // ����϶�
        v2 global_drag_offset; // ����϶�λ��

        // uint32_t ���� a��b ��������� id ���
        std::unordered_map<uint32_t, collision> collisions; // ��ײ���

        std::vector<c2d_body::ptr> bodies; // Ѱ������
        std::vector<c2d_body::ptr> static_bodies; // ��̬����
        std::vector<c2d_joint::ptr> joints; // �ؽ�
        uint16_t global_id{1};
        v2 gravity{0, GRAVITY}; // ����
    };

    extern c2d_world *world;
}

#endif //CLIB2D_C2DWORLD_H
