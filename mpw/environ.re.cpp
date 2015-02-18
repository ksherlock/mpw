#include <string>
#include <unordered_map>

#include <cstdio>

/*
 * #...comment
 * var=value
 * where value may contain $var and ${var} interpolations.
 *
 */

namespace MPW {
std::string EvalString(std::string &s, std::unordered_map<std::string, std::string> &env)
{

	std::string rv;
	const char *cp = s.c_str();
	const char *marker = NULL;

	while (*cp)
	{
		const char *begin = cp;

		/*!re2c
		re2c:define:YYCTYPE  = "char";
		re2c:define:YYCURSOR = cp;
		re2c:define:YYMARKER = marker;
		re2c:yyfill:enable   = 0;
		re2c:yych:conversion = 1;
		re2c:indent:top      = 1;

		'$' [A-Za-z0-9_]+ {
			std::string tmp(begin + 1, cp);

			// lookup value...
			// append to rv.
			auto iter = env.find(tmp);
			if (iter != env.end())
			{
				rv.append(iter->second);
			}
			continue;
		}

		'${' [A-Za-z0-9_]+ '}' {

			std::string tmp(begin + 2, cp - 1);

			auto iter = env.find(tmp);
			if (iter != env.end())
			{
				rv.append(iter->second);
			}
			continue;
		}

		. {
			rv.push_back(*begin);
			continue;
		}
		[^] {
			break;
		}
		*/

	}

	return rv;
}


void LoadEnvironment(std::string &envfile, std::unordered_map<std::string, std::string> &env)
{

	FILE *fp;


	fp = fopen(envfile.c_str(), "r");

	if (!fp) return;

	for (;;)
	{
		char *begin;
		char *end;
		char *iter;

		size_t length;

		begin = fgetln(fp, &length);
		if (!begin) break; // eof or error.

		if (!length) continue;
		if (begin[0] == '#') continue;

		while (length && isspace(begin[length - 1])) --length;
		if (!length) continue;

		// key=value

		// ehh, this could really check for [A-Za-z0-9_]+ '='

		end = begin + length;
		iter = std::find(begin, end, '=');

		if (iter == end || iter == begin)
		{
			fprintf(stderr, "Invalid Environment entry: %.*s\n", (int)length, begin);
			continue;
		}

		std::string key(begin, iter);
		std::string value(iter + 1, end);

		value = EvalString(value, env);

		//fprintf(stdout, "%s = %s\n", key.c_str(), value.c_str());

		// todo -- does this replace an existing value?
		env.emplace(key, value);
	}

	fclose(fp);
}

}
