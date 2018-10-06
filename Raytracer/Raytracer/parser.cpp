#include "pch.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include "material.h"

using namespace std;

Parser::Parser() {}

Parser::~Parser() {}

Scene Parser::Parse(const std::string& filename) {
    Scene scene = Scene();
    ifstream file(filename);
    if (file.fail()) {
        cout << "Failed to open file \"" << filename << "\". Exiting." << endl;
    }

    string line, command;
    vector<string> tokens;
    vector<double> params;
    Material lastMaterial;
    while (getline(file, line)) {
        if (line.at(0) == '#') continue;

        tokens = Split(line, ' ');
        command = tokens[0];
        command = command.substr(command.find_first_not_of(' '), command.length());  // Remove leading whitespace from command
        if (command == "") {
            cout << "Invalid command while processing scene file." << endl;
            exit(-1);
        }
        tokens.erase(tokens.begin());
        params = StringsToDoubles(tokens);

        if (command == "sphere") {
            VerifyCorrectNumberParameters(command, params, 4);
            Vector3 position = Vector3(params[0], params[1], params[2]);
            double radius = params[3];

            Sphere *sphere = new Sphere(position, lastMaterial, radius);

            scene.AddPrimitive(sphere);
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
            scene.SetAmbientLight(Color(params[0], params[1], params[2]));
        } else if (command == "background") {
            VerifyCorrectNumberParameters(command, params, 3);
            scene.SetBackground(Color(params[0], params[1], params[2]));
        }
    }
    return scene;
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
        vec.push_back(stod(str));
    }

    return vec;
}

void Parser::VerifyCorrectNumberParameters(const string &command, const vector<double>& parameters, const int num_parameters) {
    if (parameters.size() != num_parameters) {
        cout << "Incorrect number of parameters for command \"" << command << "\".\n";
        exit(-1);
    }
}
