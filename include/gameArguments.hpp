#ifndef GAME_ARGUMENTS_HPP_
    #define GAME_ARGUMENTS_HPP_

    #include <vector>
    #include <iostream>
    #include <map>
    #include <any>
    #include <tuple>
    #include <functional>

    class argument {
        public:
            enum ARGUMENT_TYPE {
                SINGLE,
                VALUE,
                EQUAL,
                HELP
            };

            enum VALUE_TYPE {
                STRING,
                CHAR,
                SHORT,
                INT,
                LONG,
                FLOAT
            };

            argument(const std::string & = "--unknow", ARGUMENT_TYPE = SINGLE, VALUE_TYPE = STRING, std::any = std::any());
            ~argument() = default;

            std::string name;
            ARGUMENT_TYPE type;
            VALUE_TYPE valueType;
            std::any value;

        static std::any base_string_builder(const std::string &);
        static std::any base_char_builder(const std::string &);
        static std::any base_short_builder(const std::string &);
        static std::any base_int_builder(const std::string &);
        static std::any base_long_builder(const std::string &);
        static std::any base_float_builder(const std::string &);

        private:

    };

    class argumentTypeFactory {
        public:
            argumentTypeFactory();
            ~argumentTypeFactory() = default;

            void registerBuildRoute(argument::VALUE_TYPE, const std::function<std::any (const std::string &)> &);

            std::any build(argument::VALUE_TYPE, const std::string &) const;

        private:
            std::map<argument::VALUE_TYPE, std::function<std::any (const std::string &)>> _buildRoutes;
    };

    class argumentParser {
        public:
            argumentParser(std::size_t, std::size_t);
            ~argumentParser() = default;

            void attachFactory(const argumentTypeFactory &);

            void registerArg(const argument &);
            void registerUnamedArg(argument::VALUE_TYPE);

            void parseArgs(int, char **);

            const argument &getArg(const std::string &) const;
            bool hasArg(const std::string &) const;

            const std::vector<std::any> &getUnnamedArgs(void) const;

        private:
            std::vector<std::any> _args;
            std::vector<argument::VALUE_TYPE> _argsType;

            std::vector<argument> _lookingFor;

            std::size_t _minArgs;
            std::size_t _maxArgs;

            std::map<std::string, argument> _builts;

            argumentTypeFactory _factory;
    };

#endif /* GAME_ARGUMENTS_HPP_ */
