#include "gr55editorapplication.h"
#include "midievent.h"
#include "midiinthread.h"
#include "alsafunc.h"
#include "patch.h"

#include <QTimer>
#include <QDebug>

#include <QStringListModel>

GR55EditorApplication::GR55EditorApplication(int &argc, char **argv, const char *appName)
    :QGuiApplication(argc, argv)
{

    snd_seq_open(&handle, "default", SND_SEQ_OPEN_DUPLEX, 0);

    snd_seq_set_client_name(handle, appName);

    unsigned char midiPort = snd_seq_create_simple_port(handle, "MIDI",
                    SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_READ| SND_SEQ_PORT_CAP_SUBS_WRITE | SND_SEQ_PORT_CAP_SUBS_READ,
                    SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);

    thisMidiPort = MidiClientPortId(snd_seq_client_id(handle), midiPort);
    subscribePort(handle, MidiClientPortId(SND_SEQ_CLIENT_SYSTEM, SND_SEQ_PORT_SYSTEM_ANNOUNCE), thisMidiPort);
    setApplicationName(appName);

    midiinthread = new MidiInThread(handle, this);
    midiinthread->start();

    timeOutTimer = new QTimer(this);
    timeOutTimer->setInterval(250);
    timeOutTimer->setSingleShot(true);
    connect(timeOutTimer, &QTimer::timeout, this, &GR55EditorApplication::timeOutSlot);

    tmpPatch = new Patch(this);
    connect(tmpPatch, &Patch::dataChanged, this, &GR55EditorApplication::onTemporaryPatchChanged);

    scan();
}

GR55EditorApplication::~GR55EditorApplication()
{
    midiinthread->terminate();
    midiinthread->wait();
}

bool GR55EditorApplication::event(QEvent *e)
{
    if(e->type() == QEvent::User)
    {
        MidiEvent *me = dynamic_cast<MidiEvent *>(e);
        if(me == nullptr)
            return QGuiApplication::event(e);

        if(!reqQueue.empty())
        {
            MidiEvent *reqEvent = reqQueue.dequeue();
            if(reqEvent->isIdentityRequest() && me->isIdentityResponse())
            {
                QByteArray *sysExdata = me->variableData();
                if(sysExdata->at(5) == 0x41 &&
                        sysExdata->at(6) == 0x53 &&
                        sysExdata->at(7) == 0x02
                        )
                qDebug("Found GR-55");
                if(! gr55MidiPort.isValid())
                {
                    deviceID = sysExdata->at(2);
                    gr55MidiPort = me->Port();

                    requestPatch(sevenBitToUint32(0x18, 0x00, 0x00, 0x00), tmpPatch);
                }
            }
            else if(me->variableData() != nullptr)
            {
               QByteArray *sysExdata = me->variableData();
               if( sysExdata->size() > 12 &&
                   sysExdata->at(sysExdata->size()-1) == static_cast<char>(0xF7) &&
                   reqEvent->variableData()->left(gr55SysExHeaderSize) == sysExdata->left(gr55SysExHeaderSize) &&
                   sysExdata->at(gr55SysExHeaderSize) == static_cast<char>(0x12) //&&
                       //check checksum
                       )
               {
                   quint8 a1 = sysExdata->at(gr55SysExHeaderSize+1);
                   quint8 a2 = sysExdata->at(gr55SysExHeaderSize+2);
                   quint8 a3 = sysExdata->at(gr55SysExHeaderSize+3);
                   quint8 a4 = sysExdata->at(gr55SysExHeaderSize+4);

                   if(a1 == 0x18 && a2 == 0x00 )
                       tmpPatch->setData(sevenBitToUint32(0x00, 0x00, a3, a4), sysExdata->mid(gr55SysExHeaderSize+1+4, sysExdata->size()-(gr55SysExHeaderSize+1+4)-2), false);
                   //else
                   // user patches

                   qDebug() << sysExdata->toHex();
               }
            }
            delete reqEvent;
            processRequests();
        }
        return true;
    }
    else

        return QGuiApplication::event(e);
}

void GR55EditorApplication::scan()
{
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;

    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);

    //Subscribe all readable hardware ports
    snd_seq_client_info_set_client(cinfo, -1);
    while (snd_seq_query_next_client(handle, cinfo) >= 0)
    {
        int clientId = snd_seq_client_info_get_client(cinfo);
        if( (clientId == SND_SEQ_CLIENT_SYSTEM) || (clientId == snd_seq_client_id(handle)))
            continue;


        snd_seq_port_info_set_client(pinfo, clientId);
        snd_seq_port_info_set_port(pinfo, -1);
        while (snd_seq_query_next_port(handle, pinfo) >= 0)
        {
            MidiClientPortId cpid(clientId, snd_seq_port_info_get_port(pinfo));
            unsigned int cap = snd_seq_port_info_get_capability(pinfo);

            if((snd_seq_port_info_get_type(pinfo) & SND_SEQ_PORT_TYPE_HARDWARE) == SND_SEQ_PORT_TYPE_HARDWARE &&
                    cap & (SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ))
            {

                subscribePort(handle, cpid, thisMidiPort );
            }
        }
    }

    snd_seq_client_info_set_client(cinfo, -1);
    while (snd_seq_query_next_client(handle, cinfo) >= 0)
    {
        int clientId = snd_seq_client_info_get_client(cinfo);
        if( (clientId == SND_SEQ_CLIENT_SYSTEM) || (clientId == snd_seq_client_id(handle)))
            continue;


        snd_seq_port_info_set_client(pinfo, clientId);
        snd_seq_port_info_set_port(pinfo, -1);
        while (snd_seq_query_next_port(handle, pinfo) >= 0)
        {
            MidiClientPortId cpid(clientId, snd_seq_port_info_get_port(pinfo));
            unsigned int cap = snd_seq_port_info_get_capability(pinfo);

            if((snd_seq_port_info_get_type(pinfo) & SND_SEQ_PORT_TYPE_HARDWARE) == SND_SEQ_PORT_TYPE_HARDWARE &&
                    cap & (SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE))
            {
                subscribePort(handle, thisMidiPort, cpid );

                const unsigned char identityRequest[6] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7 };

                MidiEvent *event = new MidiEvent(MidiEvent::SysEx);
                event->setPort(cpid);
                QByteArray *sysexArr = event->variableData();
                sysexArr->append((const char *)identityRequest, 6);
                reqQueue.enqueue(event);
            }
        }
    }
    processRequests();
}

int GR55EditorApplication::sendMidiEvent(const MidiEvent *midiEvent)
{
    snd_seq_event_t sendev;
    snd_seq_ev_clear(&sendev);
    snd_seq_ev_set_source(&sendev, thisMidiPort.portId());
    snd_seq_ev_set_dest(&sendev, midiEvent->Port().clientId(), midiEvent->Port().portId());
    snd_seq_ev_set_direct(&sendev);

    QByteArray *sysexArr = const_cast<MidiEvent *>(midiEvent)->variableData();

    snd_seq_ev_set_variable(&sendev, sysexArr->size(), (void *) sysexArr->constData());
    sendev.type=SND_SEQ_EVENT_SYSEX;

    int ret;
    ret = snd_seq_event_output(handle, &sendev);
    if( ret <= 0)
        return ret;
    return snd_seq_drain_output(handle);
}

void GR55EditorApplication::processRequests()
{
    if(reqQueue.empty())
    {
        timeOutTimer->stop();
        return;
    }
    sendMidiEvent(reqQueue.head());
    timeOutTimer->start();
}

void GR55EditorApplication::timeOutSlot()
{
    qDebug("TimeOut");
    MidiEvent *reqEvent = reqQueue.dequeue();
    if(reqEvent->isIdentityRequest())
    {
        unSubscribePort(handle, thisMidiPort, reqEvent->Port());
        unSubscribePort(handle, reqEvent->Port(), thisMidiPort);
    }
    delete reqEvent;
    processRequests();
}

void GR55EditorApplication::requestPatch(quint32 baseAdress, const Patch *patch)
{
    QList<QPair<quint32, quint32>> list = patch->getOffsetLenghtList();

    for(int i=0; i< list.size(); i++)
    {
        requestData(baseAdress + list.at(i).first, list.at(i).second);
    }
}

void GR55EditorApplication::requestData(quint32 address, quint32 length)
{
    char tmpArr[4];

    MidiEvent *event = new MidiEvent(MidiEvent::SysEx);
    event->setPort(gr55MidiPort);
    QByteArray *sysexArr = event->variableData();
    *sysexArr = QByteArray::fromRawData((const char *)gr55SysExHeader, gr55SysExHeaderSize);
    sysexArr->data()[2] = deviceID;
    sysexArr->append(0x11); //request

    uint32To7bit(address, tmpArr);
    sysexArr->append(tmpArr, 4);

    uint32To7bit(length, tmpArr);
    sysexArr->append(tmpArr, 4);

    sysexArr->append(char(0));
    sysexArr->append(0xF7);
    reqQueue.enqueue(event);

    processRequests();
}

void GR55EditorApplication::onTemporaryPatchChanged(unsigned int address, int length, bool byUserControl)
{
    char *data = tmpPatch->data(address, length);
    if(data == nullptr)
        return;

    char tmpArr[4];

    MidiEvent event(MidiEvent::SysEx);
    event.setPort(gr55MidiPort);
    QByteArray *sysexArr = event.variableData();
    *sysexArr = QByteArray::fromRawData((const char *)gr55SysExHeader, gr55SysExHeaderSize);
    sysexArr->data()[2] = deviceID;
    sysexArr->append(0x12); //data

    uint32To7bit(address, tmpArr);
    tmpArr[0] = 0x18;
    sysexArr->append(tmpArr, 4);

    uint32To7bit(length, tmpArr);
    sysexArr->append(data, length);

    sysexArr->append(char(0));
    sysexArr->append(0xF7);

    sendMidiEvent(&event);
}


