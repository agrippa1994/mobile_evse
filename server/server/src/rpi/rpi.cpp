#include <evse/rpi/rpi.hpp>

#include <QProcess>
#include <QString>
#include <QRegExp>
#include <QVariant>

float evse::rpi::getTemperature(int maxwait)
{
    // Starten des Prozesses; Warten bis er beendet ist.
    QProcess tmp;
    tmp.start(QString("/opt/vc/bin/vcgencmd"), QStringList(QString("measure_temp")));
    tmp.waitForFinished(maxwait);

    // Auslesen der Ausgabe und mithilfe von RegEx zerlegen und den Wert zurückgeben
    QString response = QString(tmp.readAllStandardOutput());
    QRegExp rx("(\\d*\\.\\d+)");
    if(rx.indexIn(response) != -1)
        return QVariant(rx.cap(1)).toFloat();

    return -1.0f;
}

float evse::rpi::getCoreVoltage(int maxwait)
{
    // Starten des Prozesses; Warten bis er beendet ist.
    QProcess tmp;
    tmp.start(QString("/opt/vc/bin/vcgencmd"), QStringList({QString("measure_volts"), QString("core")}));
    tmp.waitForFinished(maxwait);

    // Auslesen der Ausgabe und mithilfe von RegEx zerlegen und den Wert zurückgeben
    QString response = QString(tmp.readAllStandardOutput());
    QRegExp rx("(\\d*\\.\\d+)");
    if(rx.indexIn(response) != -1)
        return QVariant(rx.cap(1)).toFloat();

    return -1.0f;
}
