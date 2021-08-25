#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
using namespace std;

int m[2005];
char buf[4096], buf2[4096];
char name[1024];
char g_name[1024];
char std_name[2014];

int run(char* cmd, char* input_file, char* output_file) {
	int pid = fork();
	if (pid == -1) {
		perror("vcheck run()");
		abort();
	}
	
	if (pid != 0) {
		clock_t s = clock();
		waitpid(pid, nullptr, 0);
		clock_t t = clock();
		return t - s;
	} else {
		freopen(input_file, "r", stdin);
		freopen(output_file, "w", stdout);
		char* argv[] = {cmd, nullptr};
		execv(cmd, argv);
	}
}

void write_file(char* file_name, char* content) {
	FILE *f = fopen(file_name, "w");
	fprintf(f, "%s", content);
	fclose(f);
}

bool diff_file(char* file1, char* file2) {
	static char buf[4096], buf2[4096];
	
	FILE *f1 = fopen(file1, "r");
	FILE *f2 = fopen(file2, "r");
	
	while (~fscanf(f1, "%s", buf)) {
		if (~fscanf(f2, "%s", buf2)) {
			if (strcmp(buf, buf2) != 0) {
				fclose(f1);
				fclose(f2);
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}

int main() {
	int n;
	FILE *f = fopen("vtest.conf", "r");
	fscanf(f, "%s %d", name, &n);
	
	int total = 0;
	for (int i = 0; i < n; ++i) {
		fscanf(f, "%d", m + i);
		total += m[i];
	}
	fclose(f);
	
	sprintf(std_name, "./run_%s", name);
	
	printf("0/%d made. (0%%)\n", total);
	
	int ok = 0, id = 0;
	int maxt = -1, sumt = 0, maxti, maxtj;
	for (int i = 0; i < n; ++i) {
		printf("Making subtask#%d:\n", i + 1);
		for (int j = 1; j <= m[i]; ++j) {
			sprintf(buf, "%s.%d.%d.in", name, i + 1, j);
			sprintf(buf2, "output.txt", name, i + 1, j);
			int t = run(std_name, buf, buf2);
			sumt += t;
			if (t > maxt) {
				maxt = t;
				maxti = i;
				maxtj = j;
			}
			
			sprintf(buf, "%s.%d.%d.out", name, i + 1, j);
			id += 1;
			if (diff_file(buf, buf2)) {
				ok += 1;
				printf("\tCase %d: Answer Correct. [%dms] (%d%%)\n", id, t, id * 100 / total);
			} else {
				printf("\tCase %d: Wrong Answer. [%dms] (%d%%)\n", id, t, id * 100 / total);
			}
		}
	}
	printf("%d/%d cases passed.\n", ok, total);
	printf("Sum: %dms. Slowest: %dms. Slowest case: %s.%d.%d.in\n", sumt, maxt, name, maxti + 1, maxtj);
	printf("Done.\n");
	return 0;
}
