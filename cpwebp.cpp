#include "cpwebp.h"

#include "webp/decode.h"
#include "webp/encode.h"
#include <QImage>
#include <QFile>

CPWebP::CPWebP():quality(100)
{

}

void CPWebP::save(QImage &image) {

}



bool CPWebP::write( const QImage &img, QString &filepath){
    QImage image = img;
    bool alpha = image.hasAlphaChannel();
    unsigned pixel_count = alpha ? 4 : 3;
    unsigned stride = pixel_count * image.width();

    uint8_t* data = new uint8_t[ stride * image.height() ];
    if( !data )
        return false;

    //Make sure the input is in ARGB
    if( image.format() != QImage::Format_RGB32 && image.format() != QImage::Format_ARGB32 )
        image = image.convertToFormat( QImage::Format_ARGB32 );

    for( int iy=0; iy<image.height(); ++iy ){
        const QRgb* in = (const QRgb*)image.constScanLine( iy );
        uint8_t* out = data + iy*stride;

        for( int ix=0; ix<image.width(); ++ix, ++in ){
            *(out++) = qRed( *in );
            *(out++) = qGreen( *in );
            *(out++) = qBlue( *in );
            if( alpha )
                *(out++) = qAlpha( *in );
        }
    }

    uint8_t* output = NULL;
    size_t size;

    if( quality == 100 ){
        //Lossless
        if( alpha )
            size = WebPEncodeLosslessRGBA( data, image.width(), image.height(), stride, &output );
        else
            size = WebPEncodeLosslessRGB( data, image.width(), image.height(), stride, &output );
    }
    else{
        //Lossy
        if( alpha )
            size = WebPEncodeRGBA( data, image.width(), image.height(), stride, quality+1, &output );
        else
            size = WebPEncodeRGB( data, image.width(), image.height(), stride, quality+1, &output );
    }

    delete[] data;
    if( !output || size == 0 )
        return false;

    //device()->write( (char*)output, size );



    QFile file(filepath);

    if (!file.open(QIODevice::WriteOnly))
    {

    }

    QDataStream out(&file);
    out.writeRawData((char*)output, size);

    file.close();
    free( output );

    return true;
}
