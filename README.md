*This project has been created as part of the 42 curriculum by [ibel-lot ].*

# Project Name

## Description

This project is a multithreaded simulation designed to explore and solve classic
concurrency problems using POSIX threads. It models a group of "coders" who must
periodically acquire a shared, limited resource (referred to here as "dongles") in
order to work, while a monitoring thread tracks each coder's status and detects
when a coder has gone too long without working ("burnout").

The goal of the project is to correctly implement thread synchronization in a
scenario that mirrors the classic dining philosophers problem, while addressing
real-world concurrency pitfalls such as deadlock, starvation, race conditions, and
precise timing constraints — all without relying on busy-waiting or imprecise
sleep-based polling wherever avoidable.

## Instructions

### Compilation

The project is compiled using a `Makefile` provided at the root of the repository.

```bash
make        # builds the project
make clean  # removes object files
make fclean # removes object files and the binary
make re     # rebuilds the project from scratch
```

### Execution

```bash
./program <number_of_coders> <burnout_time> <work_time> <cooldown_time> [<number_of_cycles>]
```

- `number_of_coders`: number of coder threads to simulate
- `burnout_time`: time (in ms) after which a coder is considered "burnt out" if they haven't started working
- `work_time`: time (in ms) it takes a coder to complete a work session
- `cooldown_time`: time (in ms) a coder rests between work sessions
- `number_of_cycles` (optional): if provided, the simulation stops once every coder has completed this many work cycles

Example:

```bash
./program 5 800 200 200
```

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions):** shared resources ("dongles") are
  always requested in a fixed, consistent order across all threads, which breaks
  the circular wait condition. Resources are also acquired atomically as a pair
  where required, preventing a thread from holding one resource indefinitely while
  waiting on another.
- **Starvation prevention:** resource access is arbitrated so that no single coder
  can be perpetually skipped in favor of others — for example by alternating
  priority, using fair queuing on the resource locks, or bounding the number of
  times a neighbor can acquire a resource before yielding it.
- **Cooldown handling:** the cooldown period is implemented as a non-blocking wait
  relative to the rest of the simulation, ensuring a resting coder does not hold
  onto any shared resource and does not block the monitor thread from checking
  other coders' status.
- **Precise burnout detection:** timestamps are recorded with a high-resolution
  clock each time a coder finishes an action, and the monitoring thread compares
  the current time against these timestamps at short, regular intervals so that
  burnout is detected as close as possible to the exact configured threshold,
  without relying on coarse or delayed polling.
- **Log serialization:** all status messages (a coder starting to work, resting,
  or burning out) are written through a single synchronized logging function so
  that output lines are never interleaved or corrupted when multiple threads log
  at the same time.

## Thread synchronization mechanisms

- **`pthread_mutex_t`:** used to protect every piece of shared state that more
  than one thread can access, including the dongles/resources array, each
  coder's "last action" timestamp, and the shared output stream used for
  logging. Each critical section is kept as short as possible to minimize
  contention and to avoid holding a lock while performing time-consuming
  operations (such as sleeping).
- **`pthread_cond_t`:** used where threads need to wait efficiently for a
  condition to become true (for example, waiting for a resource to be released)
  instead of busy-waiting in a loop. This reduces CPU usage and ensures a
  waiting thread is woken up as soon as the resource it needs becomes available.
- **Custom event/state tracking:** a small set of helper functions wraps access
  to shared state (such as a coder's status or timestamp) so that every read or
  write goes through the same lock/unlock pattern, reducing the risk of a
  missed or inconsistent update.

**Race condition prevention example:** without synchronization, two coder
threads could simultaneously read that a dongle is available and both attempt
to take it, corrupting the resource count. By wrapping the "check and take"
sequence in a single mutex-protected critical section, only one thread can
evaluate and modify the resource state at a time, eliminating the race.

**Thread-safe communication example:** the monitor thread needs an up-to-date
view of every coder's last action timestamp without interfering with coders
that are actively working. Each coder updates its own timestamp under a
per-coder mutex immediately before and after eating/working, and the monitor
acquires that same mutex briefly just to read the value. This keeps the
critical section extremely short on both sides, so the monitor can poll
frequently with minimal impact on coder throughput.

## Resources

- *The Little Book of Semaphores* — Allen B. Downey, for a deep and approachable
  treatment of classic synchronization problems, including the dining
  philosophers problem.
- POSIX Threads Programming (`man pthread_mutex_init`, `man pthread_cond_wait`,
  `man pthread_create`) — the official man pages, used as the primary reference
  for correct API usage.
- Edsger W. Dijkstra's original writings on the dining philosophers problem, for
  historical context on the concurrency problem this project is modeled after.
- Coffman et al., *System Deadlocks* (1971) — for the formal definition of the
  four conditions necessary for deadlock, used as a checklist when designing the
  resource-acquisition strategy.

### AI usage disclosure

AI assistance was used during this project to help explain and clarify
concurrency concepts (such as the dining philosophers problem and Coffman's
conditions), to review draft code for potential race conditions and deadlock
scenarios, and to help structure and word this README. All core design
decisions, the synchronization logic itself, and the final implementation were
written and verified by the project author(s).
