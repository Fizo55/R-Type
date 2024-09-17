#ifndef ENGINE_EXCEPTIONS_HPP_
    #define ENGINE_EXCEPTIONS_HPP_

    #include <iostream>

    namespace engine {
        class engineError : public std::exception {
            public:
                engineError(const std::string &what_message) : _what_message(what_message) {};
                ~engineError() = default;

                const char* what() const noexcept override
                {
                    return (_what_message.c_str());
                }

            private:
                std::string _what_message;
        };

        class outOfMemoryError : public engineError {
            public:
                outOfMemoryError(const std::string &what_message) : engineError(what_message) {};
        };

        class nullPtrError : public engineError {
            public:
                nullPtrError(const std::string &what_message) : engineError(what_message) {};
        };

        class loadError : public engineError {
            public:
                loadError(const std::string &what_message) : engineError(what_message) {};
        };
    };

#endif /* !ENGINE_EXCEPTIONS_HPP_ */
