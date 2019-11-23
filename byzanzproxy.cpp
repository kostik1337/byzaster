#include "byzanzproxy.h"
#include <QProcess>
#include <QDateTime>
#include <QDir>

#include <QDebug>

ByzanzProxy::ByzanzProxy(QObject *parent) : QObject(parent)
{
    connect(&m_byzanzProcess, &QProcess::started, this, 
            &ByzanzProxy::onByzanzStarted);

    connect(&m_byzanzProcess,  QOverload<int>::of(&QProcess::finished), this,
            &ByzanzProxy::onByzanzFinished);
}

void ByzanzProxy::startRecording()
{
    QStringList args;
    QRect view = m_region.normalized();
    
    args << "-x" << QString::number(view.x());
    args << "-y" << QString::number(view.y());
    args << "-w" << QString::number(view.width());
    args << "-h" << QString::number(view.height());
    
    if (recordCursor()) args << "-c";
    if (recordAudio()) args << "-a";
    
    args << "-e" << "sleep infinity";
    
    m_filename = m_savePath + "Screencast-" + 
                QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate) +
                fileExtension(m_format);
    emit filenameChanged(m_filename);
    
    args << m_filename;
    
    m_waiting = true;
    emit waitingChanged(m_waiting);
    
    m_byzanzProcess.start("byzanz-record", args);
    
    qDebug() << "call byzanz-record with args: " << args;
}

void ByzanzProxy::stopRecording()
{
    m_waiting = true;
    emit waitingChanged(m_waiting);
    QProcess().execute("pkill", { "-nf", "sleep infinity" });
}

void ByzanzProxy::onByzanzStarted()
{
    m_recording = true;
    m_waiting = false;
    emit waitingChanged(m_waiting);
    emit recordingChanged(m_recording);
}

void ByzanzProxy::onByzanzFinished(int)
{
    m_recording = false;
    m_waiting = false;
    emit waitingChanged(m_waiting);
    emit recordingChanged(m_recording);
}

QString ByzanzProxy::fileExtension(ByzanzProxy::OutputFormat format)
{
    switch (format) {
    case FORMAT_FLV: return ".flv";
    case FORMAT_GIF: return ".gif";
    case FORMAT_OGV: return ".ogv";
    case FORMAT_WEBM: return ".webm";
    }
}

void ByzanzProxy::setRecordCursor(bool recordCursor)
{
    if (m_recordCursor == recordCursor)
        return;
    
    m_recordCursor = recordCursor;
    emit recordCusorChanged(m_recordCursor);
}

void ByzanzProxy::setRecordAudio(bool recordAudio)
{
    if (m_recordAudio == recordAudio)
        return;
    
    m_recordAudio = recordAudio;
    emit recordAudioChanged(m_recordAudio);
}

void ByzanzProxy::setRegion(QRect region)
{
    if (m_region == region) {
        qDebug() << "same region";
        return;
    }
    
    m_region = region;
    emit regionChanged(m_region);
}

void ByzanzProxy::setFormat(ByzanzProxy::OutputFormat format)
{
    if (m_format == format)
        return;
    
    m_format = format;
    emit formatChanged(m_format);
}

void ByzanzProxy::setSavePath(QString savePath)
{
    if (m_savePath == savePath)
        return;
    if (!savePath.endsWith(QDir::separator())) {
        savePath += QDir::separator();
    }
    m_savePath = savePath;
    emit savePathChanged(m_savePath);
}

bool ByzanzProxy::recording() const
{
    return m_recording;
}

bool ByzanzProxy::recordCursor() const
{
    return m_recordCursor;
}

bool ByzanzProxy::recordAudio() const
{
    return m_recordAudio;
}

QRect ByzanzProxy::region() const
{
    return m_region;
}

ByzanzProxy::OutputFormat ByzanzProxy::format() const
{
    return m_format;
}

QString ByzanzProxy::filename() const
{
    return m_filename;
}

QString ByzanzProxy::savePath() const
{
    return m_savePath;
}

bool ByzanzProxy::waiting() const
{
    return m_waiting;
}
