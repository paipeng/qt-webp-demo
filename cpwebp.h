#ifndef CPWEBP_H
#define CPWEBP_H


//
class QImage;
class QString;

class CPWebP
{
public:
    CPWebP();
public:
    int save( const QImage &img, QString &filepath, int target_size);
    bool write( const QImage &img , QString &filepath);
    bool read(const QString &filepath, QImage* img_pointer);
private:
    int quality;
};

#endif // CPWEBP_H
