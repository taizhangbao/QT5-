#include <QApplication>
#include "showmaclk.h"

int main(int argc, char *argv[])
{
#if 0
    QApplication a(argc, argv);
    ShowmacLk w;
    w.show();

    return a.exec();
#endif
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(SHOWMAC_LK_TOOL);
    ShowmacLk shmacLk;
    shmacLk.show();
    return app.exec();
}
