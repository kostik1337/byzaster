#include "clipboard.h"
#include <QUrl>
#include <QGuiApplication>
#include <QMimeData>
#include <QClipboard>
#include <QString>
#include <QDebug>

Clipboard::Clipboard(QObject *parent) : QObject(parent)
{
    
}

void Clipboard::copy(QString filename)
{
    QMimeData* mda = new QMimeData;
    QString s(QUrl::fromLocalFile(filename).toEncoded());
    mda->setData("text/uri-list", s.toStdString().c_str());
    QGuiApplication::clipboard()->setMimeData(mda);
}
