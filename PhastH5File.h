#pragma once

#include "H5Cpp.h"
using namespace H5;

#include <string>
#include <vector>

class PhastH5File : public H5File
{
public:
    PhastH5File(const char* name, unsigned int flags,
        const FileCreatPropList& create_plist = FileCreatPropList::DEFAULT,
        const FileAccPropList& access_plist = FileAccPropList::DEFAULT);

    std::vector<std::string> getFixedStrings(const std::string& path);

    std::vector<std::string> getScalars();
    std::vector<std::string> getTimeSteps();

    size_t getTimeStepCount();
    size_t getScalarCount();

    std::vector<double> PhastH5File::getActiveArray(int time_idx);
private:
    std::vector<double> PhastH5File::getActiveArray(const std::string& time);

    std::vector<std::string> m_scalars;
    std::vector<std::string> m_timesteps;

    std::vector<double> m_xs;
    std::vector<double> m_ys;
    std::vector<double> m_zs;

    size_t m_nx;
    size_t m_nxy;
    size_t m_nxyz;
};

