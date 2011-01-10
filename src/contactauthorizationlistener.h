/******************************************************************************
**
** This file is part of commhistory-daemon.
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Alexander Shalamov <alexander.shalamov@nokia.com>
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License version 2.1 as
** published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
** or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
** License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this library; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
**
******************************************************************************/

#ifndef CONTACTAUTHORIZATIONLISTENER_H
#define CONTACTAUTHORIZATIONLISTENER_H

#include <QObject>
#include <QStringList>

// Tp
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/AccountManager>

namespace RTComLogger
{
class ConnectionUtils;

class ContactAuthorizationListener : public QObject
{
    Q_OBJECT

public:
    explicit ContactAuthorizationListener(ConnectionUtils *connectionUtils,
                                          QObject *parent = 0);

private Q_SLOTS:
    void slotConnectionReady(const Tp::ConnectionPtr& connection);
    void slotShowUnableToAuthorizeDialog(const QString&, const QString&, const QString&,
                                         const QString&, const QString&, const QString&);

private:
    ConnectionUtils *m_pConnectionUtils;
};

} // namespace RTComLogger

#endif // CONTACTAUTHORIZATIONLISTENER_H