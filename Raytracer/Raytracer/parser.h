#include "scene.h"
#include "renderer.h"

class Parser {
   public:
    Parser();
    ~Parser();

    Renderer* Parse(const std::string &filename);

   private:
    std::vector<std::string> Split(const std::string &str, char delimiter);
    std::vector<double> StringsToDoubles(const std::vector<std::string>& string_vector);
    void VerifyCorrectNumberParameters(const std::string &command, const std::vector<double> &parameters, const int num_parameters);
};
