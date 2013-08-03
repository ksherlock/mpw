
#include <string>
#include <unordered_map>
#include <cstdio>
#include <vector>


namespace MPW
{
	extern std::unordered_map<std::string, std::string> Environment;

	std::string ExpandVariables(const std::string &s);
}

namespace {


	%%{
		machine name;

		main :=

			[A-Za-z0-9_]+ @{
				name.push_back(fc);
			}
		;

		write data;
	}%%

	%%{
		machine assign;

		ws = [ \t\r\n];
		main :=

			ws*
			(
				'=' @{
					op = 1;
				}
				|
				'+=' @{
					op = 2;
				}
				|
				'?=' @{
					op = 3;
				}
			)
			ws*
		;

		write data;
	}%%

	%%{
		machine variables;	

		main := |*

			'{' [A-Za-z0-9_]+ '}' {

				std::string name(ts + 1, te - 1);
				auto iter = Environment.find(name);
				if (iter != Environment.end())
					rv.append(iter->second);
			};

			# backwards compatibility.
			'${' [A-Za-z0-9_]+ '}' {

				std::string name(ts + 2, te - 1);
				auto iter = Environment.find(name);
				if (iter != Environment.end())
					rv.append(iter->second);
			};

			# backwards compatibility.
			'$' [A-Za-z0-9_]+ {
				std::string name(ts + 1, te);
				auto iter = Environment.find(name);
				if (iter != Environment.end())
					rv.append(iter->second);
			};

			any {
				rv.push_back(*ts);
			};
		*|;

		write data;
	}%%

	// split out since goto names conflict.
	const char* GetName(const char *p, const char *pe, std::string &name)
	{
		const char *eof = pe;
		int cs;

		%%{
			machine name;
			write init;
			write exec;	
		}%%

		return p;
	}

	const char *GetAssignment(const char *p, const char *pe, unsigned &op)
	{
		const char *eof = pe;
		int cs;

		%%{
			machine assign;
			write init;
			write exec;	
		}%%

		return p;
	}

	bool ParseLine(const char *p, const char *pe)
	{
		/*
		 * name = value
		 * name += value
		 * name ?= value
		 */

		std::string name;
		unsigned op = 0;

		const char *begin = p;

		// 1. get the name
		p = GetName(p, pe, name);
		if (!name.length())
		{
		#ifdef TESTING
			fprintf(stderr, "No Name\n");
		#endif
			return false;
		}

		// 2. get the op
		p = GetAssignment(p, pe, op);
		if (!op)
		{
			fprintf(stderr, "No assignment\n");
			return false;
		}

		std::string value(p, pe);
		value = MPW::ExpandVariables(value);

		auto iter = MPW::Environment.find(name);
		if (iter == MPW::Environment.end())
		{
			MPW::Environment.emplace(std::move(name), std::move(value));
		}
		else
		{
			switch(op)
			{
			case 1: // =
				iter->second = std::move(value);
				break;
			case 2: // +=
				iter->second += value;
				break;
			case 3: // ?=
				break;

			}
		}
		return true;
	}

}

namespace MPW {


	std::string GetEnv(const std::string &key)
	{
		auto iter = Environment.find(key);
		if (iter == Environment.end()) return "";
		return iter->second;
	}

	std::string ExpandVariables(const std::string &s)
	{
		if (s.find_first_of("{$") == s.npos) return s;

		std::string rv;
		const char *p = s.c_str();
		const char *pe = p + s.length();
		const char *eof = pe;
		const char *te;
		const char *ts;

		int cs;
		int act;

		%%{
			machine variables;

			write init;
			write exec;
		}%%

		return rv;
	}

	void EnvLoadArray(const std::vector<std::string> &data)
	{
		for (const auto &s : data)
		{
			const char *begin = s.c_str();
			const char *end = begin + s.length();
			if (!s.length()) continue;
			if (!ParseLine(begin, end))
			{
				fprintf(stderr, "Error in variable: %s\n", s.c_str());
			}
		}
	}


	void EnvLoadFile(const std::string &envfile)
	{
		
		FILE *fp;


		fp = fopen(envfile.c_str(), "r");

		if (!fp) return;


		/*
		 * getline(3) is 2008 posix. it allocates (and resizes as appropriate)
		 * the buffer.
		 *
		 */
		char *lineBuffer = NULL;
		size_t lineSize = 0;

		for(;;)
		{
			char *line;
			ssize_t length;

			length = getline(&lineBuffer, &lineSize, fp);
			if (!length) continue; //?
			if (length < 0) break; // eof or error.

			line = lineBuffer;

			// skip any leading space.
			while (length && isspace(*line))
			{
				++line;
				--length;
			}
			if (!length) continue;

			// comments
			if (*line == '#') continue;


			// strip any trailing space.
			// (will be \n terminated unless there was no \n)
			while (length && isspace(line[length - 1]))
			{
				line[--length] = 0;
			}
			if (!length) continue;

			if (!ParseLine(line, line + length))
			{
				fprintf(stderr, "Error in variable: %s\n", line);
			}
		}

		fclose(fp);
	}

} // namespace

#ifdef TESTING

namespace MPW
{
	std::unordered_map<std::string, std::string> Environment;
}


int main(int argc, char **argv)
{

		
	for (int i = 1; i < argc; ++i)
	{
		char *cp = argv[i];
		int len = strlen(cp);

		if (!ParseLine(cp, cp + len))
		{
			fprintf(stderr, "Error: %s\n", cp);
		}
	}

	MPW::EnvLoadFile("/Users/kelvin/mpw/Environment.text");	

	for (auto kv : MPW::Environment)
	{
		printf("%s --> %s\n", kv.first.c_str(), kv.second.c_str());
	}

	return 0;
}

#endif
