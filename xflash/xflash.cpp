#include <hdf5.h>

void XFLASH_METADATA::ReadFile(XSTRING cFilename)
{
    fileId = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

}
