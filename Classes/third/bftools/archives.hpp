//
//  archives.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#ifndef archives_hpp
#define archives_hpp

#include <stdio.h>
#include "macros.h"
#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/complex.hpp"

NS_BF_BEGIN

class Archives {
public:
    typedef std::stringstream Stream;
    typedef cereal::BinaryOutputArchive OutputArchive;
    typedef cereal::BinaryInputArchive InputArchive;
    typedef std::function<void(OutputArchive& archive)> OutputFunc;
    typedef std::function<void(InputArchive& archive)> InputFunc;
public:
    Archives();
public:
    void setStream(std::string const& str);
    Stream const& getStream() const;
    Stream& getStream();
public:
    void Get(InputFunc func);
    void Set(OutputFunc func);
private:
    Stream mStream;
};

NS_BF_END

#endif /* archives_hpp */
