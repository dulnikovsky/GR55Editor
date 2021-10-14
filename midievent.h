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
#ifndef MIDIEVENT_H
#define MIDIEVENT_H

#include <QEvent>
//#include "userevents.h"
#include "midiportidalsa.h"

class MidiEvent : public QEvent
{
    Q_GADGET
public:

    enum MidiEventType { NoteOff=0x08, NoteOn, PolyphonicKeyPressure, ControlChange, ProgramChange, ChannelPressure, PitchBend, SystemCommon };
    enum SystemCommonType { SysEx, MIDITimeCodeQuarterFrame, SongPositionPointer, SongSelect, Reserved1, Reserved2, TuneRequest, EOX,
                          TimingClock, Reserved3, Start, Continue, Stop, Reserved4, ActiveSensing, Reset};

    explicit MidiEvent(MidiEventType metype, quint8 channel = 0, quint8 data1 =0, quint8 data2 = 0);
    explicit MidiEvent(SystemCommonType systype);
    MidiEvent(const MidiEvent &other);
    MidiEvent &operator=(const MidiEvent &other);

    ~MidiEvent();

    inline MidiEventType midiType() const { return static_cast<MidiEventType> (status>>4); }

    inline quint8 Status() const { return status >> 4; }
    inline quint8 Channel() const { return status & 0x0F; }

    inline quint8 Data1() const { return data1; }
    inline quint8 Data2() const { return data2; }

    inline void setData1( quint8 data) { data1 = data; }
    inline void setData2( quint8 data) { data2 = data; }

    inline MidiClientPortId Port() const { return port; }
    inline void setPort(MidiClientPortId id) { port=id; }

    inline QByteArray* variableData() const { return dataArrayPtr; }

    bool isUniversalSysEx() const;

    bool isIdentityRequest() const;
    bool isIdentityResponse() const;

private:
    QByteArray *dataArrayPtr{nullptr};
    quint8 status;
    quint8 data1;
    quint8 data2;

    MidiClientPortId port;
};

#endif // MIDIEVENT_H
