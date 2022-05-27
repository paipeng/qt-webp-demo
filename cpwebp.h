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
    void save(QImage &image);
    bool write( const QImage &img , QString &filepath);
private:
    int quality;
};

#endif // CPWEBP_H
