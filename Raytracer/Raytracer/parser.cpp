#include "pch.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include "material.h"
#include "point_light.h"
#include "sphere.h"
#include "spot_light.h"

#define _USE_MATH_DEFINES
#include "math.h"
#include "directional_light.h"

using namespace std;

Parser::Parser() {}

Parser::~Parser() {}

double ToRadians(double angleDegrees) {
    return (angleDegrees / double(180)) * M_PI;
}

Renderer* Parser::Parse(const std::string& filename) {
    Scene* scene = new Scene();
    Renderer* renderer = new Renderer();

    ifstream file(filename);
    if (file.fail()) {
        cout << "Failed to open file \"" << filename << "\". Exiting." << endl;
    }

    string line, command;
    vector<string> tokens;
    vector<double> params;
    Material lastMaterial;
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

        if (command == "sphere") {
            VerifyCorrectNumberParameters(command, params, 4);
            Vector3 position = Vector3(params[0], params[1], params[2]);
            double radius = params[3];

            Sphere* sphere = new Sphere(position, lastMaterial, radius);

            scene->AddPrimitive(sphere);
        } else if (command == "material") {
            VerifyCorrectNumberParameters(command, params, 14);
            Color ambient = Color(params[0], params[1], params[2]);
            Color diffuse = Color(params[3], params[4], params[5]);
            Color specular = Color(params[6], params[7], params[8]);
            int phong_factor = int(params[9]);
            Color transmissive = Color(params[10], params[11], params[12]);
            double index_of_refraction = params[13];

            lastMaterial = Material(ambient, diffuse, specular, transmissive, phong_factor, index_of_refraction);
        } else if (command == "ambient_light") {
            VerifyCorrectNumberParameters(command, params, 3);
            AmbientLight ambient_light = AmbientLight(Color(params[0], params[1], params[2]));
            scene->SetAmbientLight(ambient_light);
        } else if (command == "background") {
            VerifyCorrectNumberParameters(command, params, 3);
            scene->SetBackgroundColor(Color(params[0], params[1], params[2]));
        } else if (command == "camera") {
            VerifyCorrectNumberParameters(command, params, 10);
            Vector3 position = Vector3(params[0], params[1], params[2]);
            Vector3 direction = Vector3(params[3], params[4], params[5]);
            Vector3 up = Vector3(params[6], params[7], params[8]);
            double height_angle = ToRadians(params[9]);

            Camera camera = Camera(position, direction, up, height_angle);
            scene->SetCamera(camera);
        } else if (command == "film_resolution") {
            VerifyCorrectNumberParameters(command, params, 2);
            renderer->SetRenderDimensions(int(params[0]), int(params[1]));
        } else if (command == "point_light") {
            VerifyCorrectNumberParameters(command, params, 6);
            Color color(params[0], params[1], params[2]);
            Vector3 position(params[3], params[4], params[5]);

            PointLight* point_light = new PointLight(color, position);
            scene->AddLight(point_light);
        } else if (command == "spot_light") {
            VerifyCorrectNumberParameters(command, params, 11);
            Color color(params[0], params[1], params[2]);
            Vector3 position(params[3], params[4], params[5]);  // Should maybe do something about this
                                                                // duplication with point_light (static parse function?)
            Vector3 direction(params[6], params[7], params[8]);
            double angle1 = ToRadians(params[9]);
            double angle2 = ToRadians(params[10]);

            SpotLight* spot_light = new SpotLight(color, position, direction, angle1, angle2);
            scene->AddLight(spot_light);
        } else if (command == "directional_light") {
            VerifyCorrectNumberParameters(command, params, 6);
            Color color(params[0], params[1], params[2]);
            Vector3 direction(params[3], params[4], params[5]);

            DirectionalLight* directional_light = new DirectionalLight(color, direction);
            scene->AddLight(directional_light);
        } else if (command == "max_depth") {
            VerifyCorrectNumberParameters(command, params, 1);
            renderer->SetRecursiveDepth(params[0]);
        } else {
            printf("Unrecognized command %s\n", command.c_str());
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
        } catch (std::exception& e) {
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
