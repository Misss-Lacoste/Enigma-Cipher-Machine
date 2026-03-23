class RotorConfigurationError : public std::runtime_error {
public:
    explicit RotorConfigurationError(const std::string& message)
        : std::runtime_error(message) {}
};
//to be continued