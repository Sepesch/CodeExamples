import pickle
from src.CommandHistory import CommandHistory
class SaveLoader:
    @staticmethod
    def serialize(max_size, commands):
        result = ""
        result = result + str(max_size) + "/" + str(len(commands)) + "/"
        for i in range(len(commands)-1):
            result = result + commands[i] + "/"
        result = result + commands[len(commands)-1]
        return result
    @staticmethod
    def save_to_file(filename, commands, max_size):
        data = SaveLoader.serialize(max_size, commands)
        with open(filename, "w") as file:
            file.write(data)


    @staticmethod
    def save_history(self):
        """Сохраняет текущую историю команд в файл."""
        with open("Tester.txt", "wb") as file:
            pickle.dump((self.max_size, self.commands), file)
    @staticmethod
    def load_history(self):
        """Загружает историю команд из файла."""
        try:
            with open("Tester.txt", "rb") as file:
                self.max_size, self.commands = pickle.load(file)
        except FileNotFoundError:
            # Если файл не найден, начинаем с пустой истории
            self.commands = []
        except Exception:
            print(f"Ошибка при загрузке состояния")
            self.commands = []
    @staticmethod
    def load_from_file(filename):
        try:
            new_his = CommandHistory(100)
            with open(filename, "r") as file:
                data = file.read()
            inp = data.split("/")
            new_his.max_size = int(inp[0])
            tmp = int(inp[1])
            for i in range(tmp):
                if (inp[i + 2] == None):
                    raise Exception("Improper amount of commands")
                    break
                if (i >= len(new_his.commands)):
                    new_his.commands.append(inp[i + 2])
                else:
                     new_his.commands[i] = inp[i + 2]
            return new_his
        except FileNotFoundError:
            print(f"Файл {filename} не найден.")
            new_his = CommandHistory(100)
            return new_his
        except Exception:
            print(f"Ошибка при загрузке состояния")
            new_his = CommandHistory(100)
            return new_his

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
    elif command == "Save":
        SaveLoader.save_history(commandHistory)
        continue
    elif command == "Load":
        SaveLoader.load_history(commandHistory)
        continue
    else:
        commandHistory.add_command(command)