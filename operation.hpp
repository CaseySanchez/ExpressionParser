#pragma once

#include <memory>
#include <cmath>

#include "term.hpp"

class Affirmation : public Term
{
    std::shared_ptr<Term> term;

public:
    Affirmation(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Negation : public Term
{
    std::shared_ptr<Term> term;

public:
    Negation(std::shared_ptr<Term> const &term);

    double Value() const override;
};

class Exponentiation : public Term
{
    std::shared_ptr<Term> lhs;
    std::shared_ptr<Term> rhs;

public:
    Exponentiation(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs);

    double Value() const override;
};

class Multiplication : public Term
{
    std::shared_ptr<Term> lhs;
    std::shared_ptr<Term> rhs;

public:
    Multiplication(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs);

    double Value() const override;
};

class Division : public Term
{
    std::shared_ptr<Term> lhs;
    std::shared_ptr<Term> rhs;

public:
    Division(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs);

    double Value() const override;
};

class Addition : public Term
{
    std::shared_ptr<Term> lhs;
    std::shared_ptr<Term> rhs;

public:
    Addition(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs);

    double Value() const override;
};

class Subtraction : public Term
{
    std::shared_ptr<Term> lhs;
    std::shared_ptr<Term> rhs;

public:
    Subtraction(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs);

    double Value() const override;
};
