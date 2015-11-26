/*
 *
 * Run MPW's make.
 * 
 * MPW Make prints a list of commands to stdout.
 * read and execute them.
 */

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cctype>

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>


#include <unistd.h>
#include <sysexits.h>

#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>


int parse_makefile(int fd);
void help(void);
void launch_make(std::vector<char *> &argv);
void print_command(const std::vector<char *> &argv, bool active);
void print_command(const std::vector<std::string> &argv, bool active);

int launch_command(const std::vector<std::string> &argv);
int launch_command(std::vector<char *> &argv);

bool dry_run = false;

void help(void) {

	#undef _
	#define _(x) puts(x)

	_("Make                       # build up-to-date version of a program");
	_("Make [option...] [target...]");
	_("    -d name[=value]         # define variable name (overrides makefile definition)");
	_("    -e                      # rebuild everything regardless of dates");
	_("    -f filename             # read dependencies from specified file (default: MakeFile)");
	_("    -i dirname              # additional directory to search for include files");
	_("    -[no]mf                 # [don't] use temporary memory (default: mf)");
	_("    -p                      # write progress information to diagnostics");
	_("    -r                      # display the roots of the dependency graph");
	_("    -s                      # display the structure of the dependency graph");
	_("    -t                      # touch dates of targets and prerequisites");
	_("    -u                      # write list of unreachable targets to diagnostics");
	_("    -v                      # write verbose explanations to diagnostics (implies -p)");
	_("    -w                      # suppress warning messages");
	_("    -y                      # like -v, but omit announcing up-to-date targets");
	_("");
	_("    --help                  # display help");
	_("    --dry-run               # show what commands would run");
#undef _
}


std::string &lowercase(std::string &s) {
	std::transform(std::begin(s), std::end(s), std::begin(s), std::tolower);
	return s;
}

std::unordered_map<std::string, std::string> environment;
std::unordered_map<std::string, std::function<int(const std::vector<std::string> &)>> builtins;

/*
 * 0 = active
 * 1+ = nesting level.
 */
unsigned if_status = 0;

/* true/false if true condition encountered at this level */

std::vector<unsigned> if_stack;

int builtin_set(const std::vector<std::string> &argv) {
	// Set name value

	bool active = if_status == 0;
	print_command(argv, active);

	if (!active) return 0;

	if (argv.size() != 3) return 1;
	std::string key = argv[1];
	std::string value = argv[2];

	// lower-case key...
	lowercase(key);

	// c++17 has insert_or_assign
	environment[std::move(key)] = std::move(value);


	return 0;
}

bool evaluate(const std::vector<std::string> &v)
{
	return false;
}



int builtin_if(const std::vector<std::string> &argv) {
	/*
	 * if expression
	 * ...
	 * [else if expression]
	 * [else]
	 * end
	 */

	bool active = (if_status == 0);

	print_command(argv, active);


	if (if_status) {
		if_stack.push_back(false);
		++if_status;
		return 0;
	}
	// todo... actually evaluate expression.

	bool ok = evaluate(argv);
	if_stack.push_back(ok);
	if (!ok) ++if_status;

	return 0;
}


int builtin_else(const std::vector<std::string> &argv) {

	// else 
	// else if ....

	print_command(argv, if_status < 2);

	if (if_stack.empty()) {
		fprintf(stderr, "ELSE without IF\n");
		return -1;
	}

	unsigned &processed = if_stack.back();



	// best case -- entire if/else/end stmt is inactive.

	if (if_status > 1) {
		return 0;
	}

	// second best case -- a true condition has already been found.
	if (processed) {
		if_status = 1;
		return 0;
	}

	// third case -- evalute a naked else
	if (argv.size() == 1) {
		processed = true;
		if_status = 0;
		return 0;
	}

	// fourth case -- evaluate and else if
	bool ok  = evaluate(argv);
	if (ok) {
		processed = true;
		if_status = 0;
		return 0;
	} else {
		if_status = 1;
		return 0;
	}

}

int builtin_end(const std::vector<std::string> &argv) {

	print_command(argv, if_status < 2);

	if (if_stack.empty()) {
		fprintf(stderr, "END without IF\n");
		return 1;
	}


	if_stack.pop_back();

	if (if_status) {
		--if_status;
	}

	return 0;
}

int builtin_echo(const std::vector<std::string> &argv) {
	// echo [-n] ...

	bool active = (if_status == 0);

	print_command(argv, active);
	if (!active) return 0;
	if (dry_run) return 0;
	
	bool n = false;
	bool printed = false;

	for (auto iter = argv.begin() + 1; iter != argv.end(); ++iter) {
		const std::string &s = *iter;
		if (s.length() == 2 && s == "-n") {
			n = true;
			continue;
		}
		if (printed) fputs(" ", stdout);
		fputs(s.c_str(), stdout);
		printed = true;
	}
	if (!n) fputs("\n", stdout);
	return 0;
}

void launch_make(std::vector<char *> &argv) {
	int ok;

	if (argv.empty() || argv.back() != nullptr)
		argv.push_back(nullptr);

	ok = execvp(argv.front(), argv.data());
	perror("execvp: ");
	exit(EX_OSERR);
}

void print_command(const std::vector<char *> &argv, bool active) {

	if (!active) printf("# ");
	for (auto cp : argv) {
		if (cp) printf("%s ", cp);
	}
	printf("\n");
}

void print_command(const std::vector<std::string> &argv, bool active) {

	if (!active) printf("# ");
	for (const auto &cp : argv) {
		printf("%s ", cp.c_str());
	}
	printf("\n");
}



int launch_command(std::vector<char *> &argv) {

	int pid;

	// parse command-line
	// launch command.
	// wait for a return status.
	//puts(cmd.c_str());

	bool active = (if_status == 0);

	print_command(argv, active);

	if (!active) return 0;
	if (dry_run) {
		printf("\n");
		return 0;
	}

	pid = fork();
	if (pid < 0) {
		perror("fork: ");
		exit(EX_OSERR);
	}

	if (pid == 0) {
		execvp(argv.front(), argv.data());
		perror("execvp: ");
		exit(EX_OSERR);
	}

	for(;;) {
		int status;
		pid_t ok;
		ok = waitpid(pid, &status, 0);
		if (ok < 0) {
			if (errno == EINTR) continue;
			perror("waitpid:");
			exit(EX_OSERR);
		}
		printf("\n");

		if (WIFEXITED(status)) return WEXITSTATUS(status);
		if (WIFSIGNALED(status)) return -1;
		fprintf(stderr, "waitpid - unexpected result\n");
		exit(EX_OSERR);
	}

}

int launch_command(const std::vector<std::string> &argv) {


	if (argv.empty()) return 0;

	std::string cmd = argv[0];
	lowercase(cmd);
	auto iter = builtins.find(cmd);
	if (iter != builtins.end()) return iter->second(argv);

	std::vector<char *> cargv;
	cargv.reserve(argv.size()+2);


	// convert to char * for exec, add "mpw", and remove blanks.

	cargv.push_back(strdup("mpw"));
	for (const auto &s : argv) {
		if (!s.empty()) cargv.push_back(strdup(s.c_str()));
	}

	// and 0-terminate.
	cargv.push_back(nullptr);

	int rv = launch_command(cargv);
	std::for_each(cargv.begin(), cargv.end(), free);
	return rv;
}

int main(int argc, char **argv) {

	int pipes[2];
	int ok;
	int child;

#if 0
	builtins.emplace("if", builtin_if);
	builtins.emplace("end", builtin_end);
	builtins.emplace("else", builtin_else);
	builtins.emplace("set", builtin_set);
#endif
	builtins = {
		{"if", builtin_if},
		{"end", builtin_end},
		{"else", builtin_else},
		{"set", builtin_set},
		{"echo", builtin_echo},
	};

	std::vector<char *>new_argv;
	new_argv.push_back((char *)"mpw");
	new_argv.push_back((char *)"Make");




	//new_argv.insert(new_argv.end(), argv + 1, argv + argc);

	// filter out --help and --dry-run

	std::copy_if(argv + 1, argv + argc, std::back_inserter(new_argv),
			[](char *cp){
				if (strcmp(cp, "--help") == 0) {
					help();
					exit(0);
					return false;
				}
				if (strcmp(cp, "--dry-run") == 0) {
					dry_run = true;
					return false;
				}
				return true;
			}

	);


	ok = pipe(pipes);
	if (ok < 0) {
		perror("pipe: ");
	}

	child = fork();
	if (child < 0) {
		perror("fork: ");
		exit(EX_OSERR);
	}

	if (child == 0) {
		// child.
		//close(STDIN_FILENO);
		//close(STDOUT_FILENO);
		//close(STDERR_FILENO);

		ok = dup2(pipes[1], STDOUT_FILENO);
		if (ok < 0) {
			perror("dup2: ");
			exit(EX_OSERR);
		}
		close(pipes[0]);
		close(pipes[1]);

		launch_make(new_argv); // no return.
		exit(0);
	}

	// parent process.
	// read commands from pipe[0] and execute them. 
	// 0xb6 is a line continuation marker.
	//int fd = pipes[0];

	/*
	FILE *f = fdopen(pipes[0], "r");
	if (!f) {
		perror("fdopen: ");
		exit(EX_OSERR);
	}
	*/
	close(pipes[1]);

	// prevent children from inheriting the pipe fd.
	fcntl(pipes[0], F_SETFD, FD_CLOEXEC);

	ok = parse_makefile(pipes[0]);
	close(pipes[0]);


	// return any error from make itself.
	for(;;) {
		int status;
		int ok = waitpid(child, &status, 0);
		if (ok < 0) {
			if (errno == EINTR) continue;
			perror("waitpid: ");
			exit(EX_OSERR);
		}

		if (WIFEXITED(status)) {
			ok = WEXITSTATUS(status);
			if (ok) exit(ok);
			break;
		}
		if (WIFSIGNALED(status))
			exit(1);

		fprintf(stderr, "waitpid - unexpected result\n");
		exit(EX_OSERR);
	}

	return ok ? 2 : 0;
}
