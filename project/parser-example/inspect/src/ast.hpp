#pragma once

#include <string>
#include <vector>

#include "address.hpp"
#include "binary_op.h"

typedef std::vector<std::string> Arguments;

typedef std::string (*EvalAddressCallback)(Address const&, void* pData);
typedef std::string (*EvalFunctionCallback)(std::string const& name,
                                            Arguments const&, void* pData);

class Node
{
public:
    virtual ~Node(){};
    virtual std::string evaluate(EvalAddressCallback, EvalFunctionCallback,
                                 void* pData) const = 0;
    virtual operator std::string() const = 0;
};

class LitDoubleNode : public Node
{
public:
    LitDoubleNode(double value);
    virtual std::string evaluate(EvalAddressCallback, EvalFunctionCallback,
                                 void* pData) const;
    virtual operator std::string() const;

private:
    double m_value;
};

class LitStringNode : public Node
{
public:
    LitStringNode(std::string const& value);
    virtual std::string evaluate(EvalAddressCallback, EvalFunctionCallback,
                                 void* pData) const;
    virtual operator std::string() const;

private:
    std::string m_value;
};

class BinaryOpNode : public Node
{
public:
    BinaryOpNode(BinaryOp binaryOp, Node const* pLeft, Node const* pRight);
    virtual ~BinaryOpNode();
    virtual std::string evaluate(EvalAddressCallback, EvalFunctionCallback,
                                 void* pData) const;
    virtual operator std::string() const;

private:
    BinaryOp m_binaryOp;
    Node const* m_pLeft;
    Node const* m_pRight;
};

class VarIdentifierNode : public Node
{
public:
    VarIdentifierNode(std::string const& name);
    std::string const& getName() const;
    virtual std::string evaluate(EvalAddressCallback, EvalFunctionCallback,
                                 void* pData) const;
    virtual operator std::string() const;

private:
    std::string m_name;
};

class VarAddressNode : public Node
{
public:
    VarAddressNode(Address const& address);
    Address const& getAddress() const;
    virtual std::string evaluate(EvalAddressCallback, EvalFunctionCallback,
                                 void* pData) const;
    virtual operator std::string() const;

private:
    Address m_address;
};

class FnCallNode : public Node
{
public:
    virtual ~FnCallNode();
    void setFnName(std::string const& fnName);
    void pushParam(Node const* pNode);
    virtual std::string evaluate(EvalAddressCallback, EvalFunctionCallback,
                                 void* pData) const;
    virtual operator std::string() const;

private:
    typedef std::vector<Node const*> Params;
    Params m_params;
    std::string m_fnName;
};
