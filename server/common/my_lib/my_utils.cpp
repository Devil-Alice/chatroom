#include "my_utils.h"

namespace my_utils
{
    string generate_uuid()
    {
        boost::uuids::random_generator gen;
        return boost::uuids::to_string(gen());
    }

}
