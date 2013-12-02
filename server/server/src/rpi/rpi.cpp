#include <evse/rpi/rpi.hpp>

#include <QProcess>
#include <QString>
#include <QRegExp>
#include <QVariant>

int g_iWait = 1000;

void evse::rpi::setMaximumWait(int wait)
{
    g_iWait = wait;
}

float evse::rpi::getTemperature()
{
    // Starten des Prozesses; Warten bis er beendet ist.
    QProcess tmp;
    tmp.start(QString("/opt/vc/bin/vcgencmd"), QStringList(QString("measure_temp")));
    tmp.waitForFinished(g_iWait);

    // Auslesen der Ausgabe und mithilfe von RegEx zerlegen und den Wert zurückgeben
    QString response = QString(tmp.readAllStandardOutput());
    QRegExp rx("(\\d*\\.\\d+)");
    if(rx.indexIn(response) != -1)
        return QVariant(rx.cap(1)).toFloat();

    return -1.0f;
}



float getVoltage(QString cmd)
{
    // Starten des Prozesses; Warten bis er beendet ist.
    QProcess tmp;
    tmp.start(QString("/opt/vc/bin/vcgencmd"), QStringList({QString("measure_volts"), QString(cmd)}));
    tmp.waitForFinished(g_iWait);

    // Auslesen der Ausgabe und mithilfe von RegEx zerlegen und den Wert zurückgeben
    QString response = QString(tmp.readAllStandardOutput());
    QRegExp rx("(\\d*\\.\\d+)");
    if(rx.indexIn(response) != -1)
        return QVariant(rx.cap(1)).toFloat();

    return -1.0f;
}

float evse::rpi::getCorevoltage()
{
    return getVoltage("core");
}

float evse::rpi::getSDRAMCvoltage()
{
    return getVoltage("sdram_c");
}

float evse::rpi::getSDRAMIvoltage()
{
    return getVoltage("sdram_i");
}

float evse::rpi::getSDRAMPvoltage()
{
    return getVoltage("sdram_p");
}


std::uint64_t getFrequency(QString cmd)
{
    // Starten des Prozesses; Warten bis er beendet ist.
    QProcess tmp;
    tmp.start(QString("/opt/vc/bin/vcgencmd"), QStringList({QString("measure_clock"), QString(cmd)}));
    tmp.waitForFinished(g_iWait);

    // Auslesen der Ausgabe und mithilfe von RegEx zerlegen und den Wert zurückgeben
    QString response = QString(tmp.readAllStandardOutput());
    QRegExp rx(".*\\=(\\d*)");
    if(rx.indexIn(response) != -1){
        return QVariant(rx.cap(1)).toInt();
    }

    return 0;
}


std::uint64_t evse::rpi::getARMfrequency()
{
    return getFrequency("arm");
}

std::uint64_t evse::rpi::getCorefrequency()
{
    return getFrequency("core");
}

std::uint64_t evse::rpi::getH264frequency()
{
    return getFrequency("h264");
}

std::uint64_t evse::rpi::getISPfrequency()
{
    return getFrequency("isp");
}

std::uint64_t evse::rpi::getV3Dfrequency()
{
    return getFrequency("v3d");
}

std::uint64_t evse::rpi::getUARTfrequency()
{
    return getFrequency("uart");
}

std::uint64_t evse::rpi::getPWMfrequency()
{
    return getFrequency("pwm");
}

std::uint64_t evse::rpi::getEMMCfrequency()
{
    return getFrequency("emmc");
}

std::uint64_t evse::rpi::getPixelfrequency()
{
    return getFrequency("pixel");
}

std::uint64_t evse::rpi::getVECfrequency()
{
    return getFrequency("vec");
}

std::uint64_t evse::rpi::getHDMIfrequency()
{
    return getFrequency("hdmi");
}

std::uint64_t evse::rpi::getDPIfrequency()
{
    return getFrequency("dpi");
}
