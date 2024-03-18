// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef ADDEMPLOYEE_H
#define ADDEMPLOYEE_H

#include <qwidget.h>
#include <QScopedPointer>

namespace Ui
{
class AddEmployee;
}

/**
 * @todo write docs
 */
class AddEmployee : public QWidget
{
    Q_OBJECT

private:
    QScopedPointer<Ui::AddEmployee> m_ui;
};

#endif // ADDEMPLOYEE_H
