#include "function.hpp"

Cos::Cos(std::shared_ptr<Term> const &term) : term(term)
{
}

double Cos::Value() const
{
    return std::cos(term->Value());
}

Sin::Sin(std::shared_ptr<Term> const &term) : term(term)
{
}

double Sin::Value() const
{
    return std::sin(term->Value());
}

Tan::Tan(std::shared_ptr<Term> const &term) : term(term)
{
}

double Tan::Value() const
{
    return std::tan(term->Value());
}

Acos::Acos(std::shared_ptr<Term> const &term) : term(term)
{
}

double Acos::Value() const
{
    return std::acos(term->Value());
}

Asin::Asin(std::shared_ptr<Term> const &term) : term(term)
{
}

double Asin::Value() const
{
    return std::asin(term->Value());
}

Atan::Atan(std::shared_ptr<Term> const &term) : term(term)
{
}

double Atan::Value() const
{
    return std::atan(term->Value());
}

Sqrt::Sqrt(std::shared_ptr<Term> const &term) : term(term)
{
}

double Sqrt::Value() const
{
    return std::sqrt(term->Value());
}

Abs::Abs(std::shared_ptr<Term> const &term) : term(term)
{
}

double Abs::Value() const
{
    return std::abs(term->Value());
}

Exp::Exp(std::shared_ptr<Term> const &term) : term(term)
{
}

double Exp::Value() const
{
    return std::exp(term->Value());
}

Log::Log(std::shared_ptr<Term> const &term) : term(term)
{
}

double Log::Value() const
{
    return std::log(term->Value());
}
