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
};

