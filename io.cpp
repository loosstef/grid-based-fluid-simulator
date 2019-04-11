#include "io.h"
#include <QString>
#include <QImage>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>

IO::IO()
{

}

// TODO: maybe clean up this code a bith
void IO::saveImage(QString &path, QImage *image, bool override)
{
    QStringList splittedPath = path.split(QString("/"));
    QString fullPath(path);
    QString pathOnly("");
    QString fileNameOnly = splittedPath.at(splittedPath.length()-1);
    for(int i = 0; i < splittedPath.length()-1; ++i) {
        pathOnly.append(splittedPath.at(i));
        pathOnly.append("/");
    }

    QDir().mkpath(pathOnly);

    int fileIndex = 0;
    QStringList splittedFileName = fileNameOnly.split(QString("."));
    while(QFileInfo::exists(fullPath) && !override) {
        Q_ASSERT(splittedFileName.length() == 2);
        QString newFileName(splittedFileName.at(0));
        newFileName.append(QString::number(fileIndex));
        newFileName.append(".");
        newFileName.append(splittedFileName.at(1));
        fullPath = pathOnly;
        fullPath.append(newFileName);
        ++fileIndex;
    }

    image->save(fullPath);
}
