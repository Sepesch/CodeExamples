from CommandHistory import CommandHistory
commandHistory = CommandHistory(100)
while True:
    command = input()
    if command == "print_his":
        print(commandHistory)
        continue
    elif command.split()[0] == "set_size":
        commandHistory.set_max_size(int(command.split()[1]))
        continue
    elif command == "break_hisProgram":
        break
    else:
        commandHistory.add_command(command)