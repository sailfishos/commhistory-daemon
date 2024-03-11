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

#ifndef VOICECALLFILTERHANDLER_H
#define VOICECALLFILTERHANDLER_H

#include <QHash>
#include <QSharedPointer>
#include <QObject>

#include <qofonomanager.h>
#include <qofonovoicecallmanager.h>

class VoiceCallFilterHandler : public QObject
{
    Q_OBJECT

public:
    explicit VoiceCallFilterHandler(QObject *parent);

private Q_SLOTS:
    void onOfonoAvailableChanged(bool available);
    void onModemAdded(QString path);
    void onModemRemoved(QString path);
    void onCallFiltered(QOfonoVoiceCallManager::FilterType filter,
                        const QString &from);

private:
    void addModem(const QString &path);

private:
    QSharedPointer<QOfonoManager> m_ofonoManager;
    QHash<QString, QOfonoVoiceCallManager*> m_modems;
};

#endif // VOICECALLFILTERHANDLER_H
