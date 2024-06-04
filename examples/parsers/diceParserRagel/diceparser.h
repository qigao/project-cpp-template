#ifndef DICEPARSER_H
#define DICEPARSER_H

#include <cstdint>
#include <iostream>
#include <stack>
#include <string>
#include <variant>
#include <vector>

namespace DP {
using RollFormulaError = std::runtime_error;

class RollFormula;

using RollSequence = std::vector<RollFormula>;

enum RollOperation : const char {
  multiplication = '*',
  division       = '/',
  modul          = '%',
  plus           = '+',
  minus          = '-',
  cast           = 'd',
  recast         = '!',
  MZ             = 'M',
  mZ             = 'm',
  woMod          = 's'
};

class RollFormula
{
public:
  struct FormulaModification
  {
    RollOperation operation;
    std::int64_t  value;
  };

  RollFormula() = default;

  template<typename T,
    typename = std::enable_if_t<
      std::is_constructible_v<std::variant<RollSequence, std::int64_t, RollOperation>, T>>>
  RollFormula(T _formula) : formula{ std::move(_formula) }
  {}

  std::int64_t evaluate() const;

  void addEnd(const RollFormula &element)
  {
    if (formula.index() == 0) {
      std::get<RollSequence>(formula).push_back(element);
    } else {
      RollSequence sequence;
      sequence.push_back(formula);
      formula = sequence;
    }
  }

  friend std::istream &operator>>(std::istream &in, RollFormula &rf);
  friend std::ostream &operator<<(std::ostream &out, const RollFormula &rf)
  {
    if (rf.formula.index() == 0) {
      RollSequence seq = std::get<0>(rf.formula);
      for (RollFormula f : seq) out << f << ' ';
    } else if (rf.formula.index() == 1) {
      out << std::get<1>(rf.formula);
    } else {
      out << (char)std::get<2>(rf.formula);
    }
    return out;
  }

private:
  std::variant<RollSequence, std::int64_t, RollOperation> formula;

  std::int64_t operator()(const RollFormula &left, const RollFormula &right) const;

  inline static FormulaModification modification = { woMod, 0 };
};
}// namespace DP

#endif// DICEPARSER_H
