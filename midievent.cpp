/****************************************************************************
**
** Copyright (C) 2018 Robert Vetter.
**
** This file is part of the MagicstompFrenzy - an editor for Yamaha Magicstomp
** effect processor
**
** THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
** ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
** IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
** PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version . The licenses are
** as published by the Free Software Foundation and appearing in the file LICENSE
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**/
#include "midievent.h"

MidiEvent::MidiEvent(MidiEventType metype, quint8 ch, quint8 d1, quint8 d2)
    :QEvent(QEvent::User)
{
    status = metype << 4;
    status |= (ch & 0x0F);
    data1 = d1;
    data2 = d2;
    if(metype == SystemCommon)
        status &= Reset;
}

MidiEvent::MidiEvent(SystemCommonType systype)
    :QEvent(QEvent::User)
{
    status = systype;
    data1 = 0;
    data2 = 0;
    if(systype == SysEx)
        dataArrayPtr = new QByteArray();
}

MidiEvent::MidiEvent(const MidiEvent &other) : QEvent(other)
{
    status = other.status;
    data1 = other.data1;
    data2 = other.data2;
    dataArrayPtr = other.dataArrayPtr;
    if(other.dataArrayPtr != nullptr)
        dataArrayPtr = new QByteArray(*other.dataArrayPtr);
}
MidiEvent& MidiEvent::operator=(const MidiEvent &other)
{
    QEvent::operator=(other);

    status = other.status;
    data1 = other.data1;
    data2 = other.data2;
    dataArrayPtr = other.dataArrayPtr;
    if(other.dataArrayPtr != nullptr)
        dataArrayPtr = new QByteArray(*other.dataArrayPtr);

    return *this;
}

MidiEvent::~MidiEvent()
{
    if(dataArrayPtr != nullptr)
        delete dataArrayPtr;
}

bool MidiEvent::isUniversalSysEx() const
{
    if(! (status == SysEx))
        return false;

    if(dataArrayPtr->isEmpty())
        return false;

    if(dataArrayPtr->at(1) == 0x7E || dataArrayPtr->at(1) == 0x7F)
        return true;

    return false;
}

bool MidiEvent::isIdentityRequest() const
{
    if(! isUniversalSysEx())
        return false;

    if(dataArrayPtr->size() != 6)
        return false;

    if(dataArrayPtr->at(3) == 0x06 && dataArrayPtr->at(4) == 0x01)
        return true;

    return false;
}

bool MidiEvent::isIdentityResponse() const
{
    if(! isUniversalSysEx())
        return false;

    if( !( dataArrayPtr->size() == 15 || dataArrayPtr->size() == 17)) // Manufacterer ID can be 1 or 3 bytes long
        return false;

    if(dataArrayPtr->at(3) == 0x06 && dataArrayPtr->at(4) == 0x02)
        return true;

    return false;
}
