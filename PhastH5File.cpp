#include "PhastH5File.h"

#include <iostream>


PhastH5File::PhastH5File(const char* name, unsigned int flags,
    const FileCreatPropList& create_plist,
    const FileAccPropList& access_plist)
: H5File(name, flags, create_plist, access_plist)
{
}

std::vector<std::string> PhastH5File::getFixedStrings(const std::string& path)
{
    hsize_t dims[2], maxdims[2];

    DataSet dataset = openDataSet(path);
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
    if (m_timesteps.empty()) {
        m_timesteps = getFixedStrings("/TimeSteps");
    }
    return m_timesteps;
}

size_t PhastH5File::getTimeStepCount()
{
    return getTimeSteps().size();
}

std::vector<std::string> PhastH5File::getScalars()
{
    if (m_scalars.empty()) {
        m_scalars = getFixedStrings("/Scalars");
    }
    return m_scalars;
}

size_t PhastH5File::getScalarCount()
{
    return getScalars().size();
}

std::vector<double> PhastH5File::getActiveArray(int time_idx)
{
    std::vector<std::string> timesteps = getTimeSteps();
    if (time_idx < 0) throw new std::exception();
    if (time_idx >= m_timesteps.size()) throw new std::exception();
    return getActiveArray(m_timesteps[time_idx]);
}

std::vector<double> PhastH5File::getActiveArray(const std::string& timestep)
{
    hsize_t dims[2], maxdims[2];
    DataSet dataset = openDataSet(std::string("/") + timestep + "/ActiveArray");
    DataSpace dataspace = dataset.getSpace();

    int ndims = dataspace.getSimpleExtentDims(dims, maxdims);

    std::cout << "ndims = " << ndims << std::endl;
    std::cout << "dims[0] = " << dims[0] << std::endl;
    std::cout << "maxdims[0] = " << maxdims[0] << std::endl;

    std::vector<double> active(dims[0], 0.0);
    dataset.read(active.data(), PredType::NATIVE_DOUBLE);

    return active;
}
