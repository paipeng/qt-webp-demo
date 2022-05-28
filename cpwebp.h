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
    int save( const QImage &img, QString &filepath);
    bool write( const QImage &img , QString &filepath);
private:
    int quality;
};

#endif // CPWEBP_H
