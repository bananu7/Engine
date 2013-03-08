#include "Resource.h"
#include <istream>
#include <fstream>
#include <string>

namespace engine {

using namespace std;

std::vector<unsigned char> buffer_from_file(std::istream& stream)
{
    typedef std::istream::pos_type pos_type; 
    typedef std::istream::off_type off_type;
    typedef std::istream_iterator<unsigned char> i_iter;
 
    std::vector<unsigned char> ret;
    stream >> std::noskipws;
    pos_type pre = stream.tellg();
    if (stream.seekg(0, std::ios_base::end)) {
        ret.resize(stream.tellg());
        stream.seekg(pre, std::ios_base::beg);
    }
    std::copy(i_iter(stream), i_iter(), std::back_inserter(ret));
    return ret;
}

} // namespace engine