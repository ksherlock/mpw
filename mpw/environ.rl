
%%{
	
	machine lexer;


	ws = [ \t];

	word = [A-Za-z0-9_];

	action emplace {
		//printf("emplacing %s\n", name.c_str());
		// trim any whitespace.
		//while (value.length() && isspace(value.back()))
		//	value.pop_back();	
		env[std::move(name)] = std::move(value);
	}

	value := |*

		'$' word+  => {
			std::string name(ts + 1, te);
			auto iter = env.find(name);
			if (iter != env.end())
				value.append(iter->second);
		};
		
		'${' word+ '}'  => {
			std::string name(ts + 2, te - 1);
			auto iter = env.find(name);
			if (iter != env.end())
				value.append(iter->second);
		};

		'$$' => {
			value.push_back('$');
		};

		'\n' => emplace;

		any - '\n' => {
			value.push_back(fc);
		} ;


		*|;


	# exit w/ cs = lexer_en_comment.

	comment := any* ${ fbreak; };

	assignment := 
		word+ ${ name.push_back(fc); }
		ws*
		'='
		ws*
		# ws does not include '\n', so that will be handled
		# as a value.
		(any - ws)? ${ fhold; fgoto value; }
	;

	main := |*
		ws; # leading space
		'\n'; # blank line.
		'#' => { fgoto comment; };
		word => { fhold; fgoto assignment; };
		*|;
}%%

#include <string>
#include <unordered_map>
#include <cstdio>
#include <cctype>

namespace MPW {

void LoadEnvironment(std::string &envfile, std::unordered_map<std::string, std::string> &env)
{
	%% write data;

	FILE *fp;


	fp = fopen(envfile.c_str(), "r");

	if (!fp) return;

	/*
	 * fgetln (for something more portable, try getline)
	 * may or may not include the trailing \r. at the eof.
	 * To compensate, trim all trailing whitespace and run it
	 * a second time (with eof=pe) on a string of "\n"
	 *
	 */

	for(;;)
	{
		const char *line;
		size_t length;

		line = fgetln(fp, &length);
		if (!line) break; // eof or error.

		// strip any trailing space.
		while (length && isspace(line[length - 1]))
			length--;

		if (!length) continue;

		std::string name;
		std::string value;

		std::string buffer(line, line + length);
		buffer.push_back('\n');

		const char *p = buffer.c_str();
		const char *pe = p + buffer.length();
		const char *eof = pe;
		const char *te;
		const char *ts;

		int cs, act;

		%%write init;

		%%write exec;

		if (cs == lexer_error)
		{
			fprintf(stderr, "Bad environment: %.*s\n", (int)length, line);
			continue;
		}
	}

	fclose(fp);
}

} // namespace

#ifdef TEST

int main(int argc, char **argv)
{

	for (int i = 1; i < argc; ++i)
	{
		std::unordered_map<std::string, std::string> env;
		std::string f(argv[i]);

		MPW::LoadEnvironment(f, env);

		for (const auto &kv : env)
		{
			printf("%s -> %s\n", kv.first.c_str(), kv.second.c_str());
		} 
	}

	return 0;
}


#endif