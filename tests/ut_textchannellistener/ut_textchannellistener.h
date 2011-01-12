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

#ifndef UT_TEXTCHANNELLISTENER_H
#define UT_TEXTCHANNELLISTENER_H

// INCLUDES
#include <QObject>
#include <QEventLoop>

#include <CommHistory/Event>
#include <CommHistory/Group>

namespace RTComLogger {

class Ut_TextChannelListener : public QObject
{
    Q_OBJECT
public:
    Ut_TextChannelListener();
    ~Ut_TextChannelListener();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

// Test functions
private Q_SLOTS:
    void imSending();
    void receiving_data();
    void receiving();
    void smsSending_data();
    void smsSending();
    void voicemail();
    void receiveVCard();

private:
    CommHistory::Group fetchGroup(const QString &localUid, const QString &remoteUid, bool wait);
    CommHistory::Event fetchEvent(int eventId);
};

}
#endif // UT_TEXTCHANNELLISTENER_H
