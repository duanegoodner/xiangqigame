#include <concepts>
#include <iostream>

template<typename T>
concept Logger = requires(T t, const std::string& message) {
    { t.log(message) } -> std::same_as<void>;
};



class ConsoleLogger {
public:
    void log(const std::string& message) {
        std::cout << "ConsoleLogger: " << message << std::endl;
    }
};

class FileLogger {
public:
    void log(const std::string& message) {
        // For demonstration, let's pretend this writes to a file.
        std::cout << "FileLogger: " << message << std::endl;
    }
};



template<Logger T>
class Application {
private:
    T& logger;

public:
    Application(T& logger) : logger(logger) {}

    void process() {
        logger.log("Starting application process...");
        // Further processing...
        logger.log("Ending application process...");
    }
};



int main() {
    ConsoleLogger consoleLogger;
    Application app1(consoleLogger);
    app1.process();

    FileLogger fileLogger;
    Application app2(fileLogger);
    app2.process();

    return 0;
}
