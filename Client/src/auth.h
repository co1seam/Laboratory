// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef AUTH_H
#define AUTH_H

#include <qwidget.h>
#include <QScopedPointer>

namespace Ui
{
class Auth;
}

/**
 * @todo write docs
 */
class Auth : public QWidget
{
    Q_OBJECT

private:
    QScopedPointer<Ui::Auth> m_ui;
};

#endif // AUTH_H
