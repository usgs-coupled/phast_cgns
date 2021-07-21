#include "PhastH5File.h"


PhastH5File::PhastH5File(const char* name, unsigned int flags,
    const FileCreatPropList& create_plist,
    const FileAccPropList& access_plist)
: H5File(name, flags, create_plist, access_plist)
{
}

std::vector<std::string> PhastH5File::getFixedStrings(const std::string& path)
{
    hsize_t dims[2], maxdims[2];

    DataSet dataset = this->openDataSet(path);
    DataSpace dataspace = dataset.getSpace();
    int ndims = dataspace.getSimpleExtentDims(dims, maxdims);

    size_t n = dims[0];

    StrType strtype = dataset.getStrType();
    H5T_str_t strt = strtype.getStrpad();
    /*
    H5T_STR_ERROR    = -1,  // error
    H5T_STR_NULLTERM = 0,   // null terminate like in C
    H5T_STR_NULLPAD  = 1,   // pad with nulls
    H5T_STR_SPACEPAD = 2,   // pad with spaces like in Fortran
    */

    std::vector<std::string> strs;

    if (strt == H5T_STR_NULLTERM) {
        // fixed length strings
        size_t len = strtype.getSize();
        char* buffer = new char[len * n];

        dataset.read(buffer, strtype, H5S_ALL);
        for (size_t i = 0; i < n; ++i) {
            std::string str(buffer + i * len);
            strs.push_back(str);
        }
        delete[] buffer;
    }
    else if (strt == H5T_STR_NULLPAD) {
        // variable length strings
        char** szs = new char* [n];

        dataset.read(szs, strtype, H5S_ALL);
        for (int i = 0; i < n; ++i) {
            std::string str(szs[i]);
            strs.push_back(str);
        }
        delete[] szs;
    }

    return strs;
}

std::vector<std::string> PhastH5File::getTimeSteps()
{
    return getFixedStrings("/TimeSteps");
}

std::vector<std::string> PhastH5File::getScalars()
{
    return getFixedStrings("/Scalars");
}
