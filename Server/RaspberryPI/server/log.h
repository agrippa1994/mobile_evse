#ifndef LOG_H
#define LOG_H
#include <boost/log/trivial.hpp>

#define LOG(t) BOOST_LOG_TRIVIAL(t) \
    << __FILE__ << " @  " \
    << __LINE__ << " @  " \
    << __FUNCTION__ << "\t" \


#endif // LOG_H
