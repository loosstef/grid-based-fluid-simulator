#include "io.h"
#include <QString>
#include <QImage>

IO::IO()
{

}

void IO::saveImage(QString *fileName, QImage *image)
{
    image->save(*fileName);
}
