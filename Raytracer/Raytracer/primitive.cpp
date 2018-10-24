#include "pch.h"
#include "primitive.h"

Primitive::Primitive() : Primitive(Material()) {}

Primitive::Primitive(const Material& material) : Materialed(material) {}

Primitive::~Primitive() = default;
