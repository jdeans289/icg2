import sys
import os

if len(sys.argv) < 3:
    print("Usage: python3 test_driver.py <working_dir> <test_exectuable> [executable args]")

working_dir = sys.argv[1]
executable_invocation = " ".join(sys.argv[2:])

command = "cd " + working_dir + " && " + executable_invocation

ret = os.system(command)

if ret != 0:
    exit(1)

exit(0)
