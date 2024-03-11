/******************************************************************************
**
** This file is part of commhistory-daemon.
**
** Copyright (C) 2024 Damien Caliste <dcaliste@free.fr>
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

#include "voicecallfilterhandler.h"
#include "debug.h"

#include <CommHistory/event.h>
#include <CommHistory/eventmodel.h>
#include <CommHistory/commonutils.h>

using namespace CommHistory;

VoiceCallFilterHandler::VoiceCallFilterHandler(QObject* parent)
    : QObject(parent)
    , m_ofonoManager(QOfonoManager::instance())
{
    QOfonoManager* ofonoManager = m_ofonoManager.data();

    connect(ofonoManager, &QOfonoManager::modemAdded,
            this, &VoiceCallFilterHandler::onModemAdded);
    connect(ofonoManager, &QOfonoManager::modemRemoved,
            this, &VoiceCallFilterHandler::onModemRemoved);

    if (ofonoManager->available()) {
        onOfonoAvailableChanged(true);
    }
    connect(ofonoManager, &QOfonoManager::availableChanged,
            this, &VoiceCallFilterHandler::onOfonoAvailableChanged);
}

void VoiceCallFilterHandler::onOfonoAvailableChanged(bool available)
{
    qCDebug(lcCommhistoryd) << "VoiceCallFilterHandler: ofono available changed to" << available;
    if (available) {
        for (const QString path : m_ofonoManager->modems()) {
            addModem(path);
        }
    } else {
        qDeleteAll(m_modems.values());
        m_modems.clear();
    }
}

void VoiceCallFilterHandler::onModemAdded(QString path)
{
    qCDebug(lcCommhistoryd) << "VoiceCallFilterHandler: onModemAdded" << path;
    addModem(path);
}

void VoiceCallFilterHandler::onModemRemoved(QString path)
{
    qCDebug(lcCommhistoryd) << "VoiceCallFilterHandler: onModemRemoved" << path;
    delete m_modems.take(path);
}


void VoiceCallFilterHandler::addModem(const QString &path)
{
    if (m_modems.contains(path))
        return;

    qCDebug(lcCommhistoryd) << "VoiceCallFilterHandler: addModem" << path;

    QOfonoVoiceCallManager *vc = new QOfonoVoiceCallManager(this);
    vc->setModemPath(path);
    connect(vc, &QOfonoVoiceCallManager::callFiltered,
            this, &VoiceCallFilterHandler::onCallFiltered);

    m_modems.insert(path, vc);
}

void VoiceCallFilterHandler::onCallFiltered(QOfonoVoiceCallManager::FilterType filter,
                                            const QString &from)
{
    QOfonoVoiceCallManager *vc = qobject_cast<QOfonoVoiceCallManager*>(sender());
    const QString ringAccountPath = RING_ACCOUNT + vc->modemPath();

    qCDebug(lcCommhistoryd) << "NotificationManager::slotCallFiltered:" << from;

    Event event;
    event.setType(Event::CallEvent);
    event.setStartTime(QDateTime::currentDateTime());
    event.setEndTime(event.startTime());
    event.setDirection(Event::Inbound);
    event.setLocalUid(ringAccountPath);
    event.setRecipients(Recipient(ringAccountPath, from));
    switch (filter) {
    case QOfonoVoiceCallManager::Ignored:
        event.setExtraProperty("FilterType", "ignored");
        break;
    case QOfonoVoiceCallManager::Blocked:
        event.setExtraProperty("FilterType", "blocked");
        break;
    default:
        break;
    }

    EventModel model;
    if (!model.addEvent(event)) {
        qWarning(lcCommhistoryd) << "Failed to save filtered call event; message dropped" << event.toString();
    }
}
