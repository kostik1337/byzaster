#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>

class Clipboard : public QObject
{
    Q_OBJECT
public:
    explicit Clipboard(QObject *parent = nullptr);
    
signals:
    
public slots:
    void copy(QString filename);
};

#endif // CLIPBOARD_H
