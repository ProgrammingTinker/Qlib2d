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
def `latest-time 0d
def `latest-str ""
def `lyc-gen (\ `(til str) `(begin (box (attr `pos (random -0.2d 0.2d) (random -0.1d 0.1d)) `(size 0.04d 0.04d) `(mass 1d) (attr `text str)) (conf (attr `wait til))))
def `lyc-sen (\ `(L t SPAN) `(if (null? L) `nil `(begin (lyc-gen t (car L)) (lyc-sen (cdr L) (+ t SPAN) SPAN))))
def `lyc-pri (\ `(t s) `(begin (def `L (word latest-str)) (def `SPAN (- t latest-time)) (lyc-sen L latest-time (/ SPAN (+ 1 (len L)))) (def `latest-time t) (def `latest-str s) (conf `(clear) `(bound))))
def `lyc (\ `(m t s) `(lyc-pri (+ (* m 60.0) t) s))
lyc 0d 00.00d "�Ż���/������ - ˫Ӱ"
lyc 0d 03.00d ""
lyc 0d 04.00d "����ܲ�������Ӿ�������"
lyc 0d 05.00d "��������ޱ"
lyc 0d 06.00d "���ʣ��׼���"
lyc 0d 07.00d "�ıࣺ��ޱ/Jim Lee"
lyc 0d 08.00d ""
lyc 0d 14.70d "aMEI��"
lyc 0d 15.51d "���Ǻ�������ʲô����"
lyc 0d 23.45d "����˷��� �Բ���"
lyc 0d 33.42d "Sandy��"
lyc 0d 33.82d "����һ���Ŀ���������"
lyc 0d 41.23d "���������� ��Թ��"
lyc 0d 50.27d "aMEI��"
lyc 0d 51.36d "����������"
lyc 0d 54.98d "Sandy��"
lyc 0d 54.40d "������˫Ӱ"
lyc 0d 59.34d "aMEI��"
lyc 1d 00.40d "һ������"
lyc 1d 02.15d "Sandy��"
lyc 1d 02.50d "��һ������"
lyc 1d 08.47d "�ϣ�"
lyc 1d 08.98d "̰һ�㰮ʲôʹҲ����"
lyc 1d 16.48d "�û��ɽ��� ����"
lyc 1d 24.33d ""
lyc 1d 30.73d "Sandy��"
lyc 1d 31.24d "�谮�ͱ��� �����н�̸"
lyc 1d 37.31d "aMEI��"
lyc 1d 38.11d "˵������"
lyc 1d 39.52d "Sandy��"
lyc 1d 39.82d "����֤�ҵ���ů ����"
lyc 1d 42.91d "aMEI��"
lyc 1d 43.31d "�ζ�·��"
lyc 1d 48.98d "�����ҵ�����һ���µ�"
lyc 1d 54.69d "Sandy��"
lyc 1d 55.47d "˿������¹�"
lyc 1d 57.68d "aMEI��"
lyc 1d 57.98d "˵����ȡů"
lyc 2d 00.31d "�ϣ�"
lyc 2d 01.35d "˭��"
lyc 2d 05.29d "Sandy��"
lyc 2d 06.36d "����˭����"
lyc 2d 09.24d "aMEI��"
lyc 2d 09.64d "������˫Ӱ"
lyc 2d 15.62d "Sandy��"
lyc 2d 16.82d "һ������"
lyc 2d 17.65d "aMEI��"
lyc 2d 18.55d "��һ������"
lyc 2d 23.55d "�ϣ�"
lyc 2d 24.40d "ԶԶ����û���ֵ�δ��"
lyc 2d 32.24d "��ʱ��̻� Ҳ��"
lyc 2d 42.17d "Lalalalalalala"
lyc 3d 35.65d "���������"
lyc 3d 38.39d "������˫Ӱ"
lyc 3d 44.33d "һ���䵭��һ������"
lyc 3d 53.36d "��Ը������Զ���ڹ���"
lyc 4d 01.21d "��һ����� ����"
lyc 4d 09.87d "��һ���Ӵ� ����"
lyc 4d 22.55d ""
lyc 4d 23.88d ""
)");
        }
        return R"(+ __author__ " " __project__)";
    }
}