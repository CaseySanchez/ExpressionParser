#pragma once

#include <memory>
#include <cmath>

#include "term.hpp"

class Cos : public Term
{
    std::shared_ptr<Term> term;

public:
    Cos(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Sin : public Term
{
    std::shared_ptr<Term> term;

public:
    Sin(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Tan : public Term
{
    std::shared_ptr<Term> term;

public:
    Tan(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Acos : public Term
{
    std::shared_ptr<Term> term;

public:
    Acos(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Asin : public Term
{
    std::shared_ptr<Term> term;

public:
    Asin(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Atan : public Term
{
    std::shared_ptr<Term> term;

public:
    Atan(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Sqrt : public Term
{
    std::shared_ptr<Term> term;

public:
    Sqrt(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Abs : public Term
{
    std::shared_ptr<Term> term;

public:
    Abs(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Exp : public Term
{
    std::shared_ptr<Term> term;

public:
    Exp(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Log : public Term
{
    std::shared_ptr<Term> term;

public:
    Log(std::shared_ptr<Term> const &term);

    double Value() const override;
};
