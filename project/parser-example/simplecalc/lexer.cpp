
#line 1 "lexer.rl"
// This is just a sketch on how to make a simple calculator.
//  Reads in a file, and outputs the result of each calculation.

#include "parser.c"
#include <iostream>
#include <stdlib.h>
#include <string>

std::string getStr(char const* beg, char const* end)
{
    return std::string(beg).substr(0, end - beg);
}

#line 73 "lexer.rl"

#line 23 "lexer.cpp"
static char const _calc_actions[] = {0, 1, 0, 1, 1, 1, 2, 1, 3,  1, 4,  1, 5, 1,
                                     6, 1, 7, 1, 8, 1, 9, 1, 10, 1, 11, 1, 12};

static char const _calc_key_offsets[] = {0, 0, 2, 14, 17};

static char const _calc_trans_keys[] = {48, 57, 32, 40, 41, 42, 43, 45, 47, 59,
                                        9,  13, 48, 57, 46, 48, 57, 48, 57, 0};

static char const _calc_single_lengths[] = {0, 0, 8, 1, 0};

static char const _calc_range_lengths[] = {0, 1, 2, 1, 1};

static char const _calc_index_offsets[] = {0, 0, 2, 13, 16};

static char const _calc_trans_targs[] = {4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                         3, 0, 1, 3, 2, 4, 2, 2, 2, 2, 0};

static char const _calc_trans_actions[] = {0,  25, 21, 11, 13, 15, 7, 9,
                                           17, 19, 21, 5,  0,  0,  5, 23,
                                           0,  23, 25, 23, 23, 0};

static char const _calc_to_state_actions[] = {0, 0, 1, 0, 0};

static char const _calc_from_state_actions[] = {0, 0, 3, 0, 0};

static char const _calc_eof_trans[] = {0, 19, 0, 21, 21};

static int const calc_start = 2;
static int const calc_first_final = 2;
static int const calc_error = 0;

static int const calc_en_main = 2;

#line 76 "lexer.rl"

class Scan
{
public:
    ~Scan();
    void init();
    void execute(char const* data, size_t len);

private:
    int cs;
    int act;
    char const* ts;
    char const* te;

    void* lparser;
};

Scan::~Scan() { ParseFree(lparser, free); }

void Scan::init()
{
    lparser = ParseAlloc(malloc);

#line 114 "lexer.cpp"
    {
        cs = calc_start;
        ts = 0;
        te = 0;
        act = 0;
    }

#line 105 "lexer.rl"
}

void Scan::execute(char const* data, size_t len)
{
    char const* p = data;
    char const* pe = data + len;
    char const* eof = pe;

#line 133 "lexer.cpp"
    {
        int _klen;
        unsigned int _trans;
        char const* _acts;
        unsigned int _nacts;
        char const* _keys;

        if (p == pe)
            goto _test_eof;
        if (cs == 0)
            goto _out;
    _resume:
        _acts = _calc_actions + _calc_from_state_actions[cs];
        _nacts = (unsigned int)*_acts++;
        while (_nacts-- > 0)
        {
            switch (*_acts++)
            {
                case 1:
#line 1 "NONE"
                {
                    ts = p;
                }
                break;
#line 154 "lexer.cpp"
            }
        }

        _keys = _calc_trans_keys + _calc_key_offsets[cs];
        _trans = _calc_index_offsets[cs];

        _klen = _calc_single_lengths[cs];
        if (_klen > 0)
        {
            char const* _lower = _keys;
            char const* _mid;
            char const* _upper = _keys + _klen - 1;
            while (1)
            {
                if (_upper < _lower)
                    break;

                _mid = _lower + ((_upper - _lower) >> 1);
                if ((*p) < *_mid)
                    _upper = _mid - 1;
                else if ((*p) > *_mid)
                    _lower = _mid + 1;
                else
                {
                    _trans += (unsigned int)(_mid - _keys);
                    goto _match;
                }
            }
            _keys += _klen;
            _trans += _klen;
        }

        _klen = _calc_range_lengths[cs];
        if (_klen > 0)
        {
            char const* _lower = _keys;
            char const* _mid;
            char const* _upper = _keys + (_klen << 1) - 2;
            while (1)
            {
                if (_upper < _lower)
                    break;

                _mid = _lower + (((_upper - _lower) >> 1) & ~1);
                if ((*p) < _mid[0])
                    _upper = _mid - 2;
                else if ((*p) > _mid[1])
                    _lower = _mid + 2;
                else
                {
                    _trans += (unsigned int)((_mid - _keys) >> 1);
                    goto _match;
                }
            }
            _trans += _klen;
        }

    _match:
    _eof_trans:
        cs = _calc_trans_targs[_trans];

        if (_calc_trans_actions[_trans] == 0)
            goto _again;

        _acts = _calc_actions + _calc_trans_actions[_trans];
        _nacts = (unsigned int)*_acts++;
        while (_nacts-- > 0)
        {
            switch (*_acts++)
            {
                case 2:
#line 1 "NONE"
                {
                    te = p + 1;
                }
                break;
                case 3:
#line 24 "lexer.rl"
                {
                    te = p + 1;
                    {
                        Parse(lparser, PLUS, 0);
                    }
                }
                break;
                case 4:
#line 28 "lexer.rl"
                {
                    te = p + 1;
                    {
                        Parse(lparser, MINUS, 0);
                    }
                }
                break;
                case 5:
#line 40 "lexer.rl"
                {
                    te = p + 1;
                    {
                        Parse(lparser, OPENP, 0);
                    }
                }
                break;
                case 6:
#line 44 "lexer.rl"
                {
                    te = p + 1;
                    {
                        Parse(lparser, CLOSEP, 0);
                    }
                }
                break;
                case 7:
#line 32 "lexer.rl"
                {
                    te = p + 1;
                    {
                        Parse(lparser, TIMES, 0);
                    }
                }
                break;
                case 8:
#line 36 "lexer.rl"
                {
                    te = p + 1;
                    {
                        Parse(lparser, DIVIDE, 0);
                    }
                }
                break;
                case 9:
#line 19 "lexer.rl"
                {
                    te = p + 1;
                    {
                        // Terminate this calculation.
                        Parse(lparser, 0, 0);
                    }
                }
                break;
                case 10:
#line 70 "lexer.rl"
                {
                    te = p + 1;
                }
                break;
                case 11:
#line 48 "lexer.rl"
                {
                    te = p;
                    p--;
                    {
                        Parse(lparser, DOUBLE, atof(getStr(ts, te).c_str()));
                    }
                }
                break;
                case 12:
#line 48 "lexer.rl"
                {
                    {
                        p = ((te)) - 1;
                    }
                    {
                        Parse(lparser, DOUBLE, atof(getStr(ts, te).c_str()));
                    }
                }
                break;
#line 282 "lexer.cpp"
            }
        }

    _again:
        _acts = _calc_actions + _calc_to_state_actions[cs];
        _nacts = (unsigned int)*_acts++;
        while (_nacts-- > 0)
        {
            switch (*_acts++)
            {
                case 0:
#line 1 "NONE"
                {
                    ts = 0;
                }
                break;
#line 295 "lexer.cpp"
            }
        }

        if (cs == 0)
            goto _out;
        if (++p != pe)
            goto _resume;
    _test_eof:
    {
    }
        if (p == eof)
        {
            if (_calc_eof_trans[cs] > 0)
            {
                _trans = _calc_eof_trans[cs] - 1;
                goto _eof_trans;
            }
        }

    _out:
    {
    }
    }

#line 115 "lexer.rl"
}

int main(int argc, char** argv)
{
    char buffer[4096];
    FILE* f;
    Scan scan;
    long numbytes;

    // Read the whole file into the buffer.
    f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    numbytes = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(buffer, 1, numbytes, f);

    // Parse the buffer in one fell swoop.
    scan.init();
    scan.execute(buffer, numbytes);
    return 0;
}
