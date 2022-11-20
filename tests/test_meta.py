source_filenames = ["test_vdlutil/test_vdlutil.c", "test_vdlerr/test_vdlerr.c", "test_vdlbt/test_vdlbt.c"]

expected_output = []
expected_exitcode = []

for filename in source_filenames:
    s = ""
    subtest_count = 0
    echo = False
    multiline_expect = False
    with open(filename, "r") as f:
        for line in f:
            if echo:
                s += '\t\t\t' + line.split('echo("')[1].replace('");\n', '\n')
                echo = False
                subtest_count = 0
                continue
            if "// echo" in line:
                echo = True
                continue

            if "// exit" in line:
                expected_exitcode.append(int(line.split("// exit(")[1].replace(")\n", "")))

            if "// expect end" in line:
                multiline_expect = False
                continue
            if multiline_expect:
                s += line.split("// ")[1]
                continue
            if "// expect(" in line:
                subtest_count += 1
                s += f"\t\t\t\tSubtest {subtest_count}: " + line.split("// expect(")[1].replace(")\n", "\n")
                continue
            if "// expect start" in line:
                multiline_expect = True
                subtest_count += 1
                s += f"\t\t\t\tSubtest {subtest_count}: "
                continue

    expected_output.append(s)
