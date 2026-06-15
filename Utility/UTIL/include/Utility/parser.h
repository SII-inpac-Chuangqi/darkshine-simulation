// Simplified from https://github.com/morrisfranken/argparse/tree/master

#ifdef __has_include
#if __has_include("TString.h")
#include "TString.h"
#define HAS_TSTRING
#endif
#else
// Pre-C++17: always include in DarkShine (ROOT always linked)
#include "TString.h"
#define HAS_TSTRING
#endif

#include <type_traits>
#include <cctype>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
//#include <any>
#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>

namespace arg_parser_helper
{

std::vector<std::string> Split(const std::string &str, const char &flag = ',')
{
    std::vector<std::string> splits;
    std::stringstream ss(str);
    std::string key;
    while (std::getline(ss, key, flag))
    {
        if (!key.empty() && key.back() == '\0')
            key.pop_back(); // last variables contain a '\0', which is unexpected when comparing to raw string, e.g. value == "test" will fail when the last character is '\0'. Therefore we can remove it
        splits.emplace_back(std::move(key));
    }
    return splits;
}

std::string StrTolower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return std::tolower(c); }
                  );
    return str;
}

template<typename T> T Convert(const std::string&);
template<> std::string Convert(const std::string &value) { return value; }
template<> int Convert(const std::string &value) { return std::stoi(value); }
template<> long Convert(const std::string &value) { return std::stol(value); }
template<> bool Convert(const std::string &value) {
    auto lower = StrTolower(value);
    if (lower == "false" || lower == "0") return false;
    if (lower == "true") return true;
    try { return std::stoi(value) != 0; }
    catch (...) { return false; }
}
template<> double Convert(const std::string &value) { return std::stod(value); }
template<> float Convert(const std::string &value) { return std::stof(value); }
#ifdef HAS_TSTRING
template<> TString Convert(const std::string &value) { return value.data(); }
#endif

}

namespace arg_parser
{

struct ParserBaseType
{
    ParserBaseType() = default;
    virtual ~ParserBaseType() = default;

    virtual void Convert(const std::string&) {};
};

template<typename T> struct ParserType : public ParserBaseType
{
public:
    ParserType() : ParserBaseType() {}

    explicit ParserType(T &t) : ParserBaseType(), t_(&t) {}
    explicit ParserType(const T &t, bool) : ParserBaseType(), owned_(new T(t)), t_(owned_.get()) {}

    virtual void Convert(const std::string &value) override
    {
        *t_ = arg_parser_helper::Convert<T>(value);
    }

    T Get() const {return *t_;}

private:
    std::unique_ptr<T> owned_;
    T *t_ = nullptr;

    T* GetP() const {return t_;}

    friend struct Entry;
};

struct Entry
{
public:
    ~Entry() = default;

    template <typename T> Entry(const std::string &key, const std::string &short_key, T &data, bool if_no_arg, const std::string &help) :
        if_no_arg_(if_no_arg),
        key_(key),
        short_key_(short_key),
        help_(help)
    {
        data_ = std::make_unique<ParserType<T>>(data);
        default_data_ = std::make_unique<ParserType<T>>(data, true);
    }

    template <typename T> Entry(const std::string &key, const std::string &short_key, T &data, const T &default_data, bool if_no_arg, const std::string &help) :
        if_no_arg_(if_no_arg),
        key_(key),
        short_key_(short_key),
        help_(help)
    {
        data_ = std::make_unique<ParserType<T>>(data);
        default_data_ = std::make_unique<ParserType<T>>(default_data, true);

        *(dynamic_cast<ParserType<T>*>(data_.get())->GetP()) = default_data;
    }

    void Convert(const std::string &value) { data_->Convert(value); }

    template <typename T> T Get() const { return dynamic_cast<ParserType<T>*>(data_.get())->Get(); }
    template <typename T> T GetDefault() const { return dynamic_cast<ParserType<T>*>(default_data_.get())->Get(); }

    bool IfNoArg() const {return if_no_arg_;}
    void InCommandLine(bool if_set_from_arg) {if_in_command_line_ = if_set_from_arg;}
    bool IfInCommandLine() const {return if_in_command_line_;}

    std::string GetKey() const {return key_;}
    std::string GetShortKey() const {return short_key_;}
    std::string GetHelp() const {return help_;}

private:
    bool if_in_command_line_{false};

    const bool if_no_arg_;
    const std::string key_;
    const std::string short_key_;
    const std::string help_;
    std::unique_ptr<ParserBaseType> data_;
    std::unique_ptr<ParserBaseType> default_data_;
};

class Parser
{
public:
    template <typename T> void Add(const std::string &keys_str, T &t, const std::string &help)
    {
        auto [key, short_key] = GetKeyAndShort(keys_str);
        if( !params_.insert({key, std::make_shared<Entry>(key, short_key, t, false, help)}).second )
            std::cerr << "[WARNING] ==> Key \033[31m" << key << "\033[0m already exists" << std::endl;
    }

    template <typename T1, typename T2> void Add(const std::string &keys_str, T1 &t, const T2 &default_t2, const std::string &help)
    {
        if(!std::is_convertible<T2, T1>::value)
        {
            std::cerr << "[WARNING] ==> \033[31mDefault value type\033[0m not compatible" << std::endl;
            return;
        }

        T1 default_t1(default_t2);

        auto [key, short_key] = GetKeyAndShort(keys_str);
        if( !params_.insert({key, std::make_shared<Entry>(key, short_key, t, default_t1, false, help)}).second )
            std::cerr << "[WARNING] ==> Key \033[31m" << key << "\033[0m already exists" << std::endl;
    }


    void AddFlag(const std::string &keys_str, bool &t, bool default_t, const std::string &help)
    {
        auto [key, short_key] = GetKeyAndShort(keys_str);
        if( !params_.insert({key, std::make_shared<Entry>(key, short_key, t, default_t, true, help)}).second )
            std::cerr << "[WARNING] ==> Key \033[31m" << key << "\033[0m already exists" << std::endl;
    }

    template <typename T> T Get(const std::string &key) const
    {
        if( !params_.count(key) )
        {
            std::cerr << "[WARNING] ==> Key \033[31m" << key << "\033[0m does not exist" << std::endl;
            return T();
        }

        return params_.at(key)->Get<T>();
    }

    template <typename T> T GetDefault(const std::string &key) const
    {
        if( !params_.count(key) )
        {
            std::cerr << "[WARNING] ==> Key \033[31m" << key << "\033[0m does not exist" << std::endl;
            return T();
        }

        return params_.at(key)->GetDefault<T>();
    }

    bool IfInCommandLine(const std::string &key) const
    {
        if( !params_.count(key) )
        {
            std::cerr << "[WARNING] ==> Key \033[31m" << key << "\033[0m does not exist" << std::endl;
            return false;
        }

        return params_.at(key)->IfInCommandLine();
    }

    void Parse(int argc, char* argv[])
    {
        for(int i = 1; i < argc; i++)
        {
            std::string input_token(argv[i]);
            input_token = arg_parser_helper::StrTolower(input_token);
            if( input_token == "?" || input_token == "-h" || input_token == "--help" )
            {
                this->Help();
                exit(0);
            }
        }

        BuildTokenIndex();

        for(int i = 1; i < argc; i++)
        {
            std::string input_token(argv[i]);
            input_token = arg_parser_helper::StrTolower(input_token);

            auto it = token_index_.find(input_token);
            if(it != token_index_.end())
            {
                auto param = it->second;
                if(param->IfNoArg()) param->Convert(std::to_string(!param->GetDefault<bool>()));
                else
                {
                    if(i + 1 >= argc)
                    {
                        std::cerr << "[Error] ==> Key \033[31m" << input_token << "\033[0m expects a value" << std::endl;
                        exit(-1);
                    }
                    param->Convert(argv[i + 1]);
                }
                param->InCommandLine(true);
            }
            else
            {
#if __cplusplus >= 202002L
                if(input_token.starts_with("--"))
#else
                if(input_token.rfind("--", 0) == 0)
#endif
                    std::cerr << "[WARNING] ==> Unkown key \033[31m" << input_token.substr(2) << "\033[0m" << std::endl;
            }
        }
    }

    void Help()
    {
        auto GetMaxShift = [&]() -> int 
                           {
                               int max_shift = 0;
                               for(const auto &[key, param] : params_)
                               {
                                   int shift = param->GetKey().size() + param->GetShortKey().size();
                                   if( shift > max_shift ) max_shift = shift;
                               }

                               return max_shift > 8 ? max_shift + 9 : 17;
                           };

        auto GetKeyTokenStr = [&](const std::string &key, const std::string &short_key, const std::string &split) -> std::string
                              {
                                  return "-" + ( key.size() == 1 ? key : "-" + key ) +
                                         ( short_key.size() && key.size() != 1 ? split + "-" + short_key : "");
                              };

        auto max_shift = GetMaxShift();

        std::cout << "Usage:\t";
        for(const auto &[key, param] : params_)
        {
            std::cout << "\033[36m" << GetKeyTokenStr(param->GetKey(), param->GetShortKey(), "/") << "\033[0m"
                      << ( param->IfNoArg() ? "  " : "  <argument>  ");
        }
        std::cout << "\033[36m?/-h/--help\033[0m" << std::endl;
                                 
        for(const auto &[key, param] : params_)
        {
            std::cout << "\033[36m" << std::setw(max_shift) << GetKeyTokenStr(param->GetKey(), param->GetShortKey(), ", ") << "\033[0m\t:\t"
                      << param->GetHelp() << std::endl;
        }
        std::cout << "\033[36m" << std::setw(max_shift) << "?, --help, -h" << "\033[0m\t:\tprint this information" << std::endl;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Entry>> params_;
    std::unordered_map<std::string, Entry*> token_index_;
    bool token_index_built_ = false;

    void BuildTokenIndex()
    {
        if(token_index_built_) return;
        for(const auto &[key, param] : params_)
        {
            token_index_[arg_parser_helper::StrTolower("--" + param->GetKey())] = param.get();
            if(!param->GetShortKey().empty())
                token_index_[arg_parser_helper::StrTolower("-" + param->GetShortKey())] = param.get();
        }
        token_index_built_ = true;
    }

    std::tuple<std::string, std::string> GetKeyAndShort(const std::string &keys_str)
    {
        auto keys = arg_parser_helper::Split(keys_str);
        if( keys.size() != 1 && keys.size() != 2 )
        {
            std::cerr << "[Error] ==> Failed to resolve keys from \033[31m" << keys_str << "\033[0m" << std::endl;
            exit(-1);
        }

        std::string key;
        std::string short_key;

        if( keys.size() == 2 &&
            ( ( keys.at(0).size() == 1 && keys.at(1).size() == 1 ) ||
              ( keys.at(0).size() >  1 && keys.at(1).size() >  1 )
            )
          )
        {
            std::cerr << "[Error] ==> Keys \033[31m" << keys.at(0) << "\033[0m and \033[31m" << keys.at(1) << "\033[0m are both short keys or long keys" << std::endl;
            exit(-1);
        }

        if( keys.size() == 1 )
        {
            if(keys.at(0).size() == 1) short_key = keys.at(0);
            else                             key = keys.at(0);
        }

        if( keys.size() == 2 )
        {
            if(keys.at(0).size() == 1) { short_key = keys.at(0);       key = keys.at(1); }
            else                       {       key = keys.at(0); short_key = keys.at(1); }
        }

        if( !key.size() ) key = short_key;

        if( arg_parser_helper::StrTolower(key) == "help" || arg_parser_helper::StrTolower(short_key) == "h" )
        {
            std::cerr << "[Error] ==> No rational person would use \033[31mhelp\033[0m or \033[31mh\033[0m as user-defined key" << std::endl;
            exit(-1);
        }

        return std::make_tuple(key, short_key);
    }
};
