#include "cpwebp.h"

#include "webp/decode.h"
#include "webp/encode.h"
#include <QImage>
#include <QFile>
#include <QDebug>


#define W_CHAR wchar_t  // WCHAR without underscore might already be defined.
#define TO_W_CHAR(STR) (L##STR)

#define WFOPEN(ARG, OPT) _wfopen((const W_CHAR*)ARG, TO_W_CHAR(OPT))
#define WSTRCMP(FILENAME, STR) wcscmp((const W_CHAR*)FILENAME, TO_W_CHAR(STR))
#define WFOPEN(ARG, OPT) _wfopen((const W_CHAR*)ARG, TO_W_CHAR(OPT))

static int MyFileWriter(const uint8_t* data, size_t data_size,
                    const WebPPicture* const pic) {
  FILE* const out = (FILE*)pic->custom_ptr;
  return data_size ? (fwrite(data, data_size, 1, out) == 1) : 1;
}

CPWebP::CPWebP():quality(100)
{

}

int CPWebP::save(const QImage &img, QString &filepath) {
    // Setup a config, starting form a preset and tuning some additional
    // parameters
    float quality_factor = 1;
    WebPConfig config;
    if (!WebPConfigPreset(&config, WEBP_PRESET_PHOTO, quality_factor)) {
      return 0;   // version error
    }
    // ... additional tuning
    config.sns_strength = 90;
    config.filter_sharpness = 6;
    int config_error = WebPValidateConfig(&config);  // not mandatory, but useful

    // Setup the input data
    WebPPicture pic;
    if (!WebPPictureInit(&pic)) {
      return 0;  // version error
    }



    QImage image = img;
    bool alpha = image.hasAlphaChannel();
    unsigned pixel_count = alpha ? 4 : 3;
    unsigned stride = pixel_count * image.width();


    pic.width = stride;
    pic.height = img.height();
    // allocated picture of dimension width x height
    if (!WebPPictureAlloc(&pic)) {
      return 0;   // memory error
    }


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

    // at this point, 'pic' has been initialized as a container,
    // and can receive the Y/U/V samples.
    // Alternatively, one could use ready-made import functions like
    // WebPPictureImportRGB(), which will take care of memory allocation.
    // In any case, past this point, one will have to call
    // WebPPictureFree(&pic) to reclaim memory.
    WebPPictureImportRGB(&pic, data, stride);

    free(data);

    // Set up a byte-output write method. WebPMemoryWriter, for instance.
    WebPMemoryWriter wrt;
    WebPMemoryWriterInit(&wrt);     // initialize 'wrt'

    FILE* out = NULL;
    const char *out_file = "test2.webp";
    std::wstring  testWstring  = filepath.toStdWString();
    //const int use_stdout = !WSTRCMP(out_file, "-");
    out = WFOPEN(testWstring.c_str(), "wb");
    pic.writer = MyFileWriter;
    pic.custom_ptr = (void*)out;

    // Compress!
    int ok = WebPEncode(&config, &pic);   // ok = 0 => error occurred!
    qDebug() << "WebPEncode: " << ok << " error code: " << pic.error_code;
    WebPPictureFree(&pic);  // must be called independently of the 'ok' result.

    // output data should have been handled by the writer at that point.
    // -> compressed data is the memory buffer described by wrt.mem / wrt.size

    // deallocate the memory used by compressed data
    WebPMemoryWriterClear(&wrt);
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
