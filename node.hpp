/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <string>

class Node
{
protected:
    double value;

    std::string type;

public:
    Node(double const &value = 0.0);

    std::string Type() const;

    virtual double Value() const;
};

class Variable : public Node
{
    std::string name;

public:
    Variable(double const &value = 0.0);

    std::string Name() const;

    Node &operator=(double const &value);
};

class Constant : public Node
{
public:
    Constant(double const &value);

    Node &operator=(Node const &node) = delete;
};
