
%%{
	
	machine lexer;


	ws = [ \t\r\n];

	word = [A-Za-z0-9_];

	action emplace {
		//printf("emplacing %s\n", name.c_str());
		// trim any whitespace.
		while (value.length() && isspace(value.back()))
			value.pop_back();	
		env[std::move(name)] = std::move(value);
	}

	action error {
		fprintf(stderr, "Bad environment: %.*s\n", (int)length, line);	
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

		any $eof(emplace)  => {
			value.push_back(fc);
		} ;

		*|;


	# exit w/ cs = lexer_en_comment.

	comment := any* ${ fbreak; };

	assignment = 
	word+ ${ name.push_back(fc); }
	ws* %eof(error)
	'='
	ws*
	(any - ws)? ${ fhold; fgoto value; }
	%eof(emplace)
	;

	main := ws* ( '#' ${ fgoto comment; } | assignment);

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

	for(;;)
	{
		const char *line;
		size_t length;

		line = fgetln(fp, &length);
		if (!line) break; // eof or error.

		// strip any trailign space.
		while (length && isspace(line[length - 1]))
			length--;

		std::string name;
		std::string value;

		const char *p = line;
		const char *pe = line + length;
		const char *eof = line + length;
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