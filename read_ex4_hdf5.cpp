// read_ex4_hdf5.cpp

#include <iostream>
#include <vector>

#include "H5Cpp.h"
using namespace H5;

int main (void)
{
    H5File file("ex4.h5", H5F_ACC_RDONLY);

    std::vector< std::string > cs = { "/Grid/X", "/Grid/Y" , "/Grid/Z" };
    std::vector< std::vector<double> > coords;
    for (size_t i = 0; i < cs.size(); ++i) {
        DataSet dataset = file.openDataSet(cs[i]);
        DataSpace dataspace = dataset.getSpace();
        hsize_t dims[2];
        int ndims = dataspace.getSimpleExtentDims(dims);
        std::vector<double> coord(dims[0], 0.0);
        dataset.read(coord.data(), PredType::NATIVE_DOUBLE);
        std::cout << cs[i] << std::endl;
        for (auto c : coord) {
            std::cout << c << std::endl;
        }
        coords.push_back(coord);
    }

    file.close();
}