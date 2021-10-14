#ifndef GR55EDITORAPPLICATION_H
#define GR55EDITORAPPLICATION_H

#include <QGuiApplication>
#include <QQueue>

#include <alsa/asoundlib.h>
#include "midiportidalsa.h"

class MidiEvent;
class MidiInThread;
class QTimer;

class Patch;
class QStringListModel;

class GR55EditorApplication : public QGuiApplication
{
    Q_OBJECT
public:
    GR55EditorApplication(int &argc, char **argv, const char *appName);

    ~GR55EditorApplication() override;

    int sendMidiEvent(const MidiEvent *midiEvent);
    void scan();

    Patch * TemporaryPatch() const { return tmpPatch;}

protected:

    bool event(QEvent *e) override;

private slots:
    void timeOutSlot();

    void requestPatch(quint32 baseAdress, const Patch *patch);

    void onTemporaryPatchChanged(unsigned int address, int length, bool byUserControl);

private:
    snd_seq_t *handle;
    MidiClientPortId thisMidiPort;
    MidiClientPortId gr55MidiPort;

    unsigned char deviceID{0x10};

    MidiInThread *midiinthread{nullptr};

    QTimer *timeOutTimer{nullptr};

    QQueue<MidiEvent *> reqQueue;

    void processRequests();

    void requestData(quint32 address, quint32 length);

    Patch *tmpPatch;

    const int gr55SysExHeaderSize = 6;
    const unsigned char gr55SysExHeader[6]={0xF0, 0x41, 0x10, 0x00, 0x00, 0x53};

    static inline void uint32To7bit(quint32 val, char *dest)
    {
        *dest++ = (val >> (24-3)) & 0x7F;
        *dest++ = (val >> (16-2)) & 0x7F;
        *dest++ = (val >> (8-1)) & 0x7F;
        *dest++ = val  & 0x7F;
    }

    constexpr quint32 sevenBitToUint32( unsigned char msb, unsigned char mcleft, unsigned char mcright, unsigned char lsb) const
    {
        quint32 val=0;
        val |= (static_cast<quint32>(msb))<<(24-3);
        val |= (static_cast<quint32>(mcleft))<<(16-2);
        val |= (static_cast<quint32>(mcright))<<(8-1);
        val |= (static_cast<quint32>(lsb))<<(0);
        return val;
    }
};

#endif // GR55EDITORAPPLICATION_H
