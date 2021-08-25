#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int m[2005];
char buf[4096], buf2[4096];
char name[1024];
char g_name[1024];
char std_name[2014];

void run(char* cmd, char* input_file, char* output_file) {
	int pid = fork();
	if (pid == -1) {
		perror("vmake");
		abort();
	}
	
	if (pid != 0) {
		waitpid(pid, nullptr, 0);
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
	
	sprintf(g_name, "./mk_%s", name);
	sprintf(std_name, "./std_%s", name);
	
	printf("0/%d made. (0%%)\n", total);
	
	int ok = 0;
	for (int i = 0; i < n; ++i) {
		printf("Making subtask#%d:\n", i + 1);
		sprintf(buf, "%d", i);
		write_file(".input.tmp", buf);
		for (int j = 1; j <= m[i]; ++j) {
			sprintf(buf, "%s.%d.%d.in", name, i + 1, j);
			sprintf(buf2, "%s.%d.%d.out", name, i + 1, j);
			run(g_name, ".input.tmp", buf);
			run(std_name, buf, buf2);
			ok += 1;
			printf("\t%d/%d made. (%d%%)\n", ok, total, ok * 100 / total);
		}
	}
	printf("Done.\n");
	return 0;
}
