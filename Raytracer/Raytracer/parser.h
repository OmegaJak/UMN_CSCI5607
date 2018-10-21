#include "scene.h"
#include "renderer.h"
#include <map>

class Parser {
   public:
    Parser();
    ~Parser();

    Renderer* Parse(const std::string &filename);

   private:
    std::vector<std::string> Split(const std::string &str, char delimiter);
    std::vector<double> StringsToDoubles(const std::vector<std::string>& string_vector);
    void VerifyCorrectNumberParameters(const std::string &command, const std::vector<double> &parameters, const int num_parameters);

    static Vector3 GetVector3(const std::vector<double> &params, int startIndex);
    static Color GetColor(const std::vector<double> &params, int startIndex);

    static std::map<std::string, int> expected_num_params;
};
