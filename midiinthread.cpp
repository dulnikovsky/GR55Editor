
#include "midiinthread.h"
#include <QByteArray>
#include <QCoreApplication>
#include "midievent.h"
#include "midiportidalsa.h"

MidiInThread::MidiInThread(snd_seq_t *h, QObject *parent)
    :QThread(parent), handle(h)
{

}

MidiInThread::~MidiInThread()
{

}

void MidiInThread::run()
{
    snd_seq_event_t *ev;
    MidiEvent *midievent = nullptr;

    while (snd_seq_event_input(handle, &ev) >= 0)
    {
        if(ev->type==SND_SEQ_EVENT_SYSEX)
        {
            QByteArray arr(static_cast<char *>(ev->data.ext.ptr), static_cast<int>(ev->data.ext.len));
            if( midievent != nullptr)
            {
                QByteArray *data = midievent->variableData();
                data->append(arr);
            }
            else
            {
                midievent = new MidiEvent(MidiEvent::SysEx);
                MidiClientPortId port(ev->source.client, ev->source.port);
                midievent->setPort(port);
                QByteArray *data = midievent->variableData();
                *data=arr;
            }
            if(static_cast<unsigned char>(arr.at(arr.size()-1)) == 0xF7)
            {
                QCoreApplication::postEvent(parent(), midievent, Qt::HighEventPriority);
                midievent=nullptr;
            }
        }
        snd_seq_free_event(ev);
    }
}
