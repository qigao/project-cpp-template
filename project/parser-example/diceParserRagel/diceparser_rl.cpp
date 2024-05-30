
#line 1 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
#include "diceparser.h"

namespace DP {

#line 121 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"

#line 11 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser_rl.cpp"
static const char _RollFormula_actions[] = { 0,
  1,
  0,
  1,
  1,
  1,
  2,
  1,
  3,
  1,
  4,
  1,
  5,
  1,
  6,
  1,
  7,
  1,
  8,
  1,
  9,
  1,
  10,
  2,
  4,
  1,
  2,
  4,
  2,
  2,
  4,
  5,
  2,
  4,
  6,
  2,
  4,
  7,
  2,
  4,
  8,
  2,
  4,
  9,
  2,
  4,
  10 };

static const char _RollFormula_key_offsets[] = { 0, 0, 16, 32, 32 };

static const char _RollFormula_trans_keys[] = { 0,
  10,
  32,
  33,
  37,
  40,
  41,
  42,
  43,
  45,
  47,
  77,
  100,
  109,
  49,
  57,
  0,
  10,
  32,
  33,
  37,
  40,
  41,
  42,
  43,
  45,
  47,
  77,
  100,
  109,
  48,
  57,
  0 };

static const char _RollFormula_single_lengths[] = { 0, 14, 14, 0, 0 };

static const char _RollFormula_range_lengths[] = { 0, 1, 1, 0, 0 };

static const char _RollFormula_index_offsets[] = { 0, 0, 16, 32, 33 };

static const char _RollFormula_trans_targs[] = { 3,
  3,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  0,
  3,
  3,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  0,
  0,
  0,
  0 };

static const char _RollFormula_trans_actions[] = { 21,
  21,
  0,
  19,
  11,
  3,
  5,
  11,
  13,
  13,
  11,
  17,
  15,
  17,
  7,
  1,
  44,
  44,
  9,
  41,
  29,
  23,
  26,
  29,
  32,
  32,
  29,
  38,
  35,
  38,
  7,
  1,
  0,
  0,
  0 };

static const char _RollFormula_to_state_actions[] = { 0, 0, 0, 0, 1 };

static const char _RollFormula_eof_actions[] = { 0, 1, 1, 0, 0 };

static const int RollFormula_start       = 1;
static const int RollFormula_first_final = 3;
static const int RollFormula_error       = 0;

static const int RollFormula_en_errors = 4;
static const int RollFormula_en_main   = 1;

#line 123 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"

std::istream &operator>>(std::istream &in, RollFormula &rf)
{
  std::stack<RollOperation> operation       = std::stack<RollOperation>();
  bool                      numPut          = false;
  int                       intVal          = 0;
  bool                      exclamationMark = false;
  char                      stack[100];
  int                       cs;
  int                       top;

#line 89 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser_rl.cpp"
  {
    cs  = RollFormula_start;
    top = 0;
  }

#line 134 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"

  try {
    std::string str;
    std::getline(in, str);
    const char *p   = str.c_str();
    const char *pe  = p + str.length() + 1;
    const char *eof = pe;
    const char *ts  = nullptr;

#line 106 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser_rl.cpp"
    {
      int          _klen;
      unsigned int _trans;
      const char  *_acts;
      unsigned int _nacts;
      const char  *_keys;

      if (p == pe) goto _test_eof;
      if (cs == 0) goto _out;
    _resume:
      _keys  = _RollFormula_trans_keys + _RollFormula_key_offsets[cs];
      _trans = _RollFormula_index_offsets[cs];

      _klen = _RollFormula_single_lengths[cs];
      if (_klen > 0) {
        const char *_lower = _keys;
        const char *_mid;
        const char *_upper = _keys + _klen - 1;
        while (1) {
          if (_upper < _lower) break;

          _mid = _lower + ((_upper - _lower) >> 1);
          if ((*p) < *_mid)
            _upper = _mid - 1;
          else if ((*p) > *_mid)
            _lower = _mid + 1;
          else {
            _trans += (unsigned int)(_mid - _keys);
            goto _match;
          }
        }
        _keys += _klen;
        _trans += _klen;
      }

      _klen = _RollFormula_range_lengths[cs];
      if (_klen > 0) {
        const char *_lower = _keys;
        const char *_mid;
        const char *_upper = _keys + (_klen << 1) - 2;
        while (1) {
          if (_upper < _lower) break;

          _mid = _lower + (((_upper - _lower) >> 1) & ~1);
          if ((*p) < _mid[0])
            _upper = _mid - 2;
          else if ((*p) > _mid[1])
            _lower = _mid + 2;
          else {
            _trans += (unsigned int)((_mid - _keys) >> 1);
            goto _match;
          }
        }
        _trans += _klen;
      }

    _match:
      cs = _RollFormula_trans_targs[_trans];

      if (_RollFormula_trans_actions[_trans] == 0) goto _again;

      _acts  = _RollFormula_actions + _RollFormula_trans_actions[_trans];
      _nacts = (unsigned int)*_acts++;
      while (_nacts-- > 0) {
        switch (*_acts++) {
        case 0:
#line 7 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          throw RollFormulaError("[ERROR w/ formula] incorrect formula");
          {
            cs = stack[--top];
            goto _again;
          }
        } break;
        case 1:
#line 11 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          numPut = true;
          operation.push(static_cast<RollOperation>((*p)));
        } break;
        case 2:
#line 15 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          numPut          = true;
          char topOfStack = operation.empty() ? ' ' : operation.top();
          if (operation.empty()) {
            stack[top++] = cs;
            cs           = 4;
            goto _again;
          } else if (operation.top() == '(')
            operation.pop();
          else {
            RollFormula rollFormula(static_cast<RollOperation>(topOfStack));
            rf.addEnd(rollFormula);
            operation.pop();
            p--;
          }
        } break;
        case 3:
#line 28 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          intVal = intVal * 10 + ((*p) - '0');
        } break;
        case 4:
#line 31 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          RollFormula rollFormula(intVal);
          rf.addEnd(intVal);
          intVal = 0;
          numPut = true;
        } break;
        case 5:
#line 37 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          numPut          = false;
          char topOfStack = operation.empty() ? ' ' : operation.top();
          if (operation.empty() || topOfStack == '+' || topOfStack == '-' || topOfStack == '(') {
            operation.push(static_cast<RollOperation>((*p)));
            exclamationMark = false;
          } else {
            RollFormula rollFormula(static_cast<RollOperation>(topOfStack));
            rf.addEnd(rollFormula);
            operation.pop();
            p--;
          }
        } break;
        case 6:
#line 51 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          numPut          = false;
          char topOfStack = operation.empty() ? ' ' : operation.top();
          if (operation.empty() || topOfStack == '(') {
            operation.push(static_cast<RollOperation>((*p)));
            exclamationMark = false;
          } else {
            RollFormula rollFormula(static_cast<RollOperation>(topOfStack));
            rf.addEnd(rollFormula);
            operation.pop();
            p--;
          }
        } break;
        case 7:
#line 65 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          char topOfStack = operation.empty() ? ' ' : operation.top();
          if (operation.empty() || topOfStack == '+' || topOfStack == '-' || topOfStack == '*'
              || topOfStack == '/' || topOfStack == '(' || topOfStack == '%') {
            if (!numPut) {
              RollFormula rollFormula(1);
              rf.addEnd(rollFormula);
              numPut = false;
            }
            operation.push(static_cast<RollOperation>((*p)));
          } else {
            RollFormula rollFormula(static_cast<RollOperation>(topOfStack));
            rf.addEnd(rollFormula);
            operation.pop();
            p--;
          }
        } break;
        case 8:
#line 82 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          char topOfStack = operation.empty() ? ' ' : operation.top();
          if (topOfStack == 'd' && !exclamationMark) {
            operation.push(static_cast<RollOperation>((*p)));
          } else {
            stack[top++] = cs;
            cs           = 4;
            goto _again;
          }
        } break;
        case 9:
#line 89 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          char topOfStack = operation.empty() ? ' ' : operation.top();
          if (topOfStack == 'd') {
            RollFormula rollFormula(static_cast<RollOperation>((*p)));
            rf.addEnd(rollFormula);
            exclamationMark = true;
          } else {
            stack[top++] = cs;
            cs           = 4;
            goto _again;
          }
        } break;
        case 10:
#line 97 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          if (!operation.empty()) {
            if (operation.top() == '(') {
              {
                stack[top++] = cs;
                cs           = 4;
                goto _again;
              }
            } else {
              while (!operation.empty()) {
                RollFormula rollFormula(operation.top());
                rf.addEnd(rollFormula);
                operation.pop();
              }
              p--;
            }
          }
        } break;
#line 316 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser_rl.cpp"
        }
      }

    _again:
      _acts  = _RollFormula_actions + _RollFormula_to_state_actions[cs];
      _nacts = (unsigned int)*_acts++;
      while (_nacts-- > 0) {
        switch (*_acts++) {
        case 0:
#line 7 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
        {
          throw RollFormulaError("[ERROR w/ formula] incorrect formula");
          {
            cs = stack[--top];
            goto _again;
          }
        } break;
#line 332 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser_rl.cpp"
        }
      }

      if (cs == 0) goto _out;
      if (++p != pe) goto _resume;
    _test_eof : {}
      if (p == eof) {
        const char  *__acts  = _RollFormula_actions + _RollFormula_eof_actions[cs];
        unsigned int __nacts = (unsigned int)*__acts++;
        while (__nacts-- > 0) {
          switch (*__acts++) {
          case 0:
#line 7 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"
          {
            throw RollFormulaError("[ERROR w/ formula] incorrect formula");
            {
              cs = stack[--top];
              if (p == pe) goto _test_eof;
              goto _again;
            }
          } break;
#line 356 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser_rl.cpp"
          }
        }
      }

    _out : {}
    }

#line 144 "/home/qigao/projects/project-cpp-template/parser-example/diceParserRagel/diceparser.rl"

  } catch (RollFormulaError &e) {
    throw e;
  }
  while (!operation.empty()) operation.pop();
  return in;
}
}// namespace DP
