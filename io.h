#ifndef IO_H
#define IO_H

class QString;
class QImage;

class IO
{
public:
    IO();
    static void saveImage(QString* fileName, QImage* image);
};

#endif // IO_H
