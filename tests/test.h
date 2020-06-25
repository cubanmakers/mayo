/****************************************************************************
** Copyright (c) 2020, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include <QtCore/QObject>
#include <QtTest/QtTest>

namespace Mayo {

class Test : public QObject {
    Q_OBJECT
private slots:
    void Application_test();
    void IO_test();
    void IO_test_data();
    void BRepUtils_test();
    void CafUtils_test();
    void MeshUtils_test();
    void MeshUtils_test_data();
    void MeshUtils_orientation_test();
    void MeshUtils_orientation_test_data();
    void Quantity_test();
    void Result_test();
    void StringUtils_append_test();
    void StringUtils_append_test_data();
    void StringUtils_text_test();
    void StringUtils_text_test_data();
    void UnitSystem_test();
    void UnitSystem_test_data();

    void LibTask_test();
    void LibTree_test();
};

} // namespace Mayo
