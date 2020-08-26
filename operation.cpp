#include "operation.hpp"

Affirmation::Affirmation(std::shared_ptr<Term> const &term) : term(term)
{
}

double Affirmation::Value() const
{
    return +1.0 * term->Value();
}

Negation::Negation(std::shared_ptr<Term> const &term) : term(term)
{
}

double Negation::Value() const
{
    return -1.0 * term->Value();
}

Exponentiation::Exponentiation(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs) : lhs(lhs), rhs(rhs)
{
}

double Exponentiation::Value() const
{
    return std::pow(lhs->Value(), rhs->Value());
}

Multiplication::Multiplication(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs) : lhs(lhs), rhs(rhs)
{
}

double Multiplication::Value() const
{
    return lhs->Value() * rhs->Value();
}

Division::Division(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs) : lhs(lhs), rhs(rhs)
{
}

double Division::Value() const
{
    return lhs->Value() / rhs->Value();
}

Addition::Addition(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs) : lhs(lhs), rhs(rhs)
{
}

double Addition::Value() const
{
    return lhs->Value() + rhs->Value();
}

Subtraction::Subtraction(std::shared_ptr<Term> const &lhs, std::shared_ptr<Term> const &rhs) : lhs(lhs), rhs(rhs)
{
}

double Subtraction::Value() const
{
    return lhs->Value() - rhs->Value();
}
