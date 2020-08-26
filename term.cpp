#include "term.hpp"

Term::Term(double const &value) : value(value)
{
}

double Term::Value() const
{
    return value;
}

Variable::Variable(double const &value) : Term(value)
{
}

Term &Variable::operator=(double const &value)
{
    this->value = value;

    return *this;
}

Constant::Constant(double const &value) : Term(value)
{
}
