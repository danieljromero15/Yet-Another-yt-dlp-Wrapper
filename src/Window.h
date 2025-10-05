//
// Created by daniel on 7/30/25.
//

#ifndef WINDOW_H
#define WINDOW_H

#include "filedownloader.h"
#include <QApplication>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QProcess>
#include <QProgressBar>


class QPushButton;

class Window final : public QWidget {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);

private slots:
    void onConfirmButtonClicked() const;
    void onTextChanged() const;
    void addVideoDetailsLabel();
    void loadThumbnail() const;
    void outputToCommandLineOutput(const QString &outputString) const;

private:
    //QString commandLineString = "yt-dlp";
    QProcess *ytdlpProcess;
    QProcess *videoDetailsProcess;
    QPushButton *confirmButton;
    QLineEdit *ytStringEdit;
    QLabel *executableFound;
    QLabel *videoThumbnail;
    QLabel *videoData;
    QTextEdit *commandLineOutput;
    FileDownloader *thumbnailCtrl;
    QProgressBar *downloadProgress;
signals:
public slots:
};


#endif //WINDOW_H
