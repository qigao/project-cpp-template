#include "sheet.hpp"

#include "address.hpp"
#include "cell.hpp"
#include "formula.hpp"

#include <iostream>
#include <string>
#include <unordered_map>

namespace
{
struct SheetCallbackData
{
    Cells& cells;
    int phase;
};

void recalculateDepthFirst(int phase, Cells& cells, Cell& cell);

std::string evalAddressCallback(Address const& address, void* pData)
{
    SheetCallbackData* pCbData = static_cast<SheetCallbackData*>(pData);
    Cells::iterator itr = pCbData->cells.find(address);
    if (itr == pCbData->cells.end())
    {
        return "";
    }

    recalculateDepthFirst(pCbData->phase, pCbData->cells, itr->second);
    return itr->second.value;
}

std::string evalFunctionCallback(std::string const& name,
                                 Formula::Arguments const&, void* pData)
{
    SheetCallbackData* pCbData = static_cast<SheetCallbackData*>(pData);
    throw std::runtime_error("Function calls are not implemented.");
}

void recalculateDepthFirst(int phase, Cells& cells, Cell& cell)
{
    // Check if cell has been discovered in this recalculation phase
    if (cell.phase == phase)
    {
        // If it has been discovered, and has also been processed, we're done
        if (cell.processed)
        {
            // Forward edge (= already recalculated in this phase)
            return;
        }
        // Otherwise, it must be a back edge (= cycle)
        throw std::runtime_error("Cycle detected.");
    }

    SheetCallbackData cbData = {cells, phase};

    cell.phase = phase;
    cell.processed = false;

    // Evaluate the value of the cell, recursively recalculating the values
    // of other cells whose values it depends on.
    cell.value =
        Formula(cell.formula)
            .evaluate(evalAddressCallback, evalFunctionCallback, &cbData);

    cell.processed = true;
}
} // namespace

Sheet::Sheet() : m_pCells(new Cells()), m_phase(1) {}

Sheet::~Sheet() {}

bool Sheet::erase(Address const& address)
{
    return m_pCells->erase(address) == 1;
}

std::string Sheet::getFormula(Address const& address) const
{
    Cells::const_iterator itr = m_pCells->find(address);
    if (itr != m_pCells->end())
    {
        Cell const& cell = itr->second;
        return cell.formula;
    }

    return "";
}

std::string Sheet::getValue(Address const& address) const
{
    Cells::const_iterator itr = m_pCells->find(address);
    if (itr != m_pCells->end())
    {
        Cell const& cell = itr->second;
        return cell.value;
    }

    return "";
}

void Sheet::print() const
{
    for (Cells::const_iterator itr = m_pCells->begin(); itr != m_pCells->end();
         itr++)
    {
        std::cout << "[" << itr->first.column << "," << itr->first.row
                  << "]: " << itr->second.value << std::endl;
    }
}

void Sheet::recalculate()
{
    m_phase *= -1;

    // Iterate over every cell in the sheet
    for (Cells::iterator itr = m_pCells->begin(); itr != m_pCells->end(); itr++)
    {
        recalculateDepthFirst(m_phase, *m_pCells, itr->second);
    }
}

bool Sheet::setFormula(Address const& address, std::string const& formula)
{
    Cells::iterator itr = m_pCells->find(address);
    if (itr == m_pCells->end())
    {
        return m_pCells->insert(Cells::value_type(address, Cell(formula)))
            .second;
    }

    itr->second.formula = formula;
    return true;
}
