#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include "ring_buffer.skel.h"
#include <sys/resource.h>
#include "ring_buffer.h"

static void bump_memlock_rlimit(void)
{
    struct rlimit rlim_new = {
        .rlim_cur    = RLIM_INFINITY,
        .rlim_max    = RLIM_INFINITY,
    };

    if (setrlimit(RLIMIT_MEMLOCK, &rlim_new)) {
        fprintf(stderr, "Failed to increase RLIMIT_MEMLOCK limit!\n");
        exit(1);
    }
}

int main(void)
{
    bump_memlock_rlimit();
    struct ring_buffer *skel = ring_buffer__open();
    ring_buffer__load(skel);
    ring_buffer__attach(skel);

    // Get the file descriptor of the events map
    int events_map_fd = bpf_map__fd(skel->maps.events);
    if (events_map_fd < 0) {
        fprintf(stderr, "Failed to get the file descriptor for the events map.\n");
        exit(1);
    }

    
    struct event *event;
    __u64 key = 1;  // Assuming key is 0, adjust if needed
    int ret = bpf_map_lookup_elem(events_map_fd, &key, &event);
       
        

    if (ret < 0 && event) {
            // Print the event details
        printf("Received event: pid=%u, uid=%u, ppid=%u, comm=%s\n",
             event->pid, event->uid, event->ppid, event->comm);
        }

    usleep(100000);  // Sleep for 100ms, adjust as needed
    

    // Detach and close the BPF program and maps
    //ring_buffer__detach(skel);
    //ring_buffer__destroy(skel);

    return 0;
}
