#ifndef CPWEBP_H
#define CPWEBP_H


#include "cpwebp_global.h"

class QImage;
class QString;

class CP_WEBP_EXPORT CPWebP
{
public:
    CPWebP();
public:
    int save( const QImage &img, const QString &filepath, int target_size);
    bool write( const QImage &img , QString &filepath);
    bool read(const QString &filepath, QImage* img_pointer);
    bool readFromData(unsigned char* data, int data_len, QImage* img_pointer);
private:
    int quality;
};

#endif // CPWEBP_H
