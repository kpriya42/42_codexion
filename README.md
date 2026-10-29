*This project has been created as part of the 42 curriculum by kri-*.

# Description

**Codexion** is a multithreaded simulation of coders competing for shared dongles. Each coder repeatedly performs three stages: **compilation**, **debugging**, and **refactoring**. A coder needs two dongles to start compiling; each dongle can be held by only one coder at a time and must cool down after release before it can be acquired again.

A dedicated monitor checks for burnout and completion. A coder burns out if the **next compilation has not started within `time_to_burnout` milliseconds of the previous compilation's start**. This deadline also applies while the coder is compiling, debugging, refactoring, or waiting for dongles. A coder is considered finished only after completing all three stages of the required final cycle.

The simulation supports two dongle request policies:

- **FIFO** (`fifo`): serve requests in arrival order.
- **EDF** (`edf`, Earliest Deadline First): prioritize the request with the earliest burnout deadline.

# Instructions

## Compilation

From the project root, run:

```sh
make
```

The Makefile uses `cc` with `-Wall -Wextra -Werror -pthread` and produces the executable `codexion`.

Other Makefile targets:

```sh
make clean   # Remove object files
make fclean  # Remove object files and the executable
make re      # Rebuild from scratch
```

## Execution

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Arguments, in order:

| Argument | Description |
| --- | --- |
| `number_of_coders` | Number of concurrent coder threads (maximum 200) |
| `time_to_burnout` | Maximum time in milliseconds between the starts of successive compilations |
| `time_to_compile` | Compilation duration in milliseconds |
| `time_to_debug` | Debugging duration in milliseconds |
| `time_to_refactor` | Refactoring duration in milliseconds |
| `number_of_compiles_required` | Number of complete cycles each coder must finish |
| `dongle_cooldown` | Minimum time in milliseconds after release before a dongle can be reused |
| `scheduler` | `fifo` or `edf` |

Example:

```sh
./codexion 5 1000 200 150 100 2 60 fifo
```

The program prints timestamped coder events. The simulation stops when the monitor detects burnout or when all coders have completed their required cycles.

# Blocking cases handled

- **Mutual exclusion:** A dongle can be owned by only one coder at a time. Its ownership and request state are protected by its mutex.
- **Deadlock prevention / Coffman's conditions:** The dongle acquisition protocol must account for the four deadlock conditions: mutual exclusion, hold-and-wait, no preemption, and circular wait. Mutual exclusion is inherent to the problem; the implementation must ensure that waiting for two dongles cannot leave all coders permanently blocked. A mutex alone does **not** prevent circular wait.
- **Starvation and scheduling:** Waiting requests are ordered according to FIFO arrival order or EDF deadline priority. These policies determine who gets a newly available dongle, but priority ordering by itself does not guarantee that every coder finishes before burnout.
- **Dongle cooldown:** Releasing a dongle records its release time. A waiting coder can acquire it only after its cooldown has elapsed; a timed condition wait avoids busy-waiting during that interval.
- **Burnout detection:** The monitor compares the current time with each coder's previous compilation start time. A coder can burn out before completing the ongoing compile/debug/refactor cycle. Finishing status and burnout timing are separate concepts.
- **Simulation shutdown:** On stop, sleeping coders and threads waiting for dongles must be notified so they can exit without remaining blocked indefinitely.
- **Log serialization:** A dedicated printing mutex prevents output from multiple threads from interleaving.

# Thread synchronization mechanisms

The project uses POSIX threads (`pthread`) and the following synchronization primitives:

- **`pthread_t`:** One thread per coder and a separate monitor thread. Threads are joined before their shared resources are destroyed.
- **`pthread_mutex_t`:** Each dongle has a mutex guarding its owner, release time, and priority queue. Each coder has a mutex guarding state read by the monitor, including the previous compilation start timestamp and completion information. The simulation-control mutex guards the global simulation status; the logger mutex serializes output.
- **`pthread_cond_t`:** The simulation condition variable provides a common start barrier and wakes sleeping threads when the simulation stops. Each dongle's condition variable allows coders to sleep while waiting for ownership, cooldown, or their turn in the queue. `pthread_cond_timedwait()` is used for deadlines rather than repeatedly polling the clock.

**Example: preventing a race on a compilation timestamp.** The coder locks its state mutex while updating the compilation start time. The monitor takes the same mutex to read it. This ensures that the monitor never observes an unsynchronized concurrent update.

**Example: waiting for a dongle.** A coder locks the dongle mutex, inserts its request, and checks whether the dongle is free, cooled down, and available to that coder under the selected policy. If not, it waits on the dongle condition variable. The wait temporarily releases the dongle mutex and reacquires it before returning. The condition is checked again after every wakeup, including spurious wakeups.

**Example: communication with the monitor.** The monitor changes the simulation status while holding the simulation mutex and broadcasts the simulation condition. Dongle waiters also need notification on shutdown because a broadcast on the simulation condition does not wake a thread waiting on a dongle condition.

# Resources

- [POSIX threads — Linux manual pages (`pthreads(7)`)](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Mutex locking — `pthread_mutex_lock(3p)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [Condition variables — `pthread_cond_wait(3p)`](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)
- [Condition broadcasts — `pthread_cond_broadcast(3p)`](https://man7.org/linux/man-pages/man3/pthread_cond_broadcast.3p.html)
- [Deadlock and Coffman conditions](https://en.wikipedia.org/wiki/Deadlock)
- [Priority queue implementation in C — GeeksforGeeks](https://www.geeksforgeeks.org/c/c-program-to-implement-priority-queue/)
- [C programming / threading video resource — YouTube playlist](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

## AI usage

ChatGPT was used as a learning and review aid for POSIX thread creation and joining, mutexes, condition variables and timed waits, thread synchronization, burnout timing, and error-path/resource cleanup. It also helped review potential concurrency issues and draft the documentation in this README. Implementation and testing remain the author's responsibility.
