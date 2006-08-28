#ifndef _OPTION_H
#define _OPTION_H

#include <list>
#include <map>
#include <string>
#include <exception>

#include "error.h"

struct option_handler_t
{
  const std::string long_opt;
  const char	    short_opt;
  const bool	    wants_arg;

  enum option_source_t {
    init_file    = 0x1,
    environment  = 0x2,
    data_file    = 0x4,
    command_line = 0x8
  };

  unsigned short handled;

  option_handler_t(const std::string& _long_opt,
		   const char	      _short_opt,
		   const bool	      _wants_arg)
    : long_opt(_long_opt), short_opt(_short_opt),
      wants_arg(_wants_arg) {}

  virtual ~option_handler_t() {}

  virtual bool prep(option_source_t source);
  virtual void run(const char * arg) = 0;
};

typedef std::map<const std::string, option_handler_t *>
  long_option_handlers_map;
typedef std::pair<const std::string, option_handler_t *>
  long_option_handlers_pair;

extern long_option_handlers_map long_option_handlers;

typedef std::map<const char, option_handler_t *>
  short_option_handlers_map;
typedef std::pair<const char, option_handler_t *>
  short_option_handlers_pair;

extern short_option_handlers_map short_option_handlers;

typedef std::list<option_handler_t *> option_handlers_list;

extern option_handlers_list all_option_handlers;

void set_option_handler(option_handler_t * handler);
void clear_option_handlers();

typedef void (*handler_t)(const char * arg);

struct static_option_t {
  const char * long_opt;
  char	       short_opt;

  option_handler_t * handler;
};

class option_error : public error {
 public:
  option_error(const std::string& reason) throw() : error(reason) {}
  virtual ~option_error() throw() {}
};

bool process_option(static_option_t * options, const std::string& opt,
		    const char * arg = NULL);
void process_arguments(static_option_t * options, int argc, char ** argv,
		       const bool anywhere, std::list<std::string>& args);
void process_environment(static_option_t * options, const char ** envp,
			 const std::string& tag);

namespace ledger {

class config_t;
class report_t;

extern config_t * config;
extern report_t * report;

#define OPTIONS_SIZE 97
extern static_option_t options[OPTIONS_SIZE];

void help(std::ostream& out);

#define OPT_BEGIN(tag, chars)					\
  struct option_ ## tag : public option_handler_t {		\
    option_ ## tag(const std::string& long_opt,			\
		   const char	      short_opt,		\
		   const bool	      wants_arg)		\
      : option_handler_t(long_opt, short_opt, wants_arg) {}	\
								\
    virtual void run(const char * optarg)

#define OPT_END(tag)				\
    }

} // namespace ledger

#endif // _OPTION_H
