#ifndef IO_H
#define IO_H

class QString;
class QImage;

class IO
{
public:
    IO();
    static void saveImage(QString &path, QImage* image, bool override = false);
};

#endif // IO_H
