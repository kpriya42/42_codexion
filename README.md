*This project has been created as part of the 42 curriculum by kri-*.

# Description

**Codexion** is a multithreaded simulation of coders competing for shared dongles. Each coder repeatedly performs three stages: **compilation**, **debugging**, and **refactoring**. A coder needs two dongles to start compiling and releases them after compilation. Each dongle can be held by only one coder at a time and must cool down after release before it can be acquired again.

A dedicated monitor checks for burnout and completion. A coder burns out if the **next compilation has not started within `time_to_burnout` milliseconds of the previous compilation's start**. This deadline also applies while the coder is compiling, debugging, refactoring, or waiting for dongles. A coder is marked finished after completing debugging and refactoring in the required final cycle. **Implementation note:** the monitor currently excludes a coder from burnout checks as soon as its compilation count reaches the required count, even if its final debugging/refactoring stages are still running.

The simulation supports two dongle request policies:

- **FIFO** (`fifo`): serve requests in arrival order.
- **EDF** (`edf`, Earliest Deadline First): prioritize the request with the earliest burnout deadline.

# Instructions

## Compilation

From the project root, run:

```sh
cd coders
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
| `number_of_coders` | Number of concurrent coder threads (1–200; `MAX_CODERS` is an exclusive upper bound in the current validation) |
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

The program prints timestamped coder events. The simulation stops when the monitor detects burnout or when all coders have completed their required cycles. In the single-coder case, the coder can acquire only one dongle and cannot proceed to compilation; burnout ends the simulation.

# Blocking cases handled

## **Mutual exclusion:** 
A dongle can be owned by only one coder at a time. Its ownership and request state are protected by its mutex.

## **Deadlock prevention / Coffman's conditions:** 
Each coder acquires the lower-address dongle first and then the higher-address dongle. This consistent total ordering prevents circular wait, breaking one of Coffman's four necessary conditions (mutual exclusion, hold-and-wait, no preemption, circular wait). With one coder, the identical second dongle is not acquired; the monitor can still detect burnout.

## **Starvation and scheduling:**
Each dongle stores up to two pending requests, since at most two adjacent coders need it. FIFO assigns increasing request numbers; EDF assigns the coder's next compilation deadline. The smallest priority wins when the dongle is available. FIFO provides request-order fairness locally; EDF prioritizes the most urgent deadline, but neither guarantees that every coder will avoid burnout.

## **Dongle cooldown:** 
Releasing a dongle records its release time. A waiting coder can acquire it only after its cooldown has elapsed; a timed condition wait avoids busy-waiting during that interval.

## **Burnout detection:** 
The monitor compares the current time with each coder's previous compilation start time. A coder can burn out before completing the ongoing compile/debug/refactor cycle. Finishing status and burnout timing are separate concepts.

## **Simulation shutdown:** 
The monitor sets the shared state to `STOP`, broadcasts `cond_sim`, then broadcasts each dongle's `cond_dongle` so blocked threads can recheck the state and exit. If coder-thread creation fails, the creation helper stops the simulation and joins the threads successfully created before returning an error.

## **Log serialization:**
A dedicated printing mutex protects each complete formatted log message so output from multiple threads does not interleave. Timestamps are relative to the shared simulation start time.


# Thread synchronization mechanisms

The project uses POSIX threads (`pthread`) and the following synchronization primitives:

- **`pthread_t`:** One thread per coder and a separate monitor thread. Threads are joined before their shared resources are destroyed.

- **`pthread_mutex_t`:** Each dongle has a mutex guarding its owner, release time, and priority queue. Each coder has a mutex guarding state read by the monitor, including the previous compilation start timestamp and completion information. The simulation-control mutex guards the global simulation status; the logger mutex serializes output

- **`pthread_cond_t`:** The simulation condition variable provides a common start barrier and wakes sleeping threads when the simulation stops. Each dongle's condition variable allows coders to sleep while waiting for ownership, cooldown, or their turn in the queue. `pthread_cond_timedwait()` is used for deadlines rather than repeatedly polling the clock.

**Example: preventing a race on a compilation timestamp.** The coder locks its state mutex while updating the compilation start time. The monitor takes the same mutex to read it. This ensures that the monitor never observes an unsynchronized concurrent update.

**Example: waiting for a dongle.** A coder locks the dongle mutex, inserts its request, and checks whether the dongle is free, cooled down, and available to that coder under the selected policy. If not, it waits on the dongle condition variable. The wait temporarily releases the dongle mutex and reacquires it before returning. The condition is checked again after every wakeup, including spurious wakeups.

**Example: communication with the monitor.** The monitor reads each coder's compilation timestamp and count under its `mutex_coder`, decides whether burnout has occurred, and changes `sim_status` under `mutex_sim`. It broadcasts `cond_sim` and each dongle's condition variable on shutdown. `sleep_ms()` uses timed condition waits to react to `STOP` without waiting for its full requested duration.

**Resource lifetime:** The initialization code checks each allocation and synchronization-object initialization. When a step fails, the corresponding cleanup helper destroys only previously initialized mutexes and conditions and frees the allocated arrays. On normal termination, all threads are joined before `destroy_all()` releases shared resources.

# Resources

- [POSIX threads — Linux manual pages (`pthreads(7)`)](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Mutex locking — `pthread_mutex_lock(3p)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [Condition variables — `pthread_cond_wait(3p)`](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)
- [Condition broadcasts — `pthread_cond_broadcast(3p)`](https://man7.org/linux/man-pages/man3/pthread_cond_broadcast.3p.html)
- [Deadlock and Coffman conditions](https://en.wikipedia.org/wiki/Deadlock)
- [Priority queue implementation in C — GeeksforGeeks](https://www.geeksforgeeks.org/c/c-program-to-implement-priority-queue/)
- [C programming / threading video resource — YouTube playlist](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

## AI usage

ChatGPT was used as a learning and review aid for POSIX thread creation and joining, mutexes, condition variables and timed waits, thread synchronization, burnout timing, and error-path/resource cleanup. It also helped review potential concurrency issues and draft the documentation in this README.
Implementation and testing remain the author's responsibility.
