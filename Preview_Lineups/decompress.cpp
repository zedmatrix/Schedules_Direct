#include "mainwindow.h"
/*
    Zlib decompress functions for the /20141201/programs /20141201/schedules /20141201/metadata/description
*/
QByteArray MainWindow::decompressGzip(const QByteArray &data) {
    if (data.size() <= 4) {
        qDebug() << "Gzip data is too small.";
        return QByteArray();
    }

    QByteArray decompressedData;
    z_stream strm;
    memset(&strm, 0, sizeof(strm));

    if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK) {
        qDebug() << "Failed to initialize zlib.";
        return QByteArray();
    }

    strm.next_in = (Bytef *)data.data();
    strm.avail_in = data.size();

    int bufferSize = 4096;
    char buffer[bufferSize];

    do {
        strm.next_out = reinterpret_cast<Bytef *>(buffer);
        strm.avail_out = bufferSize;

        int ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            qDebug() << "Zlib decompression error.";
            inflateEnd(&strm);
            return QByteArray();
        }

        decompressedData.append(buffer, bufferSize - strm.avail_out);
    } while (strm.avail_out == 0);

    inflateEnd(&strm);
    return decompressedData;
}
