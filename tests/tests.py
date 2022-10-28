import os

import test_meta


def compile_c(filename):
    exe_name = filename.replace('.c', '')
    command = f"clang -std=gnu17 -O3 -Wconversion -Wshadow -Wall -Wextra -Wpedantic {filename} -o {exe_name} 2> {exe_name}.compilemsg"
    print(f"\tCompiling ./{filename} with \"{command}\".")
    os.system(command)
    return exe_name


def run_c(exe_name):
    command = f"./{exe_name} 1> {exe_name}.out"
    print(f"\tRunning ./{exe_name}, saving results to ./{exe_name}.out.")
    return exe_name + ".out", os.system(command) >> 8


def label_diff(x, expected):
    target_len = len(x) if len(x) <= len(expected) else len(expected)

    diff_pos = next((i for i in range(target_len) if x[i] != expected[i]), None)
    if diff_pos is None:
        return x[0:target_len] + ">\n" + x[target_len:len(x)]
    if diff_pos == 0:
        return ">\n" + x
    last_newline = [i for i in range(target_len) if x[i] == "\n" and i < diff_pos]
    next_newline = [i for i in range(target_len) if x[i] == "\n" and i > diff_pos]
    if len(last_newline) == 0:
        last_newline = 0
    else:
        last_newline = last_newline[-1]
    if len(next_newline) == 0:
        next_newline = target_len
    else:
        next_newline = next_newline[0]
    num_tab = len([i for i in range(last_newline, next_newline) if x[i] == "\t"])
    x = x[0:next_newline] + "\n" + "^" * (next_newline - last_newline + num_tab * 4 - num_tab) + x[next_newline:len(x)]

    return x


def exam_c(output_name, expected_output, exit_code, expected_exitcode):
    output_flag = 0
    exit_flag = 0

    with open(output_name, "r") as f:
        content = f.read()
        if content == expected_output:
            output_flag = 1
            print("\tOutput matched!")
        else:
            content = label_diff(content, expected_output)
            print("\tOutput unmatched!")
            print(f"\t\tExpected output:\n{expected_output}")
            print(f"\t\tActual output:\n{content}")

    if exit_code == expected_exitcode:
        exit_flag = 1
        print(f"\tProgram exited as expected ({exit_code}).")
    else:
        print(f"\tProgram exited abnormally ({exit_code} <> {expected_exitcode})!")

    return output_flag, exit_flag


def run_test():
    num_passed = 0
    for i, filename in enumerate(test_meta.source_filenames):
        print(f"Test {i + 1}:")
        exe_name = compile_c(filename)
        output_name, exit_code = run_c(exe_name)
        output_flag, exit_flag = exam_c(output_name, test_meta.expected_output[i],
                                        exit_code, test_meta.expected_exitcode[i])
        if output_flag and exit_flag:
            num_passed += 1

    print("\nTest summary:")
    print(f"\t{num_passed}/{len(test_meta.source_filenames)} tests passed!")


if __name__ == '__main__':
    run_test()
