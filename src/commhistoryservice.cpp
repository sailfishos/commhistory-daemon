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

#include <QtDBus>
#include <MRemoteAction>
#include "commhistoryservice.h"
#include "notificationmanager.h"
#include "constants.h"

CommHistoryService::CommHistoryService( QObject* parent )
    : QObject(parent),
      m_IsRegistered(false)
{
    if (parent) {
        if(!QDBusConnection::sessionBus().registerObject(COMM_HISTORY_OBJECT_PATH, this)) {
            qWarning() << "Object registration failed!";
        } else {
            if(!QDBusConnection::sessionBus().registerService(COMM_HISTORY_SERVICE_NAME)) {
                qWarning() << "Unable to register commhistory service!"
                           << QDBusConnection::sessionBus().lastError();
            } else {
                m_IsRegistered = true;
            }
        }
    }
}

CommHistoryService::~CommHistoryService()
{
    QDBusConnection::sessionBus().unregisterObject(COMM_HISTORY_OBJECT_PATH);
    QDBusConnection::sessionBus().unregisterService(COMM_HISTORY_SERVICE_NAME);
}

void CommHistoryService::activateNotification(int groupId, const QString& remoteActionString)
{
    qDebug() << __PRETTY_FUNCTION__;

    RTComLogger::NotificationManager* nm =
                                   RTComLogger::NotificationManager::instance();

    if (groupId == CommHistory::Event::VoicemailEvent ||
        nm->removeNotificationGroup(groupId)) {
        MRemoteAction remoteAction(remoteActionString);
        remoteAction.trigger();
    } else
        qWarning() << "NotificationGroup" << groupId << "doesn't exist";
}

void CommHistoryService::activateAuthorization(const QString& contactId,
                                               const QString& accountPath,
                                               const QString& filename,
                                               const QString& message,
                                               const QString& transactionId,
                                               const QString& accountUniqueIdentifier)
{
    Q_EMIT showAuthorizationDialog(contactId, accountPath, filename, message,
                                  transactionId, accountUniqueIdentifier);
}

bool CommHistoryService::isRegistered()
{
    return m_IsRegistered;
}