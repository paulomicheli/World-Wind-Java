#include "filedatasource.h"
#include "cpl_vsi.h"
#include "cpl_error.h"

FileDataSource::FileDataSource(const char * fileName)
  : closeFile(true)
{
    fp = VSIFOpenL(fileName, "rb");
}

FileDataSource::FileDataSource(FILE * fp)
: closeFile(false)
{
    this->fp = fp;
}

FileDataSource::~FileDataSource()
{
    if (closeFile)
        VSIFCloseL(fp);
}

size_t FileDataSource::DataSourceFread(void* lpBuf, size_t size, size_t count)
{
    return VSIFReadL(lpBuf, size, count, fp);
}

int FileDataSource::DataSourceFgetc()
{
    char chData;

    if( VSIFReadL( &chData, 1, 1, fp ) == 1 )
        return chData;
    else
        return EOF;
}

int FileDataSource::DataSourceUngetc(int c)
{
    DataSourceFseek(-1, SEEK_CUR );
    
    return c;
}

int FileDataSource::DataSourceFseek(long offset, int origin)
{
    if (origin == SEEK_CUR && offset < 0)
        return VSIFSeekL(fp, VSIFTellL(fp) + offset, SEEK_SET);
    else
        return VSIFSeekL(fp, offset, origin);
}

int FileDataSource::DataSourceFeof()
{
    return VSIFEofL( fp );
}

long FileDataSource::DataSourceFtell()
{
    return VSIFTellL( fp );
}
