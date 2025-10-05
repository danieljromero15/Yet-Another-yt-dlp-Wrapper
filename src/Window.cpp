//
// Created by daniel on 7/30/25.
//

#include "Window.h"
#include "filedownloader.h"

#include <QPushButton>
#include <QJsonObject>
#include <QDate>
#include <QScrollBar>


QString commandLineString = "yt-dlp";
bool executableFoundBool = false;

Window::Window(QWidget *parent) : QWidget(parent) {
    setFixedSize(800, 600);
    const int winWidth = this->width();
    const int winHeight = this->height();

    ytdlpProcess = new QProcess(this);
    ytdlpProcess->setProgram("yt-dlp");
    connect(ytdlpProcess, &QProcess::readyReadStandardOutput, this, [this] {
        outputToCommandLineOutput(QString::fromUtf8(ytdlpProcess->readAllStandardOutput()));
    });
    connect(ytdlpProcess, &QProcess::readyReadStandardError, this, [this] {
        outputToCommandLineOutput(QString::fromUtf8(ytdlpProcess->readAllStandardError()));
    });
    connect(ytdlpProcess, &QProcess::finished, this, [this] {
        outputToCommandLineOutput(QString("Process finished"));
    });

    videoDetailsProcess = new QProcess(this);
    connect(videoDetailsProcess, &QProcess::finished, this, &Window::addVideoDetailsLabel);

    confirmButton = new QPushButton("Confirm", this);
    confirmButton->setGeometry(winWidth / 2 - 75 / 2, winHeight - 24 * 2, 75, 24);
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(onConfirmButtonClicked()));

    ytStringEdit = new QLineEdit(this);
    const int ytStringWidth = winWidth / 2;
    constexpr int ytStringHeight = 24;
    ytStringEdit->setGeometry(winWidth / 2 - ytStringWidth / 2, 24, ytStringWidth, ytStringHeight);
    ytStringEdit->setAlignment(Qt::AlignCenter);
    connect(ytStringEdit, &QLineEdit::textChanged, this, &Window::onTextChanged);

    executableFound = new QLabel(this);
    executableFound->setGeometry(0, 24, winWidth / 4, 24);
    executableFound->setAlignment(Qt::AlignCenter);

    //const QUrl imageUrl("");
    //thumbnailCtrl = new FileDownloader(QUrl(), this);
    //connect(thumbnailCtrl, SIGNAL(downloaded()), this, SLOT(loadThumbnail()));

    videoThumbnail = new QLabel(this);
    videoThumbnail->setGeometry(0, 56, 171, 96);
    videoThumbnail->setAlignment(Qt::AlignCenter);
    videoThumbnail->setFrameShape(QFrame::Panel);
    videoThumbnail->setScaledContents(true);

    videoData = new QLabel(this);
    videoData->setGeometry(171, 56, winWidth - 171, 48*2);
    videoData->setWordWrap(false);
    videoData->setFrameShape(QFrame::Panel);

    if (system("which yt-dlp > /dev/null 2>&1")) {
        executableFound->setText("✗ yt-dlp not found!");
    } else {
        executableFound->setText("✓ yt-dlp found!");
        executableFoundBool = true;
    }

    commandLineOutput = new QTextEdit(this);
    const int commandLineWidth = winWidth / 1.2;
    const int commandLineHeight = winHeight / 8;
    commandLineOutput->setReadOnly(true);
    commandLineOutput->setGeometry(winWidth / 2 - commandLineWidth / 2, winHeight / 1.3, commandLineWidth,
                                   commandLineHeight);

    downloadProgress = new QProgressBar(this);
    downloadProgress->setGeometry(winWidth / 2 - commandLineWidth / 2, winHeight / 1.45, commandLineWidth,
                                   commandLineHeight);
    downloadProgress->setTextVisible(true);
}

void Window::onTextChanged() const {
    if (!executableFoundBool) {
        commandLineOutput->append("yt-dlp not found. Cannot run command.");
        return;
    }

    if (const QRegularExpression yt_regex("(youtube|youtu).(com|be)\\/"); yt_regex.match(ytStringEdit->text()).hasMatch()) {
        const QStringList args = QProcess::splitCommand(
            "--dump-json " + ytStringEdit->text());
        videoDetailsProcess->start("yt-dlp", args);
    }
}

void Window::addVideoDetailsLabel() {
    const QByteArray output = videoDetailsProcess->readAllStandardOutput();

    // " | jq '{title: .title, uploader: .uploader, upload_date: .upload_date, duration: .duration, thumbnail: .thumbnail}'"

    if (const QJsonDocument doc = QJsonDocument::fromJson(output); !doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();
        const QString title = obj["title"].toString();
        const QString uploader = obj["uploader"].toString();
        const QString uploadDate = QDate::fromString(obj["upload_date"].toString(), "yyyyMMdd").toString("MM/dd/yyyy");

        const QString duration = QTime(0,0).addSecs(obj["duration"].toInt())
            .toString(obj["duration"].toInt() < 3600 ? "mm:ss" : "H:mm:ss");
        const QString thumbnail_url = obj["thumbnail"].toString();

        thumbnailCtrl = new FileDownloader(QUrl(thumbnail_url), this);
        connect(thumbnailCtrl, SIGNAL(downloaded()), this, SLOT(loadThumbnail()));
        videoData->setText(title + "\n" + uploader + "\n" + uploadDate + "\n" + duration);
    }
}

void Window::onConfirmButtonClicked() const {
    commandLineOutput->setText("");
    downloadProgress->setValue(0);
    if (!executableFoundBool) {
        commandLineOutput->append("yt-dlp not found. Cannot run command.");
        return;
    }

    const QString args = "" + ytStringEdit->text().trimmed();
    commandLineOutput->append("Running: yt-dlp " + args);

    commandLineOutput->append("----------------------------\n");

    const QStringList arguments = QProcess::splitCommand(args);

    // Start the process
    ytdlpProcess->start("yt-dlp", arguments);
}

void Window::outputToCommandLineOutput(const QString &outputString) const {
    //qInfo() << outputString;
    if (const QRegularExpression downloadRegex("^\\r\\[download\\]( ){1,3}\\d{1,3}.\\d{1,2}% of.*$"); downloadRegex.match(outputString).hasMatch()) {
        // progress bar stuff
        QStringList downloadStrings = outputString.split(" ");
        downloadStrings.removeAll(QString(""));
        QString downloadPercent = downloadStrings[1];
        downloadPercent.chop(1);
        downloadProgress->setValue(downloadPercent.split(".")[0].toInt());
    }else {
        commandLineOutput->insertPlainText(outputString);
    }
    commandLineOutput->verticalScrollBar()->setValue(commandLineOutput->verticalScrollBar()->maximum());
}

void Window::loadThumbnail() const {
    if (QPixmap thumbnail; thumbnail.loadFromData(thumbnailCtrl->downloadedData())) {
        videoThumbnail->setPixmap(thumbnail);
    }else {
        commandLineOutput->append("Failed to load thumbnail");
    }
}
