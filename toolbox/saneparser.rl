
#include "sane.h"
#include <string>

namespace SANE {

/*
 * %{} is a final transition.  fpc points to the next character
 * %!{} is a final transition to an error state. fpc " " " " 
 *
 */
%%{
	
	machine fpstr;
	

	action check { checkpoint = fpc; }

	nantype =
		'('
		digit* ${ nantype = nantype * 10 + fc - '0'; }
		')'
		$!{ nantype = 0; }
		%check
		;

	nan = 'NAN'i 
		>{ nan = true; } 
		%check
		%!check
		nantype?
		;

	infinity = 'INF'i
		>{ nan = true; }
		%{ nan = false; infinity = true; checkpoint = fpc; }
		%!{ nan = false; infinity = true; checkpoint = fpc; };

	exponent =
		[eE]
		[+\-]? ${ if (fc == '-') negative_exp = true; }
		digit+ ${ exp = exp * 10 + fc - '0'; }
		%check
		%!check
		;

	significand =
		(
			(
				digit+  ${
					// don't push leading 0s
					if (fc != '0' || siga.size())
						siga.push_back(fc);
				}
				( '.' digit* ${ sigb.push_back(fc); })?
			)
			| 
			(
				'.' 
				digit+ ${ sigb.push_back(fc); }
			)
		)
		%check
		%!check
	; 

	finite_number = significand  exponent?;

	unsigned_decimal = finite_number | infinity | nan;

	left_decimal = 
		[+\-]? ${ if (fc == '-') negative = true; } 
		unsigned_decimal
	;

	decimal_number = [ \t]* left_decimal;

	main := decimal_number;

}%%


std::string normalize(std::string &a, std::string &b, int &exponent)
{
	int pos;

	// 1 = 1e0 10 = 1e1
	// 12 = 12e0
	// 123 = 123e0

	// 1.1 = 11e-1
	// 0.1 = 1e-1

	// remove trailing 0s
	while (b.size() && b.back() == '0')
		b.pop_back();

	int bits = 0;
	if (a.length()) bits |= 0x01;
	if (b.length()) bits |= 0x02;

	std::string out;
	switch(bits)
	{
	case 0x00:
		// should never happen...
		break;

	case 0x01:
		// a only.
		// remove trailing 0s and add 1 exp for each.
		while (a.length() && a.back() == '0')
		{
			a.pop_back();
			exponent++;
		}
		out = a;
		break;

	case 0x02:
		// b only.
		//.001 = e1-3
		exponent -= b.length();
		pos = b.find_first_not_of('0');
		out = b.substr(pos);
		break;

	case 0x03:
		// a and b
		// 10.01 = 1001e-2
		exponent -= b.length();
		out = a;
		out += b;
		break;

	}
	if (out.empty()) out = "0";
	return out;
}




void str2dec(const std::string &s, uint16_t &index, decimal &d, uint16_t &vp)
{
%%write data;

		bool infinity = false;
		bool nan = false;
		int nantype = 0;
		bool negative = false;
		bool negative_exp = false;
		int exp = 0;
		std::string siga, sigb;

		if (index >= s.size()) return;

		/*
		char *p = s.c_str();
		char *pe = p + s.size();
		char *eof = pe;
		char *checkpoint = p;
		*/
		auto p = s.begin();
		auto checkpoint = s.begin();

		auto pe = s.end();
		auto eof = s.end();

		int cs;

		p += index;

	%%write init;

	%%write exec;

		d.sgn = negative ? 1 : 0;

		if (infinity)
		{
			d.sig = "I";
		}
		else if (nan)
		{
			d.sig = "N";
			if (nantype)
			{
				const char *hexstr = "0123456789abcdef";
				// 4-byte hex
				d.sig.push_back(hexstr[(nantype >> 24) & 0xff]);
				d.sig.push_back(hexstr[(nantype >> 16) & 0xff]);
				d.sig.push_back(hexstr[(nantype >> 8) & 0xff]);
				d.sig.push_back(hexstr[(nantype >> 0) & 0xff]);
			}
		}
		else
		{
			d.sig = normalize(siga, sigb, exp);
			d.exp = negative_exp ? -exp : exp;
		}

		vp = cs != fpstr_error;
		index = checkpoint - s.begin();

		return;
		#if 0


		printf("%s\n", str);

		bool valid = cs  != fpstr_error;
		int index = checkpoint - str;

		printf("infinity: %d\n", infinity);
		printf("     nan: %d\n", nan);
		printf(" nantype: %d\n", nantype);

		if (negative_exp) exp = -exp;

		printf("     exp: %d\n", exp);
		printf("negative: %d\n", negative);
		printf("     sig: %s.%s\n", siga.c_str(), sigb.c_str());
		printf("   valid: %d\n", valid);
		printf("   index: %d\n", index);
		printf("\n");


		/*
		 * now we need to normalize the significand / exponent
		 * 1.2 e0 -> 12 e-1
		 * 1000 e0 -> 1 e 3
		 */

		std::string sig = normalize(siga, sigb, exp);

		printf("normalized: %s e %d\n", sig.c_str(), exp);
		#endif
}

} // namespace

#ifdef MAIN
#include <cstdio>

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::string s = argv[i];
		SANE::decimal d;
		short index = 0;
		short valid = 0;

		SANE::str2dec(s, index, d, valid);

		printf("index: %d\n", index);
		printf("valid: %d\n", valid);
		printf(" sign: %d\n", d.sgn);
		printf("   exp: %d\n", d.exp);
		printf("  sig: %s\n", d.sig.c_str());
		printf("\n");
	}


}

#endif
