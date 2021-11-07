#include "patchpcmwrapper.h"
#include "patch.h"

int PatchPcmWrapper::getWaveform() const
{
    char *data = patch->data(baseOffset+Waveform, 2);
    if(data == nullptr)
        return -1;
    int val = (data[0] << 7 | data[1]);
    return val;
}
void PatchPcmWrapper::setWaveform( int val)
{
    QByteArray arr;
    arr.append(((val >> 7) & 0x7F));
    arr.append((val & 0x7F));
    patch->setData(baseOffset+Waveform, arr, true);
    emit waveformChanged(val);
}

bool PatchPcmWrapper::getToneSwitch() const
{
    char *data = patch->data(baseOffset+ToneSwitch, 1);
    if(data == nullptr)
        return false;
    bool val = data[0];
    return !val;
}

void PatchPcmWrapper::setToneSwitch(bool toneswitch)
{
    if (getToneSwitch() == toneswitch)
        return;

    patch->setData(baseOffset+ToneSwitch, (quint8)(!toneswitch), true);
    emit toneswitchChanged(toneswitch);
}

int PatchPcmWrapper::getLevel() const
{
    char *data = patch->data(baseOffset+ToneLevel, 1);
    if(data == nullptr)
        return -1;
    return ((((int)data[0])+1)*100)/127;
}
void PatchPcmWrapper::setLevel(int newVal)
{
    if(newVal == getLevel())
        return;

    quint8 newval8 = (newVal*127)/100;
    patch->setData(baseOffset+ToneLevel, newval8, true);
    emit levelChanged(newVal);
}


int PatchPcmWrapper::getOctave() const
{
    char *data = patch->data(baseOffset+Octave, 1);
    if(data == nullptr) return 0;

    return (int)data[0]-0x40;
}
void PatchPcmWrapper::setOctave(int newVal)
{
    if(newVal == getOctave())
        return;

    quint8 newval8 = newVal+0x40;
    patch->setData(baseOffset+Octave, newval8, true);
    emit octaveChanged(newVal);
}

bool PatchPcmWrapper::getChromatic() const
{
    char *data = patch->data(baseOffset+Chromatic, 1);
    if(data == nullptr) return false;
    bool val = data[0];
    return val;
}

void PatchPcmWrapper::setChromatic(bool val)
{
    if (getChromatic() == val) return;
    patch->setData(baseOffset+Chromatic, (quint8)(val), true);
    emit chromaticChanged(val);
}

bool PatchPcmWrapper::getLegato() const
{
    char *data = patch->data(baseOffset+Legato, 1);
    if(data == nullptr) return false;
    bool val = data[0];
    return val;
}

void PatchPcmWrapper::setLegato(bool val)
{
    if (getLegato() == val) return;
    patch->setData(baseOffset+Legato, (quint8)(val), true);
    emit legatoChanged(val);
}

bool PatchPcmWrapper::getNuance() const
{
    char *data = patch->data(baseOffset+Nuance, 1);
    if(data == nullptr) return false;
    bool val = data[0];
    return val;
}

void PatchPcmWrapper::setNuance(bool val)
{
    if (getNuance() == val) return;
    patch->setData(baseOffset+Nuance, (quint8)(val), true);
    emit nuanceChanged(val);
}

int PatchPcmWrapper::getPan() const
{
    char *data = patch->data(baseOffset+Pan, 1);
    if(data == nullptr)
        return 0;
    return ((((int)data[0])+1)*100)/127-50;
}
void PatchPcmWrapper::setPan(int newVal)
{
    if(newVal == getPan())
        return;

    quint8 newval8 = ((newVal+50)*127)/100;
    patch->setData(baseOffset+Pan, newval8, true);
    emit panChanged(newVal);
}

int PatchPcmWrapper::getPitchShift() const
{
    char *data = patch->data(baseOffset+PitchShift, 1);
    if(data == nullptr)
        return 0;
    return ((int)data[0])-64;
}
void PatchPcmWrapper::setPitchShift(int newVal)
{
    if(newVal == getPitchShift())
        return;

    quint8 newval8 = newVal+64;
    patch->setData(baseOffset+PitchShift, newval8, true);
    emit pitchshiftChanged(newVal);
}

int PatchPcmWrapper::getPitchFine() const
{
    char *data = patch->data(baseOffset+PitchFine, 1);
    if(data == nullptr)
        return 0;
    return (int)data[0] - 64;
}
void PatchPcmWrapper::setPitchFine(int newVal)
{
    if(newVal == getPitchFine())
        return;

    quint8 newval8 = newVal+64;
    patch->setData(baseOffset+PitchFine, newval8, true);
    emit pitchfineChanged(newVal);
}

int PatchPcmWrapper::getPortamento() const
{
    char *data = patch->data(baseOffset+Portamento, 1);
    if(data == nullptr)
        return -1;
    return data[0];
}
void PatchPcmWrapper::setPortamento(int newVal)
{
    if(newVal == getPortamento())
        return;

    quint8 newval8 = newVal;
    patch->setData(baseOffset+Portamento, newval8, true);
    emit portamentoChanged(newVal);
}

int PatchPcmWrapper::getPortamentoTime() const
{
    char *data = patch->data(baseOffset+PortamentoTime, 1);
    if(data == nullptr)
        return -1;
    return ((((data[0] << 4) | data[1])+1)*100)/127;
}
void PatchPcmWrapper::setPortamentoTime(int newVal)
{
    if(newVal == getPortamentoTime())
        return;

    newVal = (newVal*127)/100;

    quint16 newval16 = ((newVal << 4) & 0x0F00) | (newVal & 0x0F);
    patch->setData(baseOffset+PortamentoTime, newval16, true);
    emit portamentotimeChanged(newVal);
}

void PatchPcmWrapper::onDataChanged(unsigned int offset, int lenght, bool byUserControl)
{
    if(byUserControl)
        return;

    if( offset < baseOffset || offset >= baseOffset+lenght)
        return;

    qDebug("Patch PCM Changed: baseoffset=%d, offset=%d, length=%d", baseOffset, offset, lenght);

    emit waveformChanged(getWaveform());
    emit toneswitchChanged(getToneSwitch());
    emit levelChanged(getLevel());
    emit octaveChanged(getOctave());
    emit chromaticChanged(getChromatic());
    emit legatoChanged(getLegato());
    emit nuanceChanged(getNuance());
    emit panChanged(getPan());
    emit pitchshiftChanged(getPitchShift());
    emit pitchfineChanged(getPitchFine());
    emit portamentoChanged(getPortamento());
    emit portamentotimeChanged(getPortamentoTime());
}

const QStringList PatchPcmWrapper::toneNameList =
{
      QStringLiteral("001  St.Piano 1"),
      QStringLiteral("002  St.Piano 2"),
      QStringLiteral("003  St.Piano 3"),
      QStringLiteral("004  St.Piano 4"),
      QStringLiteral("005  St.Piano 5"),
      QStringLiteral("006  Brite Piano"),
      QStringLiteral("007  Stage Piano"),
      QStringLiteral("008  Honky Tonk"),
      QStringLiteral("009  LoFi Piano"),
      QStringLiteral("010  Piano 1"),
      QStringLiteral("011  Piano 1 w"),
      QStringLiteral("012  European Pf"),
      QStringLiteral("013  Piano 2"),
      QStringLiteral("014  Piano 2 w"),
      QStringLiteral("015  Honky-tonk"),
      QStringLiteral("016  Honky-tonk w"),
      QStringLiteral("017  Pop Piano 1"),
      QStringLiteral("018  Pop Piano 2"),
      QStringLiteral("019  Pop Piano 3"),
      QStringLiteral("020  Piano 3"),
      QStringLiteral("021 Piano 3 w"),
      QStringLiteral("022 Stage EP 1"),
      QStringLiteral("023 Stage EP 2"),
      QStringLiteral("024 Stage EP Trm"),
      QStringLiteral("025 Tremolo EP 1"),
      QStringLiteral("026 E. Piano 1"),
      QStringLiteral("027 E. Piano 2"),
      QStringLiteral("028 E. Piano 3"),
      QStringLiteral("029 E. Piano 4"),
      QStringLiteral("030 E. Piano 5"),
      QStringLiteral("031 E. Piano 6"),
      QStringLiteral("032 Dyno EP"),
      QStringLiteral("033 Dyno EP Trm"),
      QStringLiteral("034 Tremolo EP 2"),
      QStringLiteral("035 Back2the60s"),
      QStringLiteral("036 Tine EP"),
      QStringLiteral("037 SA EP 1"),
      QStringLiteral("038 SA EP 2"),
      QStringLiteral("039 Psy EP"),
      QStringLiteral("040 Wurly EP"),
      QStringLiteral("041 Wurly EP Trm"),
      QStringLiteral("042 Curly Wurly"),
      QStringLiteral("043 Super Wurly"),
      QStringLiteral("044 E. Piano 7"),
      QStringLiteral("045 St. Soft EP"),
      QStringLiteral("046 Wurly"),
      QStringLiteral("047 EP Legend 1"),
      QStringLiteral("048 EP Belle"),
      QStringLiteral("049 80's EP"),
      QStringLiteral("050 FM EP 1"),
      QStringLiteral("051 FM EP 2"),
      QStringLiteral("052 Sinus EP"),
      QStringLiteral("053 Spirit Tines"),
      QStringLiteral("054 EP Legend 2"),
      QStringLiteral("055 E. Piano 8"),
      QStringLiteral("056 Detuned EP"),
      QStringLiteral("057 St. FM EP"),
      QStringLiteral("058 EP Legend 3"),
      QStringLiteral("059 EP Phase"),
      QStringLiteral("060 Rock Organ 1"),
      QStringLiteral("061 Rock Organ 2"),
      QStringLiteral("062 Rock Organ 3"),
      QStringLiteral("063 Rock Organ 4"),
      QStringLiteral("064 Rock Organ 5"),
      QStringLiteral("065 RotaryOrgan1"),
      QStringLiteral("066 RotaryOrgan2"),
      QStringLiteral("067 Perc.Organ 1"),
      QStringLiteral("068 Perc.Organ 2"),
      QStringLiteral("069 Perc.Organ 3"),
      QStringLiteral("070 E.Organ 1"),
      QStringLiteral("071 E.Organ 2"),
      QStringLiteral("072 E.Organ 3"),
      QStringLiteral("073 E.Organ 4"),
      QStringLiteral("074 E.Organ 5"),
      QStringLiteral("075 E.Organ 6"),
      QStringLiteral("076 E.Organ 7"),
      QStringLiteral("077 70's E.Org 1"),
      QStringLiteral("078 70's E.Org 2"),
      QStringLiteral("079 Ana Organ 1"),
      QStringLiteral("080 Ana Organ 2"),
      QStringLiteral("081 Ana Organ 3"),
      QStringLiteral("082 Ana Organ 4"),
      QStringLiteral("083 Ana Organ 5"),
      QStringLiteral("084 Organ 1"),
      QStringLiteral("085 Trem. Organ"),
      QStringLiteral("086 60's Organ"),
      QStringLiteral("087 70's E.Organ"),
      QStringLiteral("088 Organ 2"),
      QStringLiteral("089 Chorus Organ"),
      QStringLiteral("090 Perc.Organ 4"),
      QStringLiteral("091 Organ 3"),
      QStringLiteral("092 Grand Pipes"),
      QStringLiteral("093 Church Org 1"),
      QStringLiteral("094 Church Org 2"),
      QStringLiteral("095 Church Org 3"),
      QStringLiteral("096 Puff Organ"),
      QStringLiteral("097 Reed Organ"),
      QStringLiteral("098 Harpsichord1"),
      QStringLiteral("099 Harpsichord2"),
      QStringLiteral("100 Coupled Hps"),
      QStringLiteral("101 Harpsi w"),
      QStringLiteral("102 Harpsi o"),
      QStringLiteral("103 Clav 1"),
      QStringLiteral("104 Pulse Clav 1"),
      QStringLiteral("105 Sweepin Clav 1"),
      QStringLiteral("106 Analog Clav"),
      QStringLiteral("107 Biting Clav"),
      QStringLiteral("108 Pulse Clv St"),
      QStringLiteral("109 Clav 2"),
      QStringLiteral("110 Pulse Clav 2"),
      QStringLiteral("111 Celesta"),
      QStringLiteral("112 AccordionIt2"),
      QStringLiteral("113 Musette"),
      QStringLiteral("114 Vodkakordion"),
      QStringLiteral("115 Accordion Fr"),
      QStringLiteral("116 Accordion It"),
      QStringLiteral("117 Bandoneon"),
      QStringLiteral("118 Harmonica 1"),
      QStringLiteral("119 Harmonica 2"),
      QStringLiteral("120 FM Sparkles"),
      QStringLiteral("121 Fm Syn Bell"),
      QStringLiteral("122 FM Heaven"),
      QStringLiteral("123 Dreaming Bel"),
      QStringLiteral("124 Analog Bell"),
      QStringLiteral("125 Music Box 1"),
      QStringLiteral("126 Music Bells"),
      QStringLiteral("127 Kalimbells"),
      QStringLiteral("128 Bell 1"),
      QStringLiteral("129 Bell 2"),
      QStringLiteral("130 Icy Keys"),
      QStringLiteral("131 Carillon 1"),
      QStringLiteral("132 Tower Bell"),
      QStringLiteral("133 Tubularbell2"),
      QStringLiteral("134 Bell Ring"),
      QStringLiteral("135 Music Box 2"),
      QStringLiteral("136 TubularBells"),
      QStringLiteral("137 Church Bell"),
      QStringLiteral("138 Carillon 2"),
      QStringLiteral("139 Crystal"),
      QStringLiteral("140 Tinkle Bell"),
      QStringLiteral("141 Toy Box"),
      QStringLiteral("142 Vibraphone 1"),
      QStringLiteral("143 VibraphoneTr"),
      QStringLiteral("144 Tremolo Vib"),
      QStringLiteral("145 Marimba 1"),
      QStringLiteral("146 Marimba 2"),
      QStringLiteral("147 Steel Drums 1"),
      QStringLiteral("148 Xylophone 1"),
      QStringLiteral("149 Xylophone 2"),
      QStringLiteral("150 Ethno Keys"),
      QStringLiteral("151 Soft StlDrm"),
      QStringLiteral("152 Jazz Vib"),
      QStringLiteral("153 BsMarimba 1"),
      QStringLiteral("154 BsMarimba 2"),
      QStringLiteral("155 Sine Mallet"),
      QStringLiteral("156 Glockenspeil"),
      QStringLiteral("157 Vibraphone 2"),
      QStringLiteral("158 Vibraphone w"),
      QStringLiteral("159 Marimba 2"),
      QStringLiteral("160 Marimba w"),
      QStringLiteral("161 Xylophone 3"),
      QStringLiteral("162 SteelDrums 2"),
      QStringLiteral("163 Nylon Gtr 1"),
      QStringLiteral("164 Nylon Gtr 2"),
      QStringLiteral("165 Nylon Gtr 3"),
      QStringLiteral("166 Nylon Gtr 4"),
      QStringLiteral("167 Wet Nyln Gtr"),
      QStringLiteral("168 Ukulele 1"),
      QStringLiteral("169 Folk Gtr 1"),
      QStringLiteral("170 Folk Gtr 2"),
      QStringLiteral("171 Folk Gtr 3"),
      QStringLiteral("172 Latin Gtr"),
      QStringLiteral("173 Nylon Gtr 5"),
      QStringLiteral("174 Ukulele 2"),
      QStringLiteral("175 Nylon Gtr 5o"),
      QStringLiteral("176 Nylon Gtr 6"),
      QStringLiteral("177 Steel-str.Gt"),
      QStringLiteral("178 12-str. Gtr"),
      QStringLiteral("179 Steel + Body"),
      QStringLiteral("180 Gt FretNoise"),
      QStringLiteral("181 Clean Gtr 1"),
      QStringLiteral("182 Clean Gtr 2"),
      QStringLiteral("183 Clean Gtr 3"),
      QStringLiteral("184 JazzGuitar 1"),
      QStringLiteral("185 Pick E.Gtr"),
      QStringLiteral("186 FunkGuitar 1"),
      QStringLiteral("187 Wet E.Gtr"),
      QStringLiteral("188 PedalSteel 1 "),
      QStringLiteral("189 JazzGuitar 2"),
      QStringLiteral("190 PedalSteel 2"),
      QStringLiteral("191 Clean Guitar"),
      QStringLiteral("192 Chorus Gtr"),
      QStringLiteral("193 Mid Tone Gtr"),
      QStringLiteral("194 Muted Guitar"),
      QStringLiteral("195 Funk Pop"),
      QStringLiteral("196 FunkGuitar 2"),
      QStringLiteral("197 Jazz Man"),
      QStringLiteral("198 Gt Cut Noise"),
      QStringLiteral("199 OverdriveGt1"),
      QStringLiteral("200 Dist Gtr 1"),
      QStringLiteral("201 Dist Gtr 2"),
      QStringLiteral("202 Dist Gtr 3"),
      QStringLiteral("203 OverdriveGtr2"),
      QStringLiteral("204 Guitar Pinch"),
      QStringLiteral("205 DistortionGt"),
      QStringLiteral("206 Gt Feedback1"),
      QStringLiteral("207 Dist Rtm Gtr"),
      QStringLiteral("208 Gt Harmonics"),
      QStringLiteral("209 Gt Feedback2"),
      QStringLiteral("210 AcousticBs 1"),
      QStringLiteral("211 AcousticBs 2"),
      QStringLiteral("212 AcousticBs 3"),
      QStringLiteral("213 String Slap"),
      QStringLiteral("214 FingeredBs 1"),
      QStringLiteral("215 FingeredBs 2"),
      QStringLiteral("216 FingeredBs 3"),
      QStringLiteral("217 Pick Bass"),
      QStringLiteral("218 FretlessBs 1"),
      QStringLiteral("219 FretlessBs 2"),
      QStringLiteral("220 Finger Slap2"),
      QStringLiteral("221 Return2Base!"),
      QStringLiteral("222 FingeredBs 4"),
      QStringLiteral("223 Finger Slap"),
      QStringLiteral("224 Picked Bass"),
      QStringLiteral("225 FretlessBs 3"),
      QStringLiteral("226 Slap Bass 1"),
      QStringLiteral("227 Slap Bass 2"),
      QStringLiteral("228 MG Bass 1"),
      QStringLiteral("229 MG Bass 2"),
      QStringLiteral("230 MG Bass 3"),
      QStringLiteral("231 Modular Bs 1"),
      QStringLiteral("232 Modular Bs 2"),
      QStringLiteral("233 PWM Bass 1"),
      QStringLiteral("234 PWM Bass 2"),
      QStringLiteral("235 Big Mini"),
      QStringLiteral("236 Fat Analog"),
      QStringLiteral("237 Spike Bass"),
      QStringLiteral("238 SH Bass"),
      QStringLiteral("239 Intrusive BS"),
      QStringLiteral("240 Synth Bass 1"),
      QStringLiteral("241 Synth Bass 2"),
      QStringLiteral("242 Synth Bass 3"),
      QStringLiteral("243 Synth Bass 4"),
      QStringLiteral("244 Synth Bass 5"),
      QStringLiteral("245 Synth Bass 6"),
      QStringLiteral("246 Synth Bass 7"),
      QStringLiteral("247 Synth Bass 8"),
      QStringLiteral("248 Synth Bass 9"),
      QStringLiteral("249 Synth Bass10"),
      QStringLiteral("250 Synth Bass11"),
      QStringLiteral("251 Synth Bass12"),
      QStringLiteral("252 Reso Bass 1"),
      QStringLiteral("253 Reso Bass 2"),
      QStringLiteral("254 Reso Bass 3"),
      QStringLiteral("255 Reso Bass 4"),
      QStringLiteral("256 Reso Bass 5"),
      QStringLiteral("257 Reso Bass 6"),
      QStringLiteral("258 Reso Bass 7"),
      QStringLiteral("259 Reso Bass 8"),
      QStringLiteral("260 Acid Bass 1"),
      QStringLiteral("261 Reso Bass 2"),
      QStringLiteral("262 Reso Bass 3"),
      QStringLiteral("263 Alpha Bass 1"),
      QStringLiteral("264 TB Bass 1"),
      QStringLiteral("265 TB Bass 2"),
      QStringLiteral("266 Alpha Bass 2"),
      QStringLiteral("267 Alpha ResoBs"),
      QStringLiteral("268 Nu Saw Bass"),
      QStringLiteral("269 Nu RnB SawBs"),
      QStringLiteral("270 Storm Bass"),
      QStringLiteral("271 Detuned Bass"),
      QStringLiteral("272 Gashed Bass"),
      QStringLiteral("273 Hi-Energy Bs"),
      QStringLiteral("274 Pedal Bass 1"),
      QStringLiteral("275 Monster Bass"),
      QStringLiteral("276 JunoSqr Bs 1"),
      QStringLiteral("277 JunoSqr Bs 2"),
      QStringLiteral("278 101 Bass"),
      QStringLiteral("279 106 Bass 1"),
      QStringLiteral("280 106 Bass 2"),
      QStringLiteral("281 Compu Bass 1"),
      QStringLiteral("282 Compu Bass 2"),
      QStringLiteral("283 Triangle Bs"),
      QStringLiteral("284 Muffled Bass"),
      QStringLiteral("285 Garage Bass"),
      QStringLiteral("286 TransistorBs"),
      QStringLiteral("287 Fazee Bass"),
      QStringLiteral("288 TB Bass 3"),
      QStringLiteral("289 TB Bass 4"),
      QStringLiteral("290 Acid Bass 4"),
      QStringLiteral("291 Acid Bass 5"),
      QStringLiteral("292 Acid Bass 6"),
      QStringLiteral("293 Brite Bass"),
      QStringLiteral("294 Pedal Bass 2"),
      QStringLiteral("295 Saw Bass"),
      QStringLiteral("296 Reso Bass 9"),
      QStringLiteral("297 Reso Bass 10"),
      QStringLiteral("298 Sub Bass"),
      QStringLiteral("299 Ramp Bass"),
      QStringLiteral("300 Fat Bass 1"),
      QStringLiteral("301 Fat Bass 2"),
      QStringLiteral("302 Fat Bass 3"),
      QStringLiteral("303 Flat Bass"),
      QStringLiteral("304 Electro Rubb"),
      QStringLiteral("305 80s Bass"),
      QStringLiteral("306 Synth Bass13"),
      QStringLiteral("307 SynthBass101"),
      QStringLiteral("308 Acid Bass 7"),
      QStringLiteral("309 Clav Bass"),
      QStringLiteral("310 Hammer Bass"),
      QStringLiteral("311 Synth Bass14"),
      QStringLiteral("312 SynSlap Bass"),
      QStringLiteral("313 Rubber Bass"),
      QStringLiteral("314 Attack Pulse"),
      QStringLiteral("315 Santur 1"),
      QStringLiteral("316 Santur 2"),
      QStringLiteral("317 Aerial Harp"),
      QStringLiteral("318 LostParadise"),
      QStringLiteral("319 Sitar 1"),
      QStringLiteral("320 Indian Frtls"),
      QStringLiteral("321 Sitar Pad"),
      QStringLiteral("322 Santur 3"),
      QStringLiteral("323 Mandolin"),
      QStringLiteral("324 Harp"),
      QStringLiteral("325 Yang Qin"),
      QStringLiteral("326 Sitar 2"),
      QStringLiteral("327 Sitar 3"),
      QStringLiteral("328 Banjo"),
      QStringLiteral("329 Shamisen"),
      QStringLiteral("330 Koto"),
      QStringLiteral("331 Taisho Koto"),
      QStringLiteral("332 Kalimba"),
      QStringLiteral("333 Violin 1"),
      QStringLiteral("334 Bright Vln"),
      QStringLiteral("335 Bright VC"),
      QStringLiteral("336 Violin 2"),
      QStringLiteral("337 Slow Violin"),
      QStringLiteral("338 Viola"),
      QStringLiteral("339 Cello"),
      QStringLiteral("340 Contrabass"),
      QStringLiteral("341 Fiddle"),
      QStringLiteral("342 Mood Strings"),
      QStringLiteral("343 Strings 1"),
      QStringLiteral("344 Strings 2"),
      QStringLiteral("345 Strings 3"),
      QStringLiteral("346 Strings 4"),
      QStringLiteral("347 Stage Str 1"),
      QStringLiteral("348 Stage Str 2"),
      QStringLiteral("349 Pop Str"),
      QStringLiteral("350 Marc. Str"),
      QStringLiteral("351 StringsStacc"),
      QStringLiteral("352 Pizz 1"),
      QStringLiteral("353 Pizz 2"),
      QStringLiteral("354 TapeStrings1"),
      QStringLiteral("355 TapeStrings2"),
      QStringLiteral("356 Hybrid Str"),
      QStringLiteral("357 Strings 5"),
      QStringLiteral("358 Hall Strings"),
      QStringLiteral("359 Tremolo Str"),
      QStringLiteral("360 PizzicatoStr"),
      QStringLiteral("361 Strings 6"),
      QStringLiteral("362 Oct Strings"),
      QStringLiteral("363 Slow Strings"),
      QStringLiteral("364 Orc. Unison 1"),
      QStringLiteral("365 Orc. Unison 2"),
      QStringLiteral("366 Full Orc"),
      QStringLiteral("367 Orchestra"),
      QStringLiteral("368 Trumpet 1"),
      QStringLiteral("369 Fr. Horn"),
      QStringLiteral("370 Trumpet 2"),
      QStringLiteral("371 Dark Trumpet"),
      QStringLiteral("372 Trombone 1"),
      QStringLiteral("373 Trombone 2"),
      QStringLiteral("374 Bright Tb"),
      QStringLiteral("375 Tuba"),
      QStringLiteral("376 MuteTrumpet1"),
      QStringLiteral("377 MuteTrumpet2"),
      QStringLiteral("378 French Horn"),
      QStringLiteral("379 Brass 1"),
      QStringLiteral("380 Brass 2"),
      QStringLiteral("381 Brass 3"),
      QStringLiteral("382 Brass 4"),
      QStringLiteral("383 F. Horn Sect"),
      QStringLiteral("384 Brass 5"),
      QStringLiteral("385 Brass 6"),
      QStringLiteral("386 Bassoon 1"),
      QStringLiteral("387 Oboe"),
      QStringLiteral("388 English Horn"),
      QStringLiteral("389 Bassoon 2"),
      QStringLiteral("390 Clarinet"),
      QStringLiteral("391 Bagpipe"),
      QStringLiteral("392 Shanai"),
      QStringLiteral("393 Flute 1"),
      QStringLiteral("394 Pan Flute 1"),
      QStringLiteral("395 Pan Pipes 1"),
      QStringLiteral("396 Shakuhachi 1"),
      QStringLiteral("397 Pan Pipes 2"),
      QStringLiteral("398 Piccolo"),
      QStringLiteral("399 Flute 2"),
      QStringLiteral("400 Pan Flute 2"),
      QStringLiteral("401 Bottle Blow"),
      QStringLiteral("402 Shakuhachi 2"),
      QStringLiteral("403 Breath Noise"),
      QStringLiteral("404 F1.Key Click"),
      QStringLiteral("405 Soprano Sax1"),
      QStringLiteral("406 BreathyTenor"),
      QStringLiteral("407 Tenor Sax 1"),
      QStringLiteral("408 Soprano Sax2"),
      QStringLiteral("409 **** Sax"),
      QStringLiteral("410 Tenor Sax 2"),
      QStringLiteral("411 Baritone Sax"),
      QStringLiteral("412 Ocarina 1"),
      QStringLiteral("413 Recorder"),
      QStringLiteral("414 Whistle"),
      QStringLiteral("415 Ocarina 2"),
      QStringLiteral("416 Dreamvox 1"),
      QStringLiteral("417 Dreamvox 2"),
      QStringLiteral("418 Choir Pad"),
      QStringLiteral("419 Angels Choir"),
      QStringLiteral("420 Aerial Choir"),
      QStringLiteral("421 Doo Pad"),
      QStringLiteral("422 Humming 1"),
      QStringLiteral("423 Humming 2"),
      QStringLiteral("424 Gospel Hum"),
      QStringLiteral("425 Vox Pad 1"),
      QStringLiteral("426 Vox Pad 2"),
      QStringLiteral("427 80s Vox"),
      QStringLiteral("428 SynVox 1"),
      QStringLiteral("429 SynVox 2"),
      QStringLiteral("430 Mini Vox"),
      QStringLiteral("431 Chipmunk"),
      QStringLiteral("432 Sample Opera"),
      QStringLiteral("433 Sad Ceremony"),
      QStringLiteral("434 5th Voice"),
      QStringLiteral("435 Sop Vox"),
      QStringLiteral("436 Choir Aahs"),
      QStringLiteral("437 Chorus Aahs"),
      QStringLiteral("438 Voice Oohs"),
      QStringLiteral("439 Humming 3"),
      QStringLiteral("440 SynVox 3"),
      QStringLiteral("441 Analog Voice"),
      QStringLiteral("442 Space Voice"),
      QStringLiteral("443 Itopia"),
      QStringLiteral("444 Jazz Scat 1"),
      QStringLiteral("445 Jazz Scat 2"),
      QStringLiteral("446 Saw Lead 1"),
      QStringLiteral("447 Saw Lead 2"),
      QStringLiteral("448 Saw Lead 3"),
      QStringLiteral("449 Saw Lead 4"),
      QStringLiteral("450 Saw Lead 5"),
      QStringLiteral("451 Saw Lead 6"),
      QStringLiteral("452 Saw Lead 7"),
      QStringLiteral("453 Saw Lead 8"),
      QStringLiteral("454 Saw Lead 9"),
      QStringLiteral("455 GR300 Lead 1"),
      QStringLiteral("456 GR300 Lead 2"),
      QStringLiteral("457 Classic GR"),
      QStringLiteral("458 Bright GR"),
      QStringLiteral("459 Fat GR Lead"),
      QStringLiteral("460 MODified Ld"),
      QStringLiteral("461 Syn Lead 1"),
      QStringLiteral("462 Syn Lead 2"),
      QStringLiteral("463 Syn Lead 3"),
      QStringLiteral("464 Syn Lead 4"),
      QStringLiteral("465 Syn Lead 5"),
      QStringLiteral("466 Syn Lead 6"),
      QStringLiteral("467 Syn Lead 7"),
      QStringLiteral("468 Pro Fat Ld 1"),
      QStringLiteral("469 JupiterLead1"),
      QStringLiteral("470 JupiterLead2"),
      QStringLiteral("471 Porta Lead"),
      QStringLiteral("472 Classic Lead"),
      QStringLiteral("473 On Air"),
      QStringLiteral("474 Pro Fat Ld 2"),
      QStringLiteral("475 Wormy Lead"),
      QStringLiteral("476 Waspy Lead"),
      QStringLiteral("477 Brite ResoLd"),
      QStringLiteral("478 Brass Lead"),
      QStringLiteral("479 Legato Tkno"),
      QStringLiteral("480 Follow Me"),
      QStringLiteral("481 Octa Juice"),
      QStringLiteral("482 Juicy Jupe"),
      QStringLiteral("483 Octa Saw"),
      QStringLiteral("484 Vintager 1"),
      QStringLiteral("485 Vintager 2"),
      QStringLiteral("486 Sync Lead"),
      QStringLiteral("487 Octa Sync"),
      QStringLiteral("488 Leading Sync"),
      QStringLiteral("489 A Leader"),
      QStringLiteral("490 Hot Coffee"),
      QStringLiteral("491 Hot Sync"),
      QStringLiteral("492 Syncro Lead"),
      QStringLiteral("493 Space Solo"),
      QStringLiteral("494 Squareheads"),
      QStringLiteral("495 Mod Lead"),
      QStringLiteral("496 Alpha Spit"),
      QStringLiteral("497 Air Lead"),
      QStringLiteral("498 Pulstar Lead"),
      QStringLiteral("499 Therasaw"),
      QStringLiteral("500 Warmy Lead"),
      QStringLiteral("501 ResoSawLead"),
      QStringLiteral("502 Soft Reso Ld"),
      QStringLiteral("503 Reso Lead 1"),
      QStringLiteral("504 Reso Lead 2"),
      QStringLiteral("505 Reso Lead 3"),
      QStringLiteral("506 Reso Lead 4"),
      QStringLiteral("507 Reso Lead 5"),
      QStringLiteral("508 Juicy Lead"),
      QStringLiteral("509 DC Triangle"),
      QStringLiteral("510 Soft Lead 1"),
      QStringLiteral("511 Soft Lead 2"),
      QStringLiteral("512 Soft Lead 3"),
      QStringLiteral("513 Soft Lead 4"),
      QStringLiteral("514 Soft Lead 5"),
      QStringLiteral("515 Soft Lead 6"),
      QStringLiteral("516 Soft Lead 7"),
      QStringLiteral("517 Soft Lead 8"),
      QStringLiteral("518 Soft Lead 9"),
      QStringLiteral("519 Soft Lead 10"),
      QStringLiteral("520 Tri Lead"),
      QStringLiteral("521 Pulse Lead 1"),
      QStringLiteral("522 Pulse Lead 2"),
      QStringLiteral("523 Simple Tri"),
      QStringLiteral("524 Simple Sine"),
      QStringLiteral("525 Whistle Ld 1"),
      QStringLiteral("526 Whistle Ld 2"),
      QStringLiteral("527 Square Pipe"),
      QStringLiteral("528 CosmicDrops1"),
      QStringLiteral("529 Spooky Lead"),
      QStringLiteral("530 Pure Lead"),
      QStringLiteral("531 303 NRG"),
      QStringLiteral("532 Round SQR"),
      QStringLiteral("533 Brite SQR"),
      QStringLiteral("534 Square SAW"),
      QStringLiteral("535 Simple SQR"),
      QStringLiteral("536 Sqr Lead"),
      QStringLiteral("537 Atk Lead"),
      QStringLiteral("538 Octa Square"),
      QStringLiteral("539 CS Lead"),
      QStringLiteral("540 Mini Growl"),
      QStringLiteral("541 Hoover Again"),
      QStringLiteral("542 Tranceformer"),
      QStringLiteral("543 Juno SQR"),
      QStringLiteral("544 Pulse Lead 3"),
      QStringLiteral("545 Pulse Lead 4"),
      QStringLiteral("546 Ramp Lead 1"),
      QStringLiteral("547 Ramp Lead 2"),
      QStringLiteral("548 Saw Lead 10"),
      QStringLiteral("549 Sine Lead 1"),
      QStringLiteral("550 Sine Lead 2"),
      QStringLiteral("551 Dance Saws1"),
      QStringLiteral("552 Resoform"),
      QStringLiteral("553 Dance Saws2"),
      QStringLiteral("554 Square Wave"),
      QStringLiteral("555 MG Square"),
      QStringLiteral("556 2600 Sine"),
      QStringLiteral("557 Saw Wave"),
      QStringLiteral("558 OB2 Saw"),
      QStringLiteral("559 Doctor Solo"),
      QStringLiteral("560 Natural Lead"),
      QStringLiteral("561 Syn. Calliope"),
      QStringLiteral("562 Chiffer Lead"),
      QStringLiteral("563 Charang"),
      QStringLiteral("564 Wire Lead"),
      QStringLiteral("565 Solo Vox"),
      QStringLiteral("566 5th Saw Wave"),
      QStringLiteral("567 Bass & Lead"),
      QStringLiteral("568 Delayed Lead"),
      QStringLiteral("569 80s Brass 1"),
      QStringLiteral("570 80s Brass 2"),
      QStringLiteral("571 80s Brass 3"),
      QStringLiteral("572 80s Brass 4"),
      QStringLiteral("573 80s Brass 5"),
      QStringLiteral("574 80s Brass 6"),
      QStringLiteral("575 80s Brass 7"),
      QStringLiteral("576 80s Brass 8"),
      QStringLiteral("577 Soft SynBrs1"),
      QStringLiteral("578 Warm SynBrs"),
      QStringLiteral("579 Brite SynBrs"),
      QStringLiteral("580 Express Brs"),
      QStringLiteral("581 EuroExpress1"),
      QStringLiteral("582 JP Brass 1"),
      QStringLiteral("583 Juno Brass"),
      QStringLiteral("584 Ox Brass"),
      QStringLiteral("585 Reso Brass"),
      QStringLiteral("586 Wide SynBrs"),
      QStringLiteral("587 Soft SynBrs2"),
      QStringLiteral("588 JP Brass 2"),
      QStringLiteral("589 106 Brass"),
      QStringLiteral("590 Octa Brass"),
      QStringLiteral("591 Poly Brass 1"),
      QStringLiteral("592 Dual Saw Brs"),
      QStringLiteral("593 Jump Poly"),
      QStringLiteral("594 Reso Key 1"),
      QStringLiteral("595 EuroExpress2"),
      QStringLiteral("596 Ox Synth"),
      QStringLiteral("597 VintageBrs 1"),
      QStringLiteral("598 VintageBrs 2"),
      QStringLiteral("599 VintageBrs 3"),
      QStringLiteral("600 VintageBrs 4"),
      QStringLiteral("601 Poly Brass 2"),
      QStringLiteral("602 Synth Brass1"),
      QStringLiteral("603 JP Brass"),
      QStringLiteral("604 Oct SynBrass"),
      QStringLiteral("605 Jump Brass"),
      QStringLiteral("606 Synth Brass2"),
      QStringLiteral("607 SynBrass sfz"),
      QStringLiteral("608 Velo Brass"),
      QStringLiteral("609 Heaven Pad 1"),
      QStringLiteral("610 Heaven Pad 2"),
      QStringLiteral("611 Dreamheaven"),
      QStringLiteral("612 Oct Heaven"),
      QStringLiteral("613 Soft Pad 1"),
      QStringLiteral("614 Soft Pad 2"),
      QStringLiteral("615 Soft Pad 3"),
      QStringLiteral("616 Soft Pad 4"),
      QStringLiteral("617 Soft Pad 5"),
      QStringLiteral("618 Soft Pad 6"),
      QStringLiteral("619 Soft Pad 7"),
      QStringLiteral("620 Soft Pad 8"),
      QStringLiteral("621 Soft Pad 9"),
      QStringLiteral("622 Soft Pad 10"),
      QStringLiteral("623 VintageStr 1"),
      QStringLiteral("624 VintageStr 2"),
      QStringLiteral("625 VintageStr 3"),
      QStringLiteral("626 VintageStr 4"),
      QStringLiteral("627 VintageStr 5"),
      QStringLiteral("628 VintageStr 6"),
      QStringLiteral("629 VintageStr 7"),
      QStringLiteral("630 JX Strings"),
      QStringLiteral("631 JP Strings 1"),
      QStringLiteral("632 JP Strings 2"),
      QStringLiteral("633 106 Strings"),
      QStringLiteral("634 PWM Str 1"),
      QStringLiteral("635 PWM Str 2"),
      QStringLiteral("636 Fading Str"),
      QStringLiteral("637 ParadisePad"),
      QStringLiteral("638 80s Strings"),
      QStringLiteral("639 Stringship"),
      QStringLiteral("640 Airy Pad"),
      QStringLiteral("641 Neo RS-202"),
      QStringLiteral("642 Sawtooth Str"),
      QStringLiteral("643 Pulse Pad"),
      QStringLiteral("644 Hollow Pad 1"),
      QStringLiteral("645 WarmHeaven 1"),
      QStringLiteral("646 WarmHeaven 2 "),
      QStringLiteral("647 Heaven Pad 3"),
      QStringLiteral("648 Heaven Pad 4 "),
      QStringLiteral("649 FineWinePad1"),
      QStringLiteral("650 FineWinePad2"),
      QStringLiteral("651 5th Pad 1"),
      QStringLiteral("652 5th Pad 2"),
      QStringLiteral("653 Nu Epic Pad"),
      QStringLiteral("654 Angelis Pad"),
      QStringLiteral("655 TrnsSweepPad"),
      QStringLiteral("656 Giant Sweep"),
      QStringLiteral("657 Voyager"),
      QStringLiteral("658 Digital Pad"),
      QStringLiteral("659 NuSoundtrack"),
      QStringLiteral("660 Xadecimal"),
      QStringLiteral("661 Strobe Pad"),
      QStringLiteral("662 BUBBLE 2"),
      QStringLiteral("663 BUBBLE 3"),
      QStringLiteral("664 Soft PWM Pad"),
      QStringLiteral("665 Org Pad"),
      QStringLiteral("666 Hollow Pad 2"),
      QStringLiteral("667 SavannaPad 1"),
      QStringLiteral("668 SavannaPad 2"),
      QStringLiteral("669 SavannaPad 3"),
      QStringLiteral("670 PWM Str 3"),
      QStringLiteral("671 PWM Pad 2"),
      QStringLiteral("672 Str Machine"),
      QStringLiteral("673 Reso Pad"),
      QStringLiteral("674 BPF Pad"),
      QStringLiteral("675 Sweep Pad 2"),
      QStringLiteral("676 Sweep Pad 3 "),
      QStringLiteral("677 Sweep Pad 4"),
      QStringLiteral("678 Scoop Pad 1"),
      QStringLiteral("679 Scoop Pad 2"),
      QStringLiteral("680 Brite Wine"),
      QStringLiteral("681 Wine Pad"),
      QStringLiteral("682 Syn. Strings1"),
      QStringLiteral("683 Syn. Strings3"),
      QStringLiteral("684 Syn. Strings2"),
      QStringLiteral("685 Warm Pad"),
      QStringLiteral("686 Sine Pad"),
      QStringLiteral("687 Bowed Glass"),
      QStringLiteral("688 Metal Pad"),
      QStringLiteral("689 Halo Pad"),
      QStringLiteral("690 Sweep Pad"),
      QStringLiteral("691 Soundtrack"),
      QStringLiteral("692 Star Theme"),
      QStringLiteral("693 D50 Fantasy"),
      QStringLiteral("694 D50 Bell"),
      QStringLiteral("695 Dreambell"),
      QStringLiteral("696 Dreaming Box"),
      QStringLiteral("697 Air Key 1"),
      QStringLiteral("698 Sweet Keys"),
      QStringLiteral("699 Soft Bell"),
      QStringLiteral("700 Air Key 2"),
      QStringLiteral("701 Stacc Heaven"),
      QStringLiteral("702 DigitalDream"),
      QStringLiteral("703 Analog Dream"),
      QStringLiteral("704 Harp Pad"),
      QStringLiteral("705 Org Bell"),
      QStringLiteral("706 Fantasia"),
      QStringLiteral("707 Ice Rain"),
      QStringLiteral("708 Atmosphere"),
      QStringLiteral("709 Brightness"),
      QStringLiteral("710 Dream Trance"),
      QStringLiteral("711 Dream Saws"),
      QStringLiteral("712 Dream Pulse"),
      QStringLiteral("713 Trance Synth"),
      QStringLiteral("714 Trancy"),
      QStringLiteral("715 Trance Keys"),
      QStringLiteral("716 Trance Saws"),
      QStringLiteral("717 Auto Trance1"),
      QStringLiteral("718 Super Saws 1"),
      QStringLiteral("719 Analog Saws"),
      QStringLiteral("720 Uni-G"),
      QStringLiteral("721 Digitaless"),
      QStringLiteral("722 Bustranza"),
      QStringLiteral("723 Super Saws 2"),
      QStringLiteral("724 Poly Synth 1"),
      QStringLiteral("725 Poly Synth 2"),
      QStringLiteral("726 Poly Synth 3"),
      QStringLiteral("727 Poly Synth 4"),
      QStringLiteral("728 Poly Synth 5"),
      QStringLiteral("729 Poly Synth 6"),
      QStringLiteral("730 Juno Saw Key"),
      QStringLiteral("731 Saw Key 1"),
      QStringLiteral("732 Saw Key 2"),
      QStringLiteral("733 Waspy Synth"),
      QStringLiteral("734 Vintage Key"),
      QStringLiteral("735 Ju-D Fifths"),
      QStringLiteral("736 Reso Key 2"),
      QStringLiteral("737 Fat Synth"),
      QStringLiteral("738 DOC Stack"),
      QStringLiteral("739 2 Saws"),
      QStringLiteral("740 Hi Saw Band"),
      QStringLiteral("741 Brite Synth"),
      QStringLiteral("742 PWM Pad 1"),
      QStringLiteral("743 RAVtune"),
      QStringLiteral("744 Heaven Key"),
      QStringLiteral("745 Pipe Key"),
      QStringLiteral("746 Shroomy"),
      QStringLiteral("747 AnalogDays 1"),
      QStringLiteral("748 Sync Key"),
      QStringLiteral("749 Detune Ramp"),
      QStringLiteral("750 Reso Saw"),
      QStringLiteral("751 EuroExpress3"),
      QStringLiteral("752 Sweep Saw"),
      QStringLiteral("753 Poly Synth 7"),
      QStringLiteral("754 Syn Mallet"),
      QStringLiteral("755 Enigmatic"),
      QStringLiteral("756 Planetz"),
      QStringLiteral("757 Shimmer Pad"),
      QStringLiteral("758 Sci-Fi"),
      QStringLiteral("759 ResoSweep Dn"),
      QStringLiteral("760 Jet Noise"),
      QStringLiteral("761 Brandish"),
      QStringLiteral("762 909 Fx"),
      QStringLiteral("763 Zap"),
      QStringLiteral("764 Polysweep Nz"),
      QStringLiteral("765 Passing By"),
      QStringLiteral("766 Lazer Points"),
      QStringLiteral("767 CosmicDrops2"),
      QStringLiteral("768 Crystal Fx"),
      QStringLiteral("769 Crystal Ice"),
      QStringLiteral("770 Mad Noise"),
      QStringLiteral("771 Robot Sci-Fi"),
      QStringLiteral("772 Computer 1"),
      QStringLiteral("773 Computer 2"),
      QStringLiteral("774 S&H Noise"),
      QStringLiteral("775 S&H Ramp"),
      QStringLiteral("776 S&H PWM"),
      QStringLiteral("777 S&H Saw 1"),
      QStringLiteral("778 S&H Saw 2"),
      QStringLiteral("779 Electrostar"),
      QStringLiteral("780 Alpha Said"),
      QStringLiteral("781 FX Ramp"),
      QStringLiteral("782 Bubble 1"),
      QStringLiteral("783 Goblin"),
      QStringLiteral("784 Echo Drops"),
      QStringLiteral("785 Echo Bell"),
      QStringLiteral("786 Analog Seq"),
      QStringLiteral("787 Seq Pop"),
      QStringLiteral("788 Periscope"),
      QStringLiteral("789 Major 7"),
      QStringLiteral("790 Juno-D Maj7"),
      QStringLiteral("791 Sweet House"),
      QStringLiteral("792 Detune Saws"),
      QStringLiteral("793 Melodic Drum"),
      QStringLiteral("794 Detune Seq"),
      QStringLiteral("795 SequencedSaw"),
      QStringLiteral("796 Echo Pan"),
      QStringLiteral("797 PanninFormnt"),
      QStringLiteral("798 Fairy's Song"),
      QStringLiteral("799 Atmospherics"),
      QStringLiteral("800 StrobeBell 1"),
      QStringLiteral("801 StrobeBell 2"),
      QStringLiteral("802 Flying Pad 1"),
      QStringLiteral("803 Flying Pad 2"),
      QStringLiteral("804 Flying Pad 3"),
      QStringLiteral("805 Flying Pad 4"),
      QStringLiteral("806 Flying Pad 5"),
      QStringLiteral("807 Sine Magic"),
      QStringLiteral("808 Pulsatron"),
      QStringLiteral("809 Motion Bass "),
      QStringLiteral("810 Trance Splt"),
      QStringLiteral("811 Rythmic 5th"),
      QStringLiteral("812 Rythmic 1"),
      QStringLiteral("813 Rythmic 2"),
      QStringLiteral("814 Mega Sync 1"),
      QStringLiteral("815 StrobeBell 3"),
      QStringLiteral("816 Strobe 1"),
      QStringLiteral("817 Strobe 2"),
      QStringLiteral("818 Strobe 3"),
      QStringLiteral("819 Strobe 4"),
      QStringLiteral("820 LFO Saw"),
      QStringLiteral("821 Keep Going"),
      QStringLiteral("822 Keep Running"),
      QStringLiteral("823 Electrons"),
      QStringLiteral("824 BriskVortex"),
      QStringLiteral("825 LFO Vox"),
      QStringLiteral("826 Pulsasaw"),
      QStringLiteral("827 Arposphere"),
      QStringLiteral("828 Mega Sync 2"),
      QStringLiteral("829 Compusonic 1"),
      QStringLiteral("830 Compusonic 2"),
      QStringLiteral("831 Compusonic 3"),
      QStringLiteral("832 Compusonic 4"),
      QStringLiteral("833 Compusonic 5"),
      QStringLiteral("834 AnalogDays 2"),
      QStringLiteral("835 Groove 007"),
      QStringLiteral("836 Juno Pop"),
      QStringLiteral("837 Auto Trance2"),
      QStringLiteral("838 In Da Groove"),
      QStringLiteral("839 80s Beat"),
      QStringLiteral("840 Cheezy Movie"),
      QStringLiteral("841 Mod Chord"),
      QStringLiteral("842 Housechord"),
      QStringLiteral("843 OrchestraHit"),
      QStringLiteral("844 Bass Hit"),
      QStringLiteral("845 6th Hit"),
      QStringLiteral("846 Euro Hit"),
      QStringLiteral("847 Scratch 1"),
      QStringLiteral("848 Seashore"),
      QStringLiteral("849 Rain"),
      QStringLiteral("850 Thunder"),
      QStringLiteral("851 Wind"),
      QStringLiteral("852 Stream"),
      QStringLiteral("853 Bubble"),
      QStringLiteral("854 Bird 1"),
      QStringLiteral("855 Dog"),
      QStringLiteral("856 Horse Gallop"),
      QStringLiteral("857 Bird 2"),
      QStringLiteral("858 Telephone 1"),
      QStringLiteral("859 Telephone 2"),
      QStringLiteral("860 DoorCreaking"),
      QStringLiteral("861 Door"),
      QStringLiteral("862 Scratch 2"),
      QStringLiteral("863 Wind Chimes"),
      QStringLiteral("864 Helicopter"),
      QStringLiteral("865 Car Engine"),
      QStringLiteral("866 Car Stop"),
      QStringLiteral("867 Car Pass"),
      QStringLiteral("868 Car Crash"),
      QStringLiteral("869 Siren"),
      QStringLiteral("870 Train"),
      QStringLiteral("871 Jetplane"),
      QStringLiteral("872 Starship"),
      QStringLiteral("873 Burst Noise"),
      QStringLiteral("874 Applause"),
      QStringLiteral("875 Laughing"),
      QStringLiteral("876 Screaming"),
      QStringLiteral("877 Punch"),
      QStringLiteral("878 Heart Beat"),
      QStringLiteral("879 Footsteps"),
      QStringLiteral("880 Gun Shot"),
      QStringLiteral("881 Machine Gun"),
      QStringLiteral("882 Laser Gun"),
      QStringLiteral("883 Explosion"),
      QStringLiteral("884 Ride Cymbal"),
      QStringLiteral("885 Timpani"),
      QStringLiteral("886 Agogo"),
      QStringLiteral("887 Woodblock"),
      QStringLiteral("888 Castanets"),
      QStringLiteral("889 Taiko"),
      QStringLiteral("890 Concert BD"),
      QStringLiteral("891 Melo. Tom 1"),
      QStringLiteral("892 Melo. Tom 2"),
      QStringLiteral("893 Synth Drum"),
      QStringLiteral("894 808 Tom"),
      QStringLiteral("895 Elec Perc"),
      QStringLiteral("896 Reverse Cymb"),
      QStringLiteral("897 Standard 1"),
      QStringLiteral("898 Standard 2"),
      QStringLiteral("899 Standard 3"),
      QStringLiteral("900 Rock Kit"),
      QStringLiteral("901 Jazz Kit"),
      QStringLiteral("902 Brush Kit"),
      QStringLiteral("903 Machine Kit"),
      QStringLiteral("904 R&B T-Analog"),
      QStringLiteral("905 R&B Mini Kit"),
      QStringLiteral("906 HipHop Kit"),
      QStringLiteral("907 R&B Kit"),
      QStringLiteral("908 Dance Kit 1"),
      QStringLiteral("909 Dance Kit 2"),
      QStringLiteral("910 Dance Kit 3")
};

