#include "pch.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "material.h"
#include "point_light.h"
#include "sphere.h"
#include "spot_light.h"

#define _USE_MATH_DEFINES
#include "Rectangle.h"
#include "directional_light.h"
#include "math.h"
#include "plane.h"
#include "triangle.h"
#include "rectangular_prism.h"

using namespace std;

Parser::Parser() {}

Parser::~Parser() {}

double ToRadians(double angleDegrees) {
    return (angleDegrees / double(180)) * M_PI;
}

Renderer* Parser::Parse(const std::string& filename) {
    Scene* scene = new Scene();
    Renderer* renderer = new Renderer();
    vector<Vector3> vertices;
    vector<Vector3> normals;
    int max_vertices = -1;
    int max_normals = -1;
    Primitive* just_initialized_primitive = nullptr;
    Positionable* last_initialized_positionable = nullptr;

    ifstream file(filename);
    if (file.fail()) {
        cout << "Failed to open file \"" << filename << "\". Exiting." << endl;
    }

    string line, command;
    vector<string> tokens;
    vector<double> params;
    Material last_material;
    while (getline(file, line)) {
        if (line.length() == 0 || line.at(0) == '#') continue;

        tokens = Split(line, ' ');
        command = tokens[0];
        command = command.substr(command.find_first_not_of(' '), command.length());  // Remove leading whitespace from command
        if (command == "") {
            cout << "Invalid command while processing scene file." << endl;
            exit(-1);
        } else if (command == "output_image") {
            if (tokens.size() != 2) {
                printf("Invalid number of parameters for command \"output\".");
                exit(-1);
            }

            renderer->SetOutputFilename(tokens[1]);
            continue;
        }

        tokens.erase(tokens.begin());
        params = StringsToDoubles(tokens);

        if (expected_num_params.count(command) != 1) {
            printf("Unrecognized command %s\n", command.c_str());
            break;
        }

        VerifyCorrectNumberParameters(command, params, expected_num_params[command]);
        if (command == "sphere") {
            Vector3 position = Vector3(params[0], params[1], params[2]);
            double radius = params[3];

            Sphere* sphere = new Sphere(position, radius);
            just_initialized_primitive = sphere;
            last_initialized_positionable = sphere;
        } else if (command == "material") {
            Color ambient = GetColor(params, 0);
            Color diffuse = GetColor(params, 3);
            Color specular = GetColor(params, 6);
            int phong_factor = int(params[9]);
            Color transmissive = GetColor(params, 10);
            double index_of_refraction = params[13];

            last_material = Material(ambient, diffuse, specular, transmissive, phong_factor, index_of_refraction);
        } else if (command == "ambient_light") {
            AmbientLight ambient_light = AmbientLight(GetColor(params, 0));
            scene->SetAmbientLight(ambient_light);
        } else if (command == "background") {
            scene->SetBackgroundColor(GetColor(params, 0));
        } else if (command == "camera") {
            Vector3 position = GetVector3(params, 0);
            Vector3 direction = GetVector3(params, 3);
            Vector3 up = GetVector3(params, 6);
            double height_angle = ToRadians(params[9]);

            Camera* camera = new Camera(position, direction, up, height_angle);
            last_initialized_positionable = camera;
            scene->SetCamera(camera);
        } else if (command == "film_resolution") {
            renderer->SetRenderDimensions(int(params[0]), int(params[1]));
        } else if (command == "point_light") {
            Color color = GetColor(params, 0);
            Vector3 position = GetVector3(params, 3);

            PointLight* point_light = new PointLight(color, position);
            scene->AddLight(point_light);

            last_initialized_positionable = point_light;
        } else if (command == "spot_light") {
            Color color = GetColor(params, 0);
            Vector3 position = GetVector3(params, 3);
            Vector3 direction = GetVector3(params, 6);
            double angle1 = ToRadians(params[9]);
            double angle2 = ToRadians(params[10]);

            SpotLight* spot_light = new SpotLight(color, position, direction, angle1, angle2);
            scene->AddLight(spot_light);

            last_initialized_positionable = spot_light;
        } else if (command == "directional_light") {
            Color color = GetColor(params, 0);
            Vector3 direction = GetVector3(params, 3);

            DirectionalLight* directional_light = new DirectionalLight(color, direction);
            scene->AddLight(directional_light);
        } else if (command == "max_depth") {
            renderer->SetRecursiveDepth(int(params[0]));
        } else if (command == "max_vertices") {
            max_vertices = int(params[0]);
        } else if (command == "max_normals") {
            max_normals = int(params[0]);
        } else if (command == "vertex") {
            vertices.push_back(GetVector3(params, 0));
        } else if (command == "normal") {
            normals.push_back(GetVector3(params, 0));
        } else if (command == "triangle") {
            just_initialized_primitive = new Triangle(vertices[int(params[0])], vertices[int(params[1])], vertices[int(params[2])]);
        } else if (command == "normal_triangle") {
            just_initialized_primitive = new Triangle(vertices[int(params[0])], vertices[int(params[1])], vertices[int(params[2])],
                                                      normals[int(params[3])], normals[int(params[4])], normals[int(params[5])]);
        } else if (command == "plane") {
            Vector3 point = GetVector3(params, 0);
            Vector3 normal = GetVector3(params, 3);

            just_initialized_primitive = new Plane(normal, point);
        } else if (command == "quad") {
            Vector3 point = GetVector3(params, 0);
            Vector3 right = GetVector3(params, 3);
            Vector3 up = GetVector3(params, 6);

            just_initialized_primitive = new Rectangle(point, right, up);
        } else if (command == "rectangular_prism") {
            Vector3 point = GetVector3(params, 0);
            Vector3 right = GetVector3(params, 3);
            Vector3 up = GetVector3(params, 6);
            Vector3 forward = GetVector3(params, 9);

            just_initialized_primitive = new RectangularPrism(point, right, up, forward);
        } else if (command == "end_position") {
            if (last_initialized_positionable != nullptr) {
                Vector3 end_position = GetVector3(params, 0);
                last_initialized_positionable->SetEndPosition(end_position);
                last_initialized_positionable = nullptr;
            } else {
                printf("Cannot set end_position because a positionable has not been initialized since the last call to end_position.\n");
            }
        } else {
            printf("The command \"%s\" was in the num params map but is still unknown\n", command.c_str());
        }

        if (just_initialized_primitive != nullptr) {
            scene->AddPrimitive(just_initialized_primitive);
            just_initialized_primitive->SetMaterial(last_material);
            just_initialized_primitive = nullptr;
        }
    }

    renderer->SetScene(scene);
    return renderer;
}

vector<string> Parser::Split(const string& str, const char delimiter) {
    vector<string> tokens;
    std::istringstream string_stream(str);
    string token;
    while (getline(string_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<double> Parser::StringsToDoubles(const std::vector<std::string>& string_vector) {
    vector<double> vec;
    for (const string str : string_vector) {
        try {
            vec.push_back(stod(str));
        } catch (std::exception&) {
        }
    }

    return vec;
}

void Parser::VerifyCorrectNumberParameters(const string& command, const vector<double>& parameters, const int num_parameters) {
    if (parameters.size() != num_parameters) {
        cout << "Incorrect number of parameters for command \"" << command << "\".\n";
        exit(-1);
    }
}

Vector3 Parser::GetVector3(const std::vector<double>& params, int startIndex) {
    return Vector3(params[startIndex], params[startIndex + 1], params[startIndex + 2]);
}

Color Parser::GetColor(const std::vector<double>& params, int startIndex) {
    return Color(params[startIndex], params[startIndex + 1], params[startIndex + 2]);
}

map<string, int> Parser::expected_num_params = {
    {"camera", 10},           {"film_resolution", 2}, {"max_vertices", 1},  {"max_normals", 1}, {"vertex", 3},    {"normal", 3},
    {"triangle", 3},          {"normal_triangle", 6}, {"sphere", 4},        {"background", 3},  {"material", 14}, {"directional_light", 6},
    {"point_light", 6},       {"spot_light", 11},     {"ambient_light", 3}, {"max_depth", 1},   {"plane", 6},     {"quad", 9},
    {"rectangular_prism", 12}, {"end_position", 3}};
