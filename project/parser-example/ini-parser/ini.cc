
#line 1 "ini.rl"
#include <cstddef>
#include <cassert>
#include <cstring>
#include <string>
#include <memory>

#include "ini.hh"
#include "mmap.hh"

using std::unique_ptr;

namespace ini {


#line 48 "ini.rl"



#line 22 "ini.cc"
static const char _parser_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 2, 0, 3, 2, 2, 3
};

static const char _parser_key_offsets[] = {
	0, 0, 2, 14, 18, 23, 25, 39, 
	45, 53, 62, 63, 73, 84, 96, 110
};

static const char _parser_trans_keys[] = {
	10, 39, 32, 45, 61, 95, 9, 13, 
	48, 57, 65, 90, 97, 122, 32, 61, 
	9, 13, 10, 32, 39, 9, 13, 10, 
	39, 10, 32, 39, 45, 61, 95, 9, 
	13, 48, 57, 65, 90, 97, 122, 10, 
	32, 39, 61, 9, 13, 45, 95, 48, 
	57, 65, 90, 97, 122, 45, 93, 95, 
	48, 57, 65, 90, 97, 122, 10, 10, 
	39, 45, 95, 48, 57, 65, 90, 97, 
	122, 10, 39, 45, 93, 95, 48, 57, 
	65, 90, 97, 122, 10, 35, 45, 59, 
	91, 95, 48, 57, 65, 90, 97, 122, 
	10, 32, 39, 45, 91, 95, 9, 13, 
	48, 57, 65, 90, 97, 122, 10, 32, 
	35, 45, 59, 61, 91, 95, 9, 13, 
	48, 57, 65, 90, 97, 122, 0
};

static const char _parser_single_lengths[] = {
	0, 2, 4, 2, 3, 2, 6, 4, 
	2, 3, 1, 4, 5, 6, 6, 8
};

static const char _parser_range_lengths[] = {
	0, 0, 4, 1, 1, 0, 4, 1, 
	3, 3, 0, 3, 3, 3, 4, 4
};

static const char _parser_index_offsets[] = {
	0, 0, 3, 12, 16, 21, 24, 35, 
	41, 47, 54, 56, 64, 73, 83, 94
};

static const char _parser_indicies[] = {
	1, 2, 0, 3, 4, 5, 4, 3, 
	4, 4, 4, 2, 6, 7, 6, 2, 
	10, 9, 2, 9, 8, 12, 2, 11, 
	14, 13, 2, 15, 5, 15, 13, 15, 
	15, 15, 11, 17, 16, 2, 7, 16, 
	11, 18, 18, 18, 18, 18, 2, 19, 
	20, 19, 19, 19, 19, 2, 1, 2, 
	12, 2, 21, 21, 21, 21, 21, 11, 
	12, 2, 22, 23, 22, 22, 22, 22, 
	11, 1, 0, 24, 0, 25, 24, 24, 
	24, 24, 2, 10, 9, 2, 26, 27, 
	26, 9, 26, 26, 26, 8, 28, 6, 
	0, 24, 0, 7, 25, 24, 6, 24, 
	24, 24, 2, 0
};

static const char _parser_trans_targs[] = {
	1, 13, 0, 3, 2, 4, 3, 4, 
	5, 4, 14, 5, 13, 7, 15, 6, 
	7, 15, 9, 9, 10, 12, 12, 5, 
	2, 8, 6, 11, 15
};

static const char _parser_trans_actions[] = {
	0, 0, 0, 5, 0, 5, 0, 0, 
	1, 1, 9, 0, 7, 5, 12, 0, 
	0, 7, 1, 0, 3, 1, 0, 3, 
	1, 0, 1, 1, 0
};

static const int parser_start = 13;
static const int parser_first_final = 13;
static const int parser_error = 0;

static const int parser_en_main = 13;


#line 51 "ini.rl"

class parser {
  int cs;
  ptrdiff_t mark;
  size_t nread;
  ini::section *section;

public:
  ini::settings settings;

  parser() : cs(0), mark(0), nread(0), section(nullptr) {
    
#line 118 "ini.cc"
	{
	cs = parser_start;
	}

#line 63 "ini.rl"
  }

  size_t feed(const char *data, size_t len, size_t off) {
    assert(off <= len && "offset past end of buffer");

    string key;
    const char *p = data + off;
    const char *pe = data + len;

    assert(pe - p == len - off && "pointers aren't same distance");

    
#line 136 "ini.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _parser_trans_keys + _parser_key_offsets[cs];
	_trans = _parser_index_offsets[cs];

	_klen = _parser_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _parser_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _parser_indicies[_trans];
	cs = _parser_trans_targs[_trans];

	if ( _parser_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _parser_actions + _parser_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 17 "ini.rl"
	{ mark = p - data; }
	break;
	case 1:
#line 19 "ini.rl"
	{
    string name(data + mark, (size_t)(p - data - mark));

    printf("section: %s\n", name.c_str());
    settings[name] = {};
    section = &settings.at(name);
  }
	break;
	case 2:
#line 27 "ini.rl"
	{
    key = string(data + mark, (size_t)(p - data - mark));
  }
	break;
	case 3:
#line 31 "ini.rl"
	{
    string value(data + mark, (size_t)(p - data - mark));
    if (section) {
      section->emplace(std::move(key), std::move(value));
    }
  }
	break;
#line 239 "ini.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 75 "ini.rl"

    assert(p <= pe && "buffer overflow after parsing.");

    nread += p - data + off;

    assert(nread <= len && "nread longer than length");
    assert(mark < len && "mark is after buffer end");

    return nread;
  }

  bool error() const { return cs == parser_error; }
  bool finished() const { return cs >= parser_error; }
};

}

/* should be able to read multiple files at once */
ini::settings ini::read_conf(const string &filename) {
  ini::parser parser;
  memorymap map(filename);

  parser.feed(map.data(), map.size(), 0);

  printf("error? %s\n", parser.error() ? "yes" : "no");
  printf("finished? %s\n", parser.finished() ? "yes" : "no");

  return parser.settings;
}
