#pragma once

class Term
{
protected:
    double value;

public:
    Term(double const &value = 0.0);

    virtual double Value() const;
};

class Variable : public Term
{
public:
    Variable(double const &value = 0.0);

    Term &operator=(double const &term);
};

class Constant : public Term
{
public:
    Constant(double const &value);

    Term &operator=(Term const &value) = delete;
};
