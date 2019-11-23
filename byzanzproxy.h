#ifndef BYZANZPROXY_H
#define BYZANZPROXY_H

#include <QObject>
#include <QProcess>
#include <QRect>
#include <QString>

class ByzanzProxy : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(bool recording READ recording NOTIFY recordingChanged)
    Q_PROPERTY(bool waiting READ waiting NOTIFY waitingChanged)
    Q_PROPERTY(QString filename READ filename NOTIFY filenameChanged)
    
    // Byzanz args
    Q_PROPERTY(bool recordCursor READ recordCursor WRITE setRecordCursor
               NOTIFY recordCusorChanged)
    Q_PROPERTY(bool recordAudio READ recordAudio WRITE setRecordAudio
               NOTIFY recordAudioChanged)
    Q_PROPERTY(QRect region READ region WRITE setRegion NOTIFY regionChanged)
    Q_PROPERTY(OutputFormat format READ format WRITE setFormat 
               NOTIFY formatChanged)
    Q_PROPERTY(QString savePath READ savePath WRITE setSavePath 
               NOTIFY savePathChanged)
    
public:
    enum OutputFormat {
        FORMAT_FLV,
        FORMAT_GIF,
        FORMAT_OGV,
        FORMAT_WEBM
    };
    Q_ENUM(OutputFormat)
    
    explicit ByzanzProxy(QObject *parent = nullptr);

    bool recording() const;
    bool recordCursor() const;
    bool recordAudio() const;
    QRect region() const;
    OutputFormat format() const;
    QString filename() const;
    QString savePath() const;
    bool waiting() const;
    
signals:
    void recordingChanged(bool recording);
    void recordCusorChanged(bool recordCursor);
    void recordAudioChanged(bool recordAudio);
    void regionChanged(QRect region);
    void formatChanged(OutputFormat format);
    void filenameChanged(QString filename);
    void savePathChanged(QString savePath);
    void waitingChanged(bool waiting);
    
public slots:
    void startRecording();
    void stopRecording();
    void setRecordCursor(bool recordCursor);
    void setRecordAudio(bool recordAudio);
    void setRegion(QRect region);
    void setFormat(OutputFormat format);
    void setSavePath(QString savePath);
    
private slots:
    void onByzanzStarted();
    void onByzanzFinished(int status);
    
private:
    bool m_recording = false;
    bool m_recordCursor = false;
    bool m_recordAudio = false;
    QRect m_region;
    OutputFormat m_format = OutputFormat::FORMAT_GIF;
    QString m_filename;
    QProcess m_byzanzProcess;
    QString m_savePath = "/tmp/";
    bool m_waiting = false;
    
    QString fileExtension(OutputFormat format);
};

#endif // BYZANZPROXY_H
