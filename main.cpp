#include <QApplication>
#include "src/Window.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.setWindowTitle("Yet Another yt-dlp Wrapper");

    window.show();
    return QApplication::exec();
}
