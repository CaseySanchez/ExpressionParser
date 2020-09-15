/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>

#include "node.hpp"

class Cos : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Cos(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Sin : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Sin(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Tan : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Tan(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Acos : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Acos(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Asin : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Asin(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Atan : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Atan(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Sqrt : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Sqrt(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Abs : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Abs(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Exp : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Exp(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Log : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Log(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};
