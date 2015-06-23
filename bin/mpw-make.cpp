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
#include <algorithm>

#include <unistd.h>
#include <sysexits.h>

#include <string>
#include <vector>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>


int parse_makefile(int fd);
void help(void);
void launch_make(std::vector<char *> &argv);
void print_command(const std::vector<char *> &argv);
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


void launch_make(std::vector<char *> &argv) {
	int ok;

	if (argv.empty() || argv.back() != nullptr)
		argv.push_back(nullptr);

	ok = execvp(argv.front(), argv.data());
	perror("execvp: ");
	exit(EX_OSERR);
}

void print_command(const std::vector<char *> &argv) {

	for (auto cp : argv) {
		if (cp) printf("%s ", cp);
	}
	printf("\n");
}

int launch_command(std::vector<char *> &argv) {

	int pid;

	// parse command-line
	// launch command.
	// wait for a return status.
	//puts(cmd.c_str());

	if (argv.empty() || argv.back() != nullptr)
		argv.push_back(nullptr);

	print_command(argv);
	if (dry_run) return 0;

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



int main(int argc, char **argv) {

	int pipes[2];
	int ok;
	int child;


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
