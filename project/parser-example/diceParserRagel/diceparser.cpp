#include "diceparser.h"
#include <algorithm>
#include <random>

namespace DP {
std::int64_t RollFormula::evaluate() const
{
  if (formula.index() == 0) {
    RollSequence RS = std::get<RollSequence>(formula);
    for (std::size_t i = 0; i < RS.size(); i++) {
      if (RS[i].formula.index() == 2) {
        if (std::get<RollOperation>(RS[i].formula) == RollOperation::recast) {
          modification = { std::get<RollOperation>(RS[i].formula), 0 };
          RS.erase(RS.begin() + i, RS.begin() + i + 1);
          i--;
        } else if (std::get<RollOperation>(RS[i].formula) == RollOperation::MZ) {
          if (RS.size() < 2) throw RollFormulaError("[ERROR w/ modifier] not enough arguments");
          if (RS[i - 1].formula.index() != 1)
            throw RollFormulaError("[ERROR w/ modifier] argument is not int");
          modification = { std::get<RollOperation>(RS[i].formula),
            std::get<std::int64_t>(RS[i - 1].formula) };
          RS.erase(RS.begin() + i - 1, RS.begin() + i + 1);
          i -= 2;
        } else if (std::get<RollOperation>(RS[i].formula) == RollOperation::mZ) {
          if (RS.size() < 2) throw RollFormulaError("[ERROR w/ modifier] not enough arguments");
          if (RS[i - 1].formula.index() != 1)
            throw RollFormulaError("[ERROR w/ modifier] argument is not int");
          modification = { std::get<RollOperation>(RS[i].formula),
            std::get<std::int64_t>(RS[i - 1].formula) };
          RS.erase(RS.begin() + i - 1, RS.begin() + i + 1);
          i -= 2;
        } else {
          if (RS.size() < 3) throw RollFormulaError("[ERROR w/ operation] not enough arguments");
          try {
            RS[i - 2].formula = (std::int64_t)RS[i](RS[i - 2], RS[i - 1]);
          } catch (RollFormulaError &e) {
            throw e;
          }
          RS.erase(RS.begin() + i - 1, RS.begin() + i + 1);
          i -= 2;
        }
      }
    }
    if (RS.size() > 1) throw RollFormulaError("[ERROR w/ formula] left > 1 elements after running");
    return std::get<std::int64_t>(RS[0].formula);
  } else if (formula.index() == 1)
    return std::get<1>(formula);
  else {
    throw RollFormulaError("[ERROR w/ evaluate] trying to evaluate single operation");
  }
}
std::int64_t RollFormula::operator()(const RollFormula &l, const RollFormula &r) const
{
  if (l.formula.index() == 2 || r.formula.index() == 2)
    throw RollFormulaError("[ERROR w/ operand] left or right operand is an operation");
  switch (std::get<RollOperation>(formula)) {
  case RollOperation::multiplication:
    return l.evaluate() * r.evaluate();
  case RollOperation::division:
    return l.evaluate() / r.evaluate();
  case RollOperation::modul:
    return l.evaluate() % r.evaluate();
  case RollOperation::plus:
    return l.evaluate() + r.evaluate();
  case RollOperation::minus:
    return l.evaluate() - r.evaluate();
  case RollOperation::cast:
    static std::random_device                          random;
    static std::mt19937_64                             randEngine(random());
    static std::uniform_int_distribution<std::int64_t> gen(1, r.evaluate());
    std::vector<std::int64_t>                          rolls;
    std::int64_t                                       countOfRoll = l.evaluate();
    std::int64_t                                       resultOfRoll;
    for (std::size_t i = 0; i < countOfRoll; i++) {
      resultOfRoll = gen(randEngine);
      if (modification.operation == RollOperation::recast && resultOfRoll == r.evaluate())
        countOfRoll++;
      rolls.insert(
        std::find_if(
          rolls.begin(), rolls.end(), [resultOfRoll](auto val) { return resultOfRoll <= val; }),
        resultOfRoll);
    }
    if (modification.operation == RollOperation::mZ) {
      for (int i = 0; i < rolls.size() && i < modification.value; i++) { resultOfRoll += rolls[i]; }
    } else if (modification.operation == RollOperation::MZ) {
      for (int i = 1; i < rolls.size() && i <= modification.value; i++) {
        resultOfRoll += rolls[rolls.size() - i];
      }
    } else {
      for (int i = 0; i < rolls.size(); i++) { resultOfRoll += rolls[i]; }
    }
    modification.operation = RollOperation::woMod;
    return resultOfRoll;
  }
  return 0;
}
}// namespace DP
