//
// Created by ljohnson on 3/8/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

enum Severity {
    trace,
    debug,
    info,
    warning,
    error,
    fatal
};


class Logger {
public:
   static void info(const std::string& message) {
       BOOST_LOG_TRIVIAL(info) << message;
   }

   static void warning(const std::string& message) {
       BOOST_LOG_TRIVIAL(warning) << message;
   }

   static void error(const std::string& message) {
       BOOST_LOG_TRIVIAL(error) << message;
   }

   static void fatal(const std::string& message) {
       BOOST_LOG_TRIVIAL(fatal) << message;
   }

   static void debug(const std::string& message) {
       BOOST_LOG_TRIVIAL(debug) << message;
   }

};

#endif //LOGGER_H