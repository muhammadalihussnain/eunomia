#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#include "user_probe.skel.h"

static void bump_memlock_rlimit(void)
{
	struct rlimit rlim_new = {
		.rlim_cur	= RLIM_INFINITY,
		.rlim_max	= RLIM_INFINITY,
	};

	if (setrlimit(RLIMIT_MEMLOCK, &rlim_new)) {
		fprintf(stderr, "Failed to increase RLIMIT_MEMLOCK limit!\n");
		exit(1);
	}
}

int main(void){
    bump_memlock_rlimit();
    struct user_probe *skel = user_probe__open();
    user_probe__load(skel);
    user_probe__attach(skel);
    while(true)
		;
    return 0;
}