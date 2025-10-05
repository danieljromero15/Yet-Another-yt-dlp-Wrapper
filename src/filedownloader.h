//
// Created by daniel on 10/4/25.
//

#ifndef YET_ANOTHER_YTDL_WRAPPER_FILEDOWNLOADER_H
#define YET_ANOTHER_YTDL_WRAPPER_FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT
    public:
    explicit FileDownloader(QUrl imageUrl, QObject *parent = nullptr);
    virtual ~FileDownloader();
    QByteArray downloadedData() const;

    signals:
     void downloaded();

private slots:
 void fileDownloaded(QNetworkReply* pReply);
private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
};

#endif //YET_ANOTHER_YTDL_WRAPPER_FILEDOWNLOADER_H