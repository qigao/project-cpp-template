#pragma once

#include "address.hpp"

#include <memory>
#include <string>
#include <vector>

class Node;

class Formula
{
public:
    typedef std::vector<std::string> Arguments;

    typedef std::string (*EvalAddressCallback)(Address const&, void* pData);
    typedef std::string (*EvalFunctionCallback)(std::string const& name,
                                                Arguments const&, void* pData);

    Formula(std::string const&);

    std::string evaluate(EvalAddressCallback, EvalFunctionCallback,
                         void* pData);

    operator std::string() const;

private:
    std::shared_ptr<Node> m_pRoot;
};
