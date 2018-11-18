#include "stdafx.h"
#include "c2dworld.h"

namespace clib {

    QString clib::c2d_world::get_script(uint32_t id)
    {
        if (id == 2)
            return QString::fromLocal8Bit(R"(
map (\ `n `(box `(pos 0.0d 0.0d) `(size 0.04d 0.05d) `(mass 1d) `(text "��"))) (range 0 10)
conf `(cycle 20) `(move 0.2d 0.2d) `(gravity 0d 0d) `(rotate 90)
conf `(cycle 1)
conf `(cycle 10)
map (\ `n `(circle `(pos 0.0d 0.0d) `(r 0.025d) `(mass 1d) `(text "Բ"))) (range 0 5)
conf `(cycle 100)
map (\ `n `(tri `(pos 0.0d 0.0d) `(edge 0.04d 0.04d) `(angle 60d) `(mass 1d) `(text "��"))) (range 0 5)
conf `(gravity 0d -9.8d) `(cycle 10)
)");
        else if (id == 3) {
            /*return QString::fromLocal8Bit(R"(
map (\ `n `(box `(pos 0.0d 0.0d) `(size 0.04d 0.05d) `(mass 1d) `(text "��"))) (range 0 5)
conf `(cycle 20) `(move 0.2d 0.2d) `(gravity 0d 0d) `(rotate 90)
conf `(record) `(wait 2.0d)
map (\ `n `(quote ok)) (range 0 10)
conf `(cycle 10)
map (\ `n `(circle `(pos 0.0d 0.0d) `(r 0.025d) `(mass 1d) `(text "Բ"))) (range 0 5)
conf `(cycle 100)
map (\ `n `(tri `(pos 0.0d 0.0d) `(edge 0.04d 0.04d) `(angle 60d) `(mass 1d) `(text "��"))) (range 0 5)
conf `(gravity 0d -9.8d) `(cycle 10)
)");*/
            return QString::fromLocal8Bit(R"(
conf `(gravity 0d -0.8d) `(cycle 1000) `(record)
def `latest-len 0
def `latest-time 0d
def `latest-str ""
def `lyc-gen (\ `(til str) `(begin (def `latest-len (+ latest-len)) (conf (attr `wait til)) (box (attr `pos (random -0.2d 0.2d) (random -0.1d 0.1d)) `(size 0.04d 0.04d) `(mass 1d) (attr `text str))))
def `lyc-sen (\ `(L t SPAN) `(if (null? L) `(conf (attr `wait t)) `(begin (lyc-gen t (car L)) (lyc-sen (cdr L) (+ t SPAN) SPAN))))
def `lyc-pri (\ `(t s) `(begin (def `L (word latest-str)) (def `SPAN (- t latest-time)) (lyc-sen L latest-time (/ SPAN (+ 2 (len L)))) (def `latest-time t) (def `latest-str s) (if (> latest-len 10) `(begin (def `latest-len 0) (conf `(clear) `(bound))) `nil) (conf (attr `gravity (random -0.2d 0.2d) (random -1d 0.1d)))))
def `lyc (\ `(m t s) `(lyc-pri (+ (* m 60.0) t) s))
lyc 0d 00.00d "�Ż���/������ - ˫Ӱ"
lyc 0d 03.00d ""
lyc 0d 04.00d "����ܲ�������Ӿ�������"
lyc 0d 05.00d "��������ޱ"
lyc 0d 06.00d "���ʣ��׼���"
lyc 0d 07.00d "�ıࣺ��ޱ/Jim Lee"
lyc 0d 08.00d ""
lyc 0d 14.70d "���Ǻ�������ʲô����"
lyc 0d 23.45d "����˷��� �Բ���"
lyc 0d 33.42d "����һ���Ŀ���������"
lyc 0d 41.23d "���������� ��Թ��"
lyc 0d 50.27d "����������"
lyc 0d 54.98d "������˫Ӱ"
lyc 0d 59.34d "һ������"
lyc 1d 02.15d "��һ������"
lyc 1d 08.47d "̰һ�㰮ʲôʹҲ����"
lyc 1d 16.48d "�û��ɽ��� ����"
lyc 1d 24.33d ""
lyc 1d 30.73d "�谮�ͱ��� �����н�̸"
lyc 1d 37.31d "˵������"
lyc 1d 39.52d "����֤�ҵ���ů ����"
lyc 1d 42.91d "�ζ�·��"
lyc 1d 48.98d "�����ҵ�����һ���µ�"
lyc 1d 54.69d "˿������¹�"
lyc 1d 57.68d "˵����ȡů"
lyc 2d 00.31d "˭��"
lyc 2d 05.29d "����˭����"
lyc 2d 09.24d "������˫Ӱ"
lyc 2d 15.62d "һ������"
lyc 2d 17.65d "��һ������"
lyc 2d 23.55d "ԶԶ����û���ֵ�δ��"
lyc 2d 32.24d "��ʱ��̻� Ҳ��"
lyc 2d 42.17d "Lalalalalalala"
lyc 3d 35.65d "���������"
lyc 3d 38.39d "������˫Ӱ"
lyc 3d 44.33d "һ���䵭��һ������"
lyc 3d 53.36d "��Ը������Զ���ڹ���"
lyc 4d 01.21d "��һ����� ����"
lyc 4d 09.87d "��һ���Ӵ� ����"
lyc 4d 22.55d ""
lyc 4d 23.88d "��"
)");
        }
        else if (id == 4) {
            return QString::fromLocal8Bit(R"(
conf `(gravity 0d -0.4d) `(clear) `(cycle 1000) `(record)
def `latest-len 0
def `latest-time 0d
def `latest-str ""
def `lyc-gen (\ `(til str) `(begin (def `latest-len (+ latest-len)) (conf (attr `wait til)) (circle (attr `pos (random -0.3d 0.3d) (random 0.25d 0.3d)) `(r 0.02d) (attr `v (random -0.01d 0.01d) (random -0.01d -0.1d)) `(mass 1d) `(snow 0.3d) `(life 10d) (attr `text str))))
def `lyc-sen (\ `(L t SPAN) `(if (null? L) `(conf (attr `wait t)) `(begin (lyc-gen t (car L)) (lyc-sen (cdr L) (+ t SPAN) SPAN))))
def `lyc-pri (\ `(t s) `(begin (def `L (word latest-str)) (def `SPAN (- t latest-time)) (lyc-sen L latest-time (/ SPAN (+ 2 (len L)))) (def `latest-time t) (def `latest-str s) (if (> latest-len 10) `(begin (def `latest-len 0)) `nil)))
def `lyc (\ `(m t s) `(lyc-pri (+ (* m 60.0) t) s))
lyc 00d 01.60d "Ĭ - ��Ӣ"
lyc 00d 02.59d "��Ӱ����������Ĭ���������"
lyc 00d 05.56d "�ʣ���Լ"
lyc 00d 06.43d "����Ǯ��"
lyc 00d 07.58d ""
lyc 00d 39.14d "�̲�ס����һ����ִ����"
lyc 00d 44.52d ""
lyc 00d 46.10d "�������������ε���"
lyc 00d 50.59d ""
lyc 00d 53.18d "����ʱ���Ϸ�������"
lyc 00d 59.43d "��Ĭ�س�û�����"
lyc 01d 04.37d ""
lyc 01d 06.18d "���¼���"
lyc 01d 09.71d "��ֻ��� ʧȥ��"
lyc 01d 14.49d ""
lyc 01d 19.95d "�ұ����д�����¼�"
lyc 01d 25.69d "������ ������"
lyc 01d 33.74d "���»������Բ"
lyc 01d 37.23d "�ļ������Ե"
lyc 01d 40.43d "����"
lyc 01d 41.79d ""
lyc 01d 47.39d "Ϊ�ΰ��д������˼�"
lyc 01d 53.07d "������ �Ӳ���"
lyc 01d 59.65d ""
lyc 02d 01.07d "üͷ�ⲻ���Ľ�"
lyc 02d 04.53d "���нⲻ���Ľ�"
lyc 02d 07.77d "����"
lyc 02d 09.78d ""
lyc 02d 17.50d "�� ʧȥ��"
lyc 02d 27.27d ""
lyc 02d 29.12d "�� ʧȥ��"
lyc 02d 41.72d ""
lyc 03d 13.42d "�̲�ס����һ����ִ����"
lyc 03d 18.89d ""
lyc 03d 20.63d "�������������ε���"
lyc 03d 25.62d ""
lyc 03d 27.30d "����ʱ���Ϸ�������"
lyc 03d 32.28d ""
lyc 03d 33.79d "��Ĭ�س�û�����"
lyc 03d 38.46d ""
lyc 03d 40.42d "�ܶ���ʼ"
lyc 03d 42.91d ""
lyc 03d 44.13d "��ֻ��� ʧȥ��"
lyc 03d 48.81d ""
lyc 03d 54.58d "�ұ����д�����¼�"
lyc 03d 59.92d "������ ������"
lyc 04d 06.47d ""
lyc 04d 07.89d "���»������Բ"
lyc 04d 11.50d "�ļ������Ե"
lyc 04d 14.54d "����"
lyc 04d 16.74d ""
lyc 04d 21.58d "Ϊ�ΰ��д������˼�"
lyc 04d 27.29d "������ �Ӳ���"
lyc 04d 34.61d ""
lyc 04d 35.55d "üͷ�ⲻ���Ľ�"
lyc 04d 38.98d "���нⲻ���Ľ�"
lyc 04d 42.14d "����"
lyc 04d 44.82d ""
lyc 04d 49.39d "�� ʧȥ��"
lyc 05d 01.36d ""
lyc 05d 03.18d "�� ��ʧȥ��"
lyc 05d 09.18d ""
)");
        }
        return R"(+ __author__ " " __project__)";
    }
}