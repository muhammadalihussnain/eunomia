#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#include "fentry_fexit.skel.h"

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
    struct fentry_fexit *skel = fentry_fexit__open();
    fentry_fexit__load(skel);
    fentry_fexit__attach(skel);
    while(true)
		;
    return 0;
}