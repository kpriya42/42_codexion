#!/usr/bin/env bash
# Run the Codexion test cases and save each run to outputN.txt.
# Usage: ./run_tests.sh [test_file]
# Optional: TIMEOUT_SECONDS=120 ./run_tests.sh

set -u

PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
INPUT_FILE=""
if (( $# > 1 )); then
    printf 'Usage: %s [test_file]\n' "$0" >&2
    exit 1
fi
if (( $# == 1 )); then
    # Resolve the optional input relative to the caller's current directory.
    case "$1" in
        /*) INPUT_FILE="$1" ;;
         *) INPUT_FILE="$PWD/$1" ;;
    esac
elif [[ -f "$PROJECT_DIR/test.txt" ]]; then
    INPUT_FILE="$PROJECT_DIR/test.txt"
fi
if [[ -n "$INPUT_FILE" && ! -f "$INPUT_FILE" ]]; then
    printf 'Test file not found: %s\n' "$INPUT_FILE" >&2
    exit 1
fi

cd "$PROJECT_DIR" || exit 1
if [[ ! -x ./codexion ]]; then
    printf 'Executable ./codexion not found. Build it first (e.g. make).\n' >&2
    exit 1
fi

TIMEOUT_SECONDS="${TIMEOUT_SECONDS:-60}"
if [[ ! "$TIMEOUT_SECONDS" =~ ^[1-9][0-9]*$ ]]; then
    printf 'TIMEOUT_SECONDS must be a positive integer.\n' >&2
    exit 1
fi
if command -v timeout >/dev/null 2>&1; then
    TIMEOUT_CMD=timeout
elif command -v gtimeout >/dev/null 2>&1; then
    TIMEOUT_CMD=gtimeout
else
    TIMEOUT_CMD=""
    printf 'Warning: timeout unavailable; a hanging test may block the script.\n' >&2
fi

# Each run gets a fresh folder, so old output files cannot mix with new ones.
OUTPUT_DIR="$PROJECT_DIR/test_outputs/run_$(date +%Y%m%d_%H%M%S)_$$"
mkdir -p "$OUTPUT_DIR" || exit 1
count=0

run_test() {
    local num="$1" title="$2" command_line="$3" purpose="$4"
    local output_file="$OUTPUT_DIR/output${num}.txt"
    local status
    local -a args

    if [[ -z "$command_line" || -z "$purpose" ]]; then
        printf 'Missing command or purpose for test %s.\n' "$num" >&2
        return 1
    fi

    # Input commands contain simple space-separated arguments, no shell syntax.
    read -r -a args <<< "$command_line"
    {
        printf 'Test number: %s\n' "$num"
        printf 'Test name: %s\n' "$title"
        printf 'Command: %s\n' "$command_line"
        printf 'Purpose: %s\n' "$purpose"
        printf '\n--- Command output (stdout + stderr) ---\n'
    } > "$output_file"

    printf 'Running test %s: %s\n' "$num" "$title"
    if [[ -n "$TIMEOUT_CMD" ]]; then
        "$TIMEOUT_CMD" --signal=TERM --kill-after=2s "${TIMEOUT_SECONDS}s" \
            "${args[@]}" >> "$output_file" 2>&1
        status=$?
    else
        "${args[@]}" >> "$output_file" 2>&1
        status=$?
    fi

    {
        printf '\n--- Execution status ---\n'
        printf 'Exit code: %s\n' "$status"
        if (( status == 124 )); then
            printf 'Timed out after %s seconds.\n' "$TIMEOUT_SECONDS"
        fi
    } >> "$output_file"

    count=$((count + 1))
    printf '  Saved: %s (exit %s)\n' "${output_file##*/}" "$status"
}

parse_tests() {
    local file="$1" line num="" title="" command_line="" purpose=""
    while IFS= read -r line || [[ -n "$line" ]]; do
        line="${line%$'\r'}"
        if [[ "$line" =~ ^TEST[[:space:]]+([0-9]+):[[:space:]]*(.*)$ ]]; then
            if [[ -n "$num" ]]; then
                run_test "$num" "$title" "$command_line" "$purpose" || return 1
            fi
            num="${BASH_REMATCH[1]}"
            title="${BASH_REMATCH[2]}"
            command_line=""
            purpose=""
        elif [[ "$line" =~ ^Command:[[:space:]]*(.*)$ ]]; then
            command_line="${BASH_REMATCH[1]}"
        elif [[ "$line" =~ ^Purpose:[[:space:]]*(.*)$ ]]; then
            purpose="${BASH_REMATCH[1]}"
        fi
    done < "$file"
    if [[ -n "$num" ]]; then
        run_test "$num" "$title" "$command_line" "$purpose" || return 1
    fi
}

if [[ -n "$INPUT_FILE" ]]; then
    parse_tests "$INPUT_FILE" || exit 1
else
    # Built-in copy of the 20 cases: only this .sh is needed in the project.
    parse_tests /dev/stdin <<'EMBEDDED_TESTS'
TEST 1: Single Coder
Command: ./codexion 1 1000 100 100 100 1 0 fifo
Purpose: Verify basic execution without contention

TEST 2: Two Coders FIFO
Command: ./codexion 2 2000 200 200 200 2 100 fifo
Purpose: Test FIFO scheduling fairness

TEST 3: Two Coders EDF
Command: ./codexion 2 2000 200 200 200 2 100 edf
Purpose: Test EDF scheduling by urgency

TEST 4: Four Coders High Contention
Command: ./codexion 4 3000 300 300 300 5 200 fifo
Purpose: Verify no circular wait deadlock under load

TEST 5: Five Coders EDF Load
Command: ./codexion 5 2500 200 200 200 4 100 edf
Purpose: Ensure atomic dongle acquisition prevents deadlock

TEST 6: FIFO Arrival Order Fairness
Command: ./codexion 3 3000 200 200 200 3 50 fifo
Purpose: Verify FIFO doesn't starve later requests

TEST 7: EDF Urgency Handling
Command: ./codexion 4 2000 300 300 300 3 100 edf
Purpose: Verify EDF prioritizes coder closest to burnout

TEST 8: Guaranteed Burnout Detection
Command: ./codexion 4 310 200 200 200 2 0 fifo
Purpose: Verify burnout is detected when inevitable

TEST 9: No Burnout Relaxed Parameters
Command: ./codexion 2 5000 300 300 300 3 50 edf
Purpose: Verify no false burnout with generous timeout

TEST 10: Burnout Edge Case
Command: ./codexion 2 500 250 250 250 2 100 fifo
Purpose: Test burnout detection at threshold boundary

TEST 11: Induced Burnout EDF
Command: ./codexion 3 400 180 180 180 3 100 edf
Purpose: Verify EDF still detects burnout under tight constraints

TEST 12: High Concurrency Six Coders
Command: ./codexion 6 2000 200 200 200 3 100 fifo
Purpose: Verify no data races with multiple threads

TEST 13: Atomic Dongle Acquisition
Command: ./codexion 5 2000 200 200 200 3 100 fifo
Purpose: Verify coders never hold partial dongle state

TEST 14: Zero Cooldown
Command: ./codexion 3 2000 200 200 200 3 0 fifo
Purpose: Verify immediate reuse prevents starvation

TEST 15: Extended Cooldown 500ms
Command: ./codexion 2 3000 300 300 300 2 500 edf
Purpose: Verify cooldown delays respected

TEST 16: Baseline FIFO
Command: ./codexion 5 800 200 200 200 2 50 fifo
Purpose: Reference FIFO implementation behavior

TEST 17: Baseline EDF
Command: ./codexion 5 800 200 200 200 2 50 edf
Purpose: Reference EDF implementation behavior

TEST 18: Maximum Concurrency Ten Coders
Command: ./codexion 10 3000 200 200 200 2 100 fifo
Purpose: Verify system handles high thread count

TEST 19: Long Mission Ten Cycles
Command: ./codexion 3 5000 200 200 200 10 100 fifo
Purpose: Verify no memory leaks over extended run

TEST 20: Invalid Arguments
Command: ./codexion
Purpose: Verify graceful handling of missing arguments
EMBEDDED_TESTS
fi
printf '\nFinished: %s tests. Reports: %s\n' "$count" "$OUTPUT_DIR"
